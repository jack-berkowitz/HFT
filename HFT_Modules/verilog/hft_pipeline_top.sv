`include "sys_defs.svh"
// ============================================================
// hft_pipeline_top.sv
//
// Top-level module for the 7-stage HFT pipeline:
//
//   Stage 1: ipv4_udp_port_filter  — Ethernet/IP/UDP filter
//   Stage 2: xdp_packet_framer    — Strip 58-byte header, realign
//   Stage 3: xdp_msg_demux        — Decode XDP messages → pillar_msg_t
//   Stage 4: order_lookup          — Hash-table order state tracker
//            (instantiates skewed_hash_table internally)
//   Stage 5: top_of_book           — Sorted N-level order book
//   Stage 6: index_arb_engine      — Weighted index + spread detector
//   Stage 7: order_gen_tx          — Risk checks + TX packet generation
//
// External interfaces:
//   RX:  AXI-Stream slave  (from 10G MAC / Corundum)
//   TX:  AXI-Stream master (to 10G MAC / Corundum)
//   CFG: Weight load port, threshold, enable, monitoring ports
//
// ============================================================
module hft_pipeline_top #(
    // ---- Filter ----
    parameter [15:0] FILTER_PORT    = `XDP_PORT,

    // ---- Framer ----
    // Filter strips beats 0-5 (Eth+IP+UDP = 48 bytes). Framer sees
    // only the residual: 58-48 = 10 bytes of XDP packet header.
    parameter        HEADER_BYTES   = `XDP_CHAIN_FRAMER_HDR,

    // ---- Order lookup (hash table) ----
    parameter        HT_N_WAYS      = 4,
    parameter        HT_TBL_ENTRIES = 4096,
    parameter        HT_MAX_CHAIN   = 16,

    // ---- Top of book ----
    parameter        TOB_N_LEVELS   = `TOB_LEVELS,
    parameter        TOB_N_SYMBOLS  = `TOB_SYMBOLS,

    // ---- Index arb engine ----
    parameter        N_COMPONENTS   = 500,
    parameter [8:0]  IDX_SYM        = `IDX_SYMBOL,

    // ---- Order gen TX ----
    parameter [47:0] SRC_MAC        = 48'hDE_AD_BE_EF_CA_FE,
    parameter [47:0] DST_MAC        = 48'h01_00_5E_00_00_01,
    parameter [31:0] SRC_IP         = {8'd10, 8'd0, 8'd0, 8'd1},
    parameter [31:0] DST_IP         = {8'd10, 8'd0, 8'd0, 8'd2},
    parameter [15:0] SRC_PORT       = 16'd12345,
    parameter [15:0] DST_PORT       = 16'd54321,
    parameter [31:0] ORDER_QTY      = 32'd100,
    parameter [31:0] COOLDOWN_CYC   = 32'd100,
    parameter [31:0] MAX_POS        = 32'd1000,
    parameter [31:0] PRICE_OFFSET   = 32'd10,
    parameter [31:0] IDX_SYM_ID     = `IDX_SYMBOL
)(
    input  logic              clk,
    input  logic              rst_n,

    // ================================================================
    // RX AXI-Stream (from 10G MAC)
    // ================================================================
    input  AXI_TDATA          rx_axis_tdata,
    input  AXI_TKEEP          rx_axis_tkeep,
    input  logic              rx_axis_tvalid,
    input  logic              rx_axis_tlast,
    output logic              rx_axis_tready,

    // ================================================================
    // TX AXI-Stream (to 10G MAC)
    // ================================================================
    output AXI_TDATA          tx_axis_tdata,
    output AXI_TKEEP          tx_axis_tkeep,
    output logic              tx_axis_tvalid,
    output logic              tx_axis_tlast,
    input  logic              tx_axis_tready,

    // ================================================================
    // Configuration inputs
    // ================================================================

    // Order gen enable gate
    input  logic              tx_enable,

    // Index arb weight load port (written once at startup)
    input  logic                      wt_wr_en,
    input  logic [`SYM_IDX_W-1:0]    wt_wr_addr,
    input  logic [`WEIGHT_W-1:0]     wt_wr_data,

    // Spread threshold (unsigned, Q44.20 fixed-point)
    input  logic [`ACCUM_W-1:0]      threshold,

    // ================================================================
    // Status / monitoring outputs
    // ================================================================

    // Order lookup ready (hash table initialised)
    output logic              ol_ready,

    // Top-of-book BBO snapshot
    output tob_out_t          tob_snapshot,

    // Trade signal from index arb engine
    output trade_signal_t     trade_signal,

    // Current computed index value (Q44.20)
    output logic [`ACCUM_W-1:0] computed_index,

    // TX statistics
    output logic [31:0]       order_count,
    output logic signed [31:0] net_position,
    output logic              pkt_sent
);

    // ================================================================
    // Internal wires
    // ================================================================

    // Stage 1 → Stage 2:  filter → framer
    AXI_TDATA  f2f_tdata;
    AXI_TKEEP  f2f_tkeep;
    logic      f2f_tvalid, f2f_tlast, f2f_tready;

    // Stage 2 → Stage 3:  framer → demux
    AXI_TDATA  f2d_tdata;
    AXI_TKEEP  f2d_tkeep;
    logic      f2d_tvalid, f2d_tlast, f2d_tready;

    // Stage 3 → Stage 4:  demux → order_lookup
    pillar_msg_t decoded_msg;

    // Stage 4 → Stage 5:  order_lookup → top_of_book
    order_lookup_out_t ol_out;

    // Stage 5 → Stage 6:  top_of_book → index_arb_engine
    tob_out_t tob_out;

    // Stage 6 → Stage 7:  index_arb_engine → order_gen_tx
    trade_signal_t arb_trade;

    // ================================================================
    // Stage 1: IPv4/UDP Port Filter
    // ================================================================
    ipv4_udp_port_filter #(
        .FILTER_PORT (FILTER_PORT)
    ) u_filter (
        .clk            (clk),
        .rst_n          (rst_n),
        // RX input
        .s_axis_tdata   (rx_axis_tdata),
        .s_axis_tkeep   (rx_axis_tkeep),
        .s_axis_tvalid  (rx_axis_tvalid),
        .s_axis_tlast   (rx_axis_tlast),
        .s_axis_tready  (rx_axis_tready),
        // To framer
        .m_axis_tdata   (f2f_tdata),
        .m_axis_tkeep   (f2f_tkeep),
        .m_axis_tvalid  (f2f_tvalid),
        .m_axis_tlast   (f2f_tlast),
        .m_axis_tready  (f2f_tready)
    );

    // ================================================================
    // Stage 2: XDP Packet Framer
    // ================================================================
    xdp_packet_framer #(
        .HEADER_BYTES (HEADER_BYTES)
    ) u_framer (
        .clk            (clk),
        .rst_n          (rst_n),
        // From filter
        .s_axis_tdata   (f2f_tdata),
        .s_axis_tkeep   (f2f_tkeep),
        .s_axis_tvalid  (f2f_tvalid),
        .s_axis_tlast   (f2f_tlast),
        .s_axis_tready  (f2f_tready),
        // To demux
        .m_axis_tdata   (f2d_tdata),
        .m_axis_tkeep   (f2d_tkeep),
        .m_axis_tvalid  (f2d_tvalid),
        .m_axis_tlast   (f2d_tlast),
        .m_axis_tready  (f2d_tready)
    );

    // ================================================================
    // Stage 3: XDP Message Demux
    // ================================================================
    xdp_msg_demux u_demux (
        .clk            (clk),
        .rst_n          (rst_n),
        // From framer
        .s_axis_tdata   (f2d_tdata),
        .s_axis_tkeep   (f2d_tkeep),
        .s_axis_tvalid  (f2d_tvalid),
        .s_axis_tlast   (f2d_tlast),
        .s_axis_tready  (f2d_tready),
        // Decoded output
        .out_msg        (decoded_msg)
    );

    // ================================================================
    // Stage 4: Order Lookup (Hash Table)
    // ================================================================
    order_lookup #(
        .N_WAYS      (HT_N_WAYS),
        .TBL_ENTRIES (HT_TBL_ENTRIES),
        .MAX_CHAIN   (HT_MAX_CHAIN)
    ) u_order_lookup (
        .clk    (clk),
        .rst_n  (rst_n),
        .ready  (ol_ready),
        .msg_in (decoded_msg),
        .out    (ol_out)
    );

    // ================================================================
    // Stage 5: Top of Book
    // ================================================================
    top_of_book #(
        .N_LEVELS  (TOB_N_LEVELS),
        .N_SYMBOLS (TOB_N_SYMBOLS)
    ) u_tob (
        .clk       (clk),
        .rst_n     (rst_n),
        .in_update (ol_out),
        .ready     (),
        .out_tob   (tob_out)
    );

    // ================================================================
    // Stage 6: Index Arbitrage Engine
    // ================================================================
    index_arb_engine #(
        .N_COMPONENTS (N_COMPONENTS),
        .IDX_SYM      (IDX_SYM)
    ) u_index_arb (
        .clk        (clk),
        .rst_n      (rst_n),
        .in_tob     (tob_out),
        .wt_wr_en   (wt_wr_en),
        .wt_wr_addr (wt_wr_addr),
        .wt_wr_data (wt_wr_data),
        .threshold  (threshold),
        .ready      (),
        .out_trade  (arb_trade),
        .out_index  (computed_index)
    );

    // ================================================================
    // Stage 7: Order Generation + TX
    // ================================================================
    order_gen_tx #(
        .SRC_MAC       (SRC_MAC),
        .DST_MAC       (DST_MAC),
        .SRC_IP        (SRC_IP),
        .DST_IP        (DST_IP),
        .SRC_PORT      (SRC_PORT),
        .DST_PORT      (DST_PORT),
        .ORDER_QTY     (ORDER_QTY),
        .COOLDOWN_CYC  (COOLDOWN_CYC),
        .MAX_POS       (MAX_POS),
        .PRICE_OFFSET  (PRICE_OFFSET),
        .IDX_SYM_ID    (IDX_SYM_ID)
    ) u_tx (
        .clk          (clk),
        .rst_n        (rst_n),
        .enable       (tx_enable),
        .in_trade     (arb_trade),
        // TX output
        .m_axis_tdata (tx_axis_tdata),
        .m_axis_tkeep (tx_axis_tkeep),
        .m_axis_tvalid(tx_axis_tvalid),
        .m_axis_tlast (tx_axis_tlast),
        .m_axis_tready(tx_axis_tready),
        // Monitoring
        .order_count  (order_count),
        .net_position (net_position),
        .pkt_sent     (pkt_sent)
    );

    // ================================================================
    // Monitoring tap-outs
    // ================================================================
    assign tob_snapshot = tob_out;
    assign trade_signal = arb_trade;

endmodule
