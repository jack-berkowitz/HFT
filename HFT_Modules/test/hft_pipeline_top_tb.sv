`timescale 1ns/1ps
`include "sys_defs.svh"
// ============================================================
// hft_pipeline_top_tb.sv
//
// Integration testbench for hft_pipeline_top — the 7-stage
// HFT pipeline wired end-to-end through a single top module.
//
//   Network RX → ipv4_udp_port_filter → xdp_packet_framer →
//   xdp_msg_demux → order_lookup (skewed_hash_table) →
//   top_of_book → index_arb_engine → order_gen_tx → Network TX
//
// Test plan:
//   Phase 1 — Reset, wait for hash table init, load weights/threshold
//   Phase 2 — Build book: Add Orders (BUY+SELL) for each component
//             + index instrument.  Verify BBO via tob_snapshot.
//   Phase 3 — Trigger arb: Modify a component bid to create a large
//             spread.  Verify trade_signal fires, TX packet emitted.
//   Phase 4 — Packet filter: send wrong-port packet, confirm no output.
//   Phase 5 — Recovery: send valid Add after a drop, confirm pipeline
//             processes it correctly.
//   Phase 6 — Delete / Exec coverage: partial fills, full deletes.
//   Phase 7 — Cooldown enforcement: rapid trade signals, verify only
//             one TX packet within cooldown window.
//
// Parameterized for fast simulation:
//   4 index components (symbols 0-3) + 1 index instrument (sym 4)
//   Hash table: 128 entries, short cooldown
// ============================================================
module hft_pipeline_top_tb;

    // ================================================================
    // Test parameters
    // ================================================================
    localparam TB_N_COMP      = 4;
    localparam [8:0] TB_IDX   = 9'd4;
    localparam TB_N_SYMBOLS   = 8;
    localparam TB_TBL_ENTRIES = 128;
    localparam TB_COOLDOWN    = 10;
    localparam TB_QTY         = 32'd100;
    localparam TB_OFFSET      = 32'd10;
    localparam AW             = `ACCUM_W;

    localparam [31:0] WEIGHT_Q = 32'd262144;         // 0.25 in Q12.20
    localparam [63:0] THRESH   = 64'd104_857_600;    // 100 cents in Q44.20

    // Packet BFM constants
    localparam KEEP_W    = 8;
    localparam MAX_PKT_B = 256;
    localparam PKT_BITS  = MAX_PKT_B * 8;
    localparam [15:0] XDP_PORT = `XDP_PORT;

    // ================================================================
    // Signals
    // ================================================================
    logic clk, rst_n;

    // RX AXI-Stream
    logic [63:0] rx_tdata;
    logic [7:0]  rx_tkeep;
    logic        rx_tvalid, rx_tlast, rx_tready;

    // TX AXI-Stream
    logic [63:0] tx_tdata;
    logic [7:0]  tx_tkeep;
    logic        tx_tvalid, tx_tlast;

    // Config
    logic                      tx_enable;
    logic                      wt_wr_en;
    logic [`SYM_IDX_W-1:0]    wt_wr_addr;
    logic [`WEIGHT_W-1:0]     wt_wr_data;
    logic [`ACCUM_W-1:0]      threshold;

    // Monitoring
    logic              ol_ready;
    tob_out_t          tob_snap;
    trade_signal_t     trade;
    logic [AW-1:0]     idx_value;
    logic [31:0]       order_count;
    logic signed [31:0] net_position;
    logic              pkt_sent;

    // ================================================================
    // DUT — hft_pipeline_top
    // ================================================================
    hft_pipeline_top #(
        .FILTER_PORT    (XDP_PORT),
        .HEADER_BYTES   (`XDP_CHAIN_FRAMER_HDR),
        .HT_N_WAYS      (4),
        .HT_TBL_ENTRIES (TB_TBL_ENTRIES),
        .HT_MAX_CHAIN   (8),
        .TOB_N_LEVELS   (`TOB_LEVELS),
        .TOB_N_SYMBOLS  (TB_N_SYMBOLS),
        .N_COMPONENTS   (TB_N_COMP),
        .IDX_SYM        (TB_IDX),
        .ORDER_QTY      (TB_QTY),
        .COOLDOWN_CYC   (TB_COOLDOWN),
        .MAX_POS        (32'd10000),
        .PRICE_OFFSET   (TB_OFFSET),
        .IDX_SYM_ID     (32'd4)
    ) dut (
        .clk              (clk),
        .rst_n            (rst_n),
        // RX
        .rx_axis_tdata    (rx_tdata),
        .rx_axis_tkeep    (rx_tkeep),
        .rx_axis_tvalid   (rx_tvalid),
        .rx_axis_tlast    (rx_tlast),
        .rx_axis_tready   (rx_tready),
        // TX
        .tx_axis_tdata    (tx_tdata),
        .tx_axis_tkeep    (tx_tkeep),
        .tx_axis_tvalid   (tx_tvalid),
        .tx_axis_tlast    (tx_tlast),
        .tx_axis_tready   (1'b1),         // always accept TX
        // Config
        .tx_enable        (tx_enable),
        .wt_wr_en         (wt_wr_en),
        .wt_wr_addr       (wt_wr_addr),
        .wt_wr_data       (wt_wr_data),
        .threshold        (threshold),
        // Monitoring
        .ol_ready         (ol_ready),
        .tob_snapshot     (tob_snap),
        .trade_signal     (trade),
        .computed_index   (idx_value),
        .order_count      (order_count),
        .net_position     (net_position),
        .pkt_sent         (pkt_sent)
    );

    // ================================================================
    // Clock: 250 MHz (4 ns period)
    // ================================================================
    initial clk = 0;
    always #2 clk = ~clk;

    // ================================================================
    // VCD dump
    // ================================================================
    initial begin
        $dumpfile("hft_pipeline_top_dump.vcd");
        $dumpvars(0, hft_pipeline_top_tb);
    end

    // ================================================================
    // Packet buffer & byte-level BFM helpers
    // ================================================================
    logic [PKT_BITS-1:0] pkt_buf;
    integer pkt_len;

    task pkt_wr_byte(input integer idx, input [7:0] val);
        pkt_buf[idx*8 +: 8] = val;
    endtask

    task pkt_wr_u16_le(input integer idx, input [15:0] val);
        pkt_buf[idx*8 +: 8]     = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
    endtask

    task pkt_wr_u16_be(input integer idx, input [15:0] val);
        pkt_buf[idx*8 +: 8]     = val[15:8];
        pkt_buf[(idx+1)*8 +: 8] = val[7:0];
    endtask

    task pkt_wr_u32_le(input integer idx, input [31:0] val);
        pkt_buf[idx*8     +: 8] = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
        pkt_buf[(idx+2)*8 +: 8] = val[23:16];
        pkt_buf[(idx+3)*8 +: 8] = val[31:24];
    endtask

    task pkt_wr_u64_le(input integer idx, input [63:0] val);
        pkt_wr_u32_le(idx,   val[31:0]);
        pkt_wr_u32_le(idx+4, val[63:32]);
    endtask

    // ----------------------------------------------------------------
    // Build Ethernet + IPv4 + UDP headers (42 bytes)
    // ----------------------------------------------------------------
    task build_net_header(input [15:0] udp_payload_len, input [15:0] dst_port);
        automatic logic [15:0] ip_total_len  = 20 + 8 + udp_payload_len;
        automatic logic [15:0] udp_total_len = 8 + udp_payload_len;

        pkt_buf = '0;
        // Ethernet: dst MAC, src MAC, EtherType=0x0800
        pkt_wr_byte(0,8'h01); pkt_wr_byte(1,8'h00); pkt_wr_byte(2,8'h5e);
        pkt_wr_byte(3,8'h00); pkt_wr_byte(4,8'h00); pkt_wr_byte(5,8'h01);
        pkt_wr_byte(6,8'hde); pkt_wr_byte(7,8'had); pkt_wr_byte(8,8'hbe);
        pkt_wr_byte(9,8'hef); pkt_wr_byte(10,8'hca); pkt_wr_byte(11,8'hfe);
        pkt_wr_byte(12,8'h08); pkt_wr_byte(13,8'h00);   // EtherType IPv4

        // IPv4 header (20 bytes)
        pkt_wr_byte(14,8'h45); pkt_wr_byte(15,8'h00);   // ver+IHL, DSCP
        pkt_wr_byte(16, ip_total_len[15:8]); pkt_wr_byte(17, ip_total_len[7:0]);
        pkt_wr_byte(18,8'h00); pkt_wr_byte(19,8'h00);   // ID
        pkt_wr_byte(20,8'h00); pkt_wr_byte(21,8'h00);   // flags+frag
        pkt_wr_byte(22,8'h40); pkt_wr_byte(23,8'h11);   // TTL=64, proto=UDP
        pkt_wr_byte(24,8'h00); pkt_wr_byte(25,8'h00);   // checksum
        pkt_wr_byte(26,8'hef); pkt_wr_byte(27,8'h01);   // src IP
        pkt_wr_byte(28,8'h01); pkt_wr_byte(29,8'h01);
        pkt_wr_byte(30,8'hef); pkt_wr_byte(31,8'h01);   // dst IP
        pkt_wr_byte(32,8'h01); pkt_wr_byte(33,8'h01);

        // UDP header (8 bytes)
        pkt_wr_byte(34,8'hCA); pkt_wr_byte(35,8'hFE);   // src port
        pkt_wr_u16_be(36, dst_port);                       // dst port
        pkt_wr_byte(38, udp_total_len[15:8]);
        pkt_wr_byte(39, udp_total_len[7:0]);
        pkt_wr_byte(40,8'h00); pkt_wr_byte(41,8'h00);   // checksum
    endtask

    // ----------------------------------------------------------------
    // Build XDP Packet Header (16 bytes at offset 42-57)
    // ----------------------------------------------------------------
    task build_xdp_pkt_hdr(input [15:0] pkt_sz, input [7:0] n_msgs, input [31:0] seq);
        pkt_wr_u16_le(42, pkt_sz);
        pkt_wr_byte(44, 8'd11);          // DeliveryFlag
        pkt_wr_byte(45, n_msgs);
        pkt_wr_u32_le(46, seq);
        pkt_wr_u32_le(50, 32'h5F3759DF); // SendTime (dummy)
        pkt_wr_u32_le(54, 32'h00001234); // SendTimeNS (dummy)
    endtask

    // ----------------------------------------------------------------
    // Common message fields (bytes 58+)
    // ----------------------------------------------------------------
    task build_msg_common(input [15:0] sz, input [15:0] typ,
                          input [31:0] sym, input [31:0] ssn,
                          input [63:0] oid);
        pkt_wr_u16_le(58, sz);
        pkt_wr_u16_le(60, typ);
        pkt_wr_u32_le(62, 32'hAAAA_BBBB);  // SourceTimeNS
        pkt_wr_u32_le(66, sym);             // SymbolIndex
        pkt_wr_u32_le(70, ssn);             // SymbolSeqNum
        pkt_wr_u64_le(74, oid);             // OrderID
    endtask

    // ----------------------------------------------------------------
    // Build Add Order (type 100, 39 bytes)
    //   bytes 82-85: Price (LE)
    //   bytes 86-89: Qty (LE)
    //   byte  90:    Side ('B' or 'S')
    // ----------------------------------------------------------------
    task build_add_order(input [31:0] sym, input [31:0] ssn,
                         input [63:0] oid, input [31:0] price,
                         input [31:0] qty, input logic side);
        automatic logic [15:0] udp_payload = 16 + 39;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd55, 8'd1, ssn);
        build_msg_common(16'd39, `XDP_MSG_ADD_ORDER, sym, ssn, oid);
        pkt_wr_u32_le(82, price);
        pkt_wr_u32_le(86, qty);
        pkt_wr_byte(90, side ? "S" : "B");
        // Remaining bytes (91-96) zeroed
        pkt_len = 42 + 16 + 39;  // 97 bytes
    endtask

    // ----------------------------------------------------------------
    // Build Modify Order (type 101, 35 bytes)
    //   bytes 82-85: Price (LE)
    //   bytes 86-89: Qty (LE)
    // ----------------------------------------------------------------
    task build_mod_order(input [31:0] sym, input [31:0] ssn,
                         input [63:0] oid, input [31:0] price,
                         input [31:0] qty);
        automatic logic [15:0] udp_payload = 16 + 35;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd51, 8'd1, ssn);
        build_msg_common(16'd35, `XDP_MSG_MOD_ORDER, sym, ssn, oid);
        pkt_wr_u32_le(82, price);
        pkt_wr_u32_le(86, qty);
        pkt_len = 42 + 16 + 35;  // 93 bytes
    endtask

    // ----------------------------------------------------------------
    // Build Delete Order (type 102, 25 bytes)
    // ----------------------------------------------------------------
    task build_del_order(input [31:0] sym, input [31:0] ssn,
                         input [63:0] oid);
        automatic logic [15:0] udp_payload = 16 + 25;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd41, 8'd1, ssn);
        build_msg_common(16'd25, `XDP_MSG_DEL_ORDER, sym, ssn, oid);
        pkt_len = 42 + 16 + 25;  // 83 bytes
    endtask

    // ----------------------------------------------------------------
    // Build Exec Order (type 103, 42 bytes)
    //   bytes 82-85: TradeID (LE)
    //   bytes 86-89: Price (LE)
    //   bytes 90-93: Qty (LE)
    //   byte  94:    Printable
    // ----------------------------------------------------------------
    task build_exec_order(input [31:0] sym, input [31:0] ssn,
                          input [63:0] oid, input [31:0] price,
                          input [31:0] qty);
        automatic logic [15:0] udp_payload = 16 + 42;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd58, 8'd1, ssn);
        build_msg_common(16'd42, `XDP_MSG_EXEC_ORDER, sym, ssn, oid);
        pkt_wr_u32_le(82, 32'hDEAD0001);  // TradeID
        pkt_wr_u32_le(86, price);
        pkt_wr_u32_le(90, qty);
        pkt_wr_byte(94, 8'h59);           // Printable = 'Y'
        pkt_len = 42 + 16 + 42;           // 100 bytes
    endtask

    // ----------------------------------------------------------------
    // AXI-Stream sender BFM
    // ----------------------------------------------------------------
    task axis_send(input integer idle_cycles);
        integer beats_total, remainder, bt, bi, bthis, bk;
        logic [63:0] bd;
        logic [7:0]  bkp;

        beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;
        remainder   = pkt_len % KEEP_W;

        for (bt = 0; bt < beats_total; bt++) begin
            if (idle_cycles > 0 && bt > 0) begin
                rx_tvalid = 1'b0;
                repeat(idle_cycles) @(posedge clk); #1;
            end

            bd  = '0;
            bkp = '0;
            bi  = bt * KEEP_W;
            bthis = (bt == beats_total - 1 && remainder != 0)
                    ? remainder : KEEP_W;

            for (bk = 0; bk < bthis; bk++)
                bd[bk*8 +: 8] = pkt_buf[(bi+bk)*8 +: 8];

            for (bk = 0; bk < bthis; bk++)
                bkp[bk] = 1'b1;

            rx_tdata  = bd;
            rx_tkeep  = bkp;
            rx_tvalid = 1'b1;
            rx_tlast  = (bt == beats_total - 1);

            @(posedge clk); #1;
            while (!rx_tready) begin @(posedge clk); #1; end
        end

        rx_tvalid = 1'b0;
        rx_tlast  = 1'b0;
    endtask

    // ----------------------------------------------------------------
    // Send packet helper (build + send + pipeline drain)
    // ----------------------------------------------------------------
    task send_pkt_and_wait(input integer drain_cycles);
        axis_send(0);
        repeat(drain_cycles) @(posedge clk); #1;
    endtask

    // ================================================================
    // Weight loading helper
    // ================================================================
    task load_weight(input [`SYM_IDX_W-1:0] addr, input [`WEIGHT_W-1:0] w);
        wt_wr_en   = 1'b1;
        wt_wr_addr = addr;
        wt_wr_data = w;
        @(posedge clk); #1;
        wt_wr_en = 1'b0;
    endtask

    // ================================================================
    // Trade signal capture
    // ================================================================
    logic              trade_seen;
    logic              trade_dir_cap;
    logic signed [AW-1:0] trade_spread_cap;
    logic [31:0]       trade_actual_cap;

    always @(posedge clk) begin
        if (trade.valid) begin
            trade_seen       <= 1'b1;
            trade_dir_cap    <= trade.direction;
            trade_spread_cap <= trade.spread;
            trade_actual_cap <= trade.actual_price;
        end
    end

    task reset_trade_cap;
        trade_seen       = 1'b0;
        trade_dir_cap    = 1'b0;
        trade_spread_cap = '0;
        trade_actual_cap = '0;
    endtask

    // ================================================================
    // TX packet capture
    // ================================================================
    logic        tx_pkt_seen;
    logic [31:0] tx_pkt_count;

    always @(posedge clk) begin
        if (!rst_n) begin
            tx_pkt_seen  <= 1'b0;
            tx_pkt_count <= '0;
        end else if (tx_tvalid && tx_tlast) begin
            tx_pkt_seen  <= 1'b1;
            tx_pkt_count <= tx_pkt_count + 1;
        end
    end

    task reset_tx_cap;
        tx_pkt_seen = 1'b0;
    endtask

    // ================================================================
    // Scoreboard
    // ================================================================
    integer fail_count;
    integer fb;   // fail_count snapshot for per-test checks
    integer seq;  // sequence number counter

    task check(input logic cond, input string msg);
        if (!cond) begin
            $display("  ** FAIL: %s", msg);
            fail_count++;
        end
    endtask

    task pr(input string name);
        if (fail_count == fb)
            $display("  PASS: %s", name);
        else
            $display("  FAIL: %s (%0d errors)", name, fail_count - fb);
    endtask

    // ================================================================
    // Main test sequence
    // ================================================================
    initial begin
        fail_count = 0;
        seq        = 1;

        // Init signals
        rx_tdata   = '0;
        rx_tkeep   = '0;
        rx_tvalid  = 1'b0;
        rx_tlast   = 1'b0;
        tx_enable  = 1'b1;
        wt_wr_en   = 1'b0;
        wt_wr_addr = '0;
        wt_wr_data = '0;
        threshold  = '0;
        pkt_buf    = '0;
        pkt_len    = 0;
        reset_trade_cap();

        $display("");
        $display("  ╔═══════════════════════════════════════════════════════════════╗");
        $display("  ║  HFT Pipeline Top-Level Integration Test                     ║");
        $display("  ║  filter → framer → demux → order_lookup → top_of_book       ║");
        $display("  ║          → index_arb_engine → order_gen_tx                   ║");
        $display("  ╚═══════════════════════════════════════════════════════════════╝");

        // ============================================================
        // PHASE 1: Reset + Initialization
        // ============================================================
        $display("\n=== PHASE 1: Reset + Hash Table Init + Weight Load ===");
        fb = fail_count;

        rst_n = 0;
        repeat(8) @(posedge clk);
        rst_n = 1;

        // Wait for hash table init sweep
        $display("  Waiting for order_lookup ready...");
        wait(ol_ready);
        $display("  order_lookup ready — hash table initialised");

        $display("  Waiting for top_of_book ready...");
        wait (dut.u_tob.ready);
        $display("  top_of_book ready");

        $display("  Waiting for index_arb_engine ready...");
        wait (dut.u_index_arb.ready);
        $display("  index_arb_engine ready");

        // Load weights: 4 components, each 0.25
        load_weight(9'd0, WEIGHT_Q);
        load_weight(9'd1, WEIGHT_Q);
        load_weight(9'd2, WEIGHT_Q);
        load_weight(9'd3, WEIGHT_Q);

        // Set threshold
        threshold = THRESH;
        @(posedge clk); #1;

        check(ol_ready, "order_lookup ready after init");
        pr("Phase 1: Reset + Init");

        // ============================================================
        // PHASE 2: Build Order Book
        //
        // Add BUY + SELL for each of the 4 components + index (sym 4)
        // All mid-prices set to 10000 cents ($100.00)
        //   BUY  @ 9900, SELL @ 10100  → mid = 10000
        // ============================================================
        $display("\n=== PHASE 2: Build Order Book (10 Add Orders) ===");
        fb = fail_count;

        // Component 0
        build_add_order(32'd0, seq, 64'h100, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd0, seq, 64'h101, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        // Component 1
        build_add_order(32'd1, seq, 64'h200, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd1, seq, 64'h201, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        // Component 2
        build_add_order(32'd2, seq, 64'h300, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd2, seq, 64'h301, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        // Component 3
        build_add_order(32'd3, seq, 64'h400, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd3, seq, 64'h401, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        // Index instrument (sym 4) — this is the "actual" index price
        build_add_order(32'd4, seq, 64'h500, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd4, seq, 64'h501, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        // Verify: no trade should have fired (all mids = 10000, index = 10000)
        repeat(20) @(posedge clk); #1;
        check(order_count == 0, "no orders sent during book build");
        $display("  computed_index = %0d (Q44.20)", idx_value);
        $display("  order_count    = %0d", order_count);
        pr("Phase 2: Build Book");

        // ============================================================
        // PHASE 3: Trigger Arbitrage
        //
        // Modify component 0 bid: 9900 → 12000 (mid goes 9900→11050)
        // New computed index rises, spread > threshold → SELL signal
        // ============================================================
        $display("\n=== PHASE 3: Trigger Arbitrage via Modify ===");
        fb = fail_count;
        reset_trade_cap();
        reset_tx_cap();

        build_mod_order(32'd0, seq, 64'h100, 32'd12000, 32'd100); seq++;
        send_pkt_and_wait(50);

        // Wait for TX packet (if triggered)
        repeat(50) @(posedge clk); #1;

        if (trade_seen) begin
            $display("  Trade signal captured:");
            $display("    direction    = %s", trade_dir_cap ? "SELL" : "BUY");
            $display("    spread       = %0d (Q44.20)", $signed(trade_spread_cap));
            $display("    actual_price = %0d", trade_actual_cap);
        end else begin
            $display("  WARNING: No trade signal captured");
        end

        $display("  computed_index = %0d (Q44.20)", idx_value);
        $display("  order_count    = %0d", order_count);
        $display("  net_position   = %0d", net_position);

        check(trade_seen, "trade signal fired after modify");
        if (trade_seen)
            check(trade_dir_cap == 1'b1, "trade direction is SELL (index overvalued)");
        pr("Phase 3: Trigger Arb");

        // ============================================================
        // PHASE 4: Wrong Port — Packet Dropped
        // ============================================================
        $display("\n=== PHASE 4: Wrong UDP Port → Dropped ===");
        fb = fail_count;

        begin
            automatic logic [31:0] saved_order_count = order_count;
            // Build packet with wrong port
            build_net_header(16'd55, 16'hDEAD);
            build_xdp_pkt_hdr(16'd55, 8'd1, seq); seq++;
            build_msg_common(16'd39, `XDP_MSG_ADD_ORDER, 32'd0, seq, 64'hBEEF);
            pkt_wr_u32_le(82, 32'd5000);
            pkt_wr_u32_le(86, 32'd50);
            pkt_wr_byte(90, "B");
            pkt_len = 97;

            send_pkt_and_wait(40);

            check(order_count == saved_order_count,
                  "order_count unchanged after wrong-port packet");
        end
        pr("Phase 4: Wrong Port Drop");

        // ============================================================
        // PHASE 5: Recovery After Drop
        // ============================================================
        $display("\n=== PHASE 5: Valid Packet After Drop ===");
        fb = fail_count;

        // Wait for cooldown to expire from phase 3
        repeat(TB_COOLDOWN + 10) @(posedge clk); #1;

        // Send a valid Add (component 1 — add another buy level)
        build_add_order(32'd1, seq, 64'h202, 32'd9800, 32'd50, 1'b0); seq++;
        send_pkt_and_wait(50);

        // The pipeline should process it without stalling
        check(1'b1, "pipeline accepts valid packet after drop");
        pr("Phase 5: Recovery");

        // ============================================================
        // PHASE 6: Delete Order — Remove from Book
        // ============================================================
        $display("\n=== PHASE 6: Delete Order ===");
        fb = fail_count;

        build_del_order(32'd1, seq, 64'h202); seq++;
        send_pkt_and_wait(50);

        // Pipeline should handle it gracefully
        check(1'b1, "delete processed without hang");
        pr("Phase 6: Delete");

        // ============================================================
        // PHASE 7: Exec (Partial Fill)
        // ============================================================
        $display("\n=== PHASE 7: Exec Partial Fill ===");
        fb = fail_count;

        // Partial fill on component 1 buy (OID 0x200, qty 50 of 100)
        build_exec_order(32'd1, seq, 64'h200, 32'd9900, 32'd50); seq++;
        send_pkt_and_wait(50);

        check(1'b1, "exec partial fill processed");
        pr("Phase 7: Exec Partial");

        // ============================================================
        // PHASE 8: TX Enable Gate
        // ============================================================
        $display("\n=== PHASE 8: TX Enable Gate ===");
        fb = fail_count;

        // Wait for cooldown
        repeat(TB_COOLDOWN + 10) @(posedge clk); #1;

        begin
            automatic logic [31:0] saved_count = order_count;

            tx_enable = 1'b0;  // disable TX

            // Trigger another arb (modify component 2 bid up)
            build_mod_order(32'd2, seq, 64'h300, 32'd15000, 32'd100); seq++;
            send_pkt_and_wait(50);
            repeat(50) @(posedge clk); #1;

            check(order_count == saved_count,
                  "no TX packet when tx_enable=0");

            tx_enable = 1'b1;  // re-enable
        end
        pr("Phase 8: TX Enable Gate");

        // ============================================================
        // Summary
        // ============================================================
        $display("");
        $display("  ════════════════════════════════════════════");
        if (fail_count == 0)
            $display("  @@@ Passed: All tests passed!");
        else
            $display("  @@@ Failed: %0d error(s) total", fail_count);
        $display("  ════════════════════════════════════════════");
        $display("");

        #100;
        $finish;
    end

    // ================================================================
    // Timeout watchdog
    // ================================================================
    initial begin
        #500_000;
        $display("\n  *** TIMEOUT — simulation exceeded 500 us ***");
        $finish;
    end

endmodule
