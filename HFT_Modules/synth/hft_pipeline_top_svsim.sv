`ifndef SYNTHESIS

//
// This is an automatically generated file from 
// dc_shell Version V-2023.12-SP5 -- Jul 16, 2024
//

// For simulation only. Do not modify.

module hft_pipeline_top_svsim #(
    parameter [15:0] FILTER_PORT    = 16'd11101,

    parameter        HEADER_BYTES   = 10,

    parameter        HT_N_WAYS      = 4,
    parameter        HT_TBL_ENTRIES = 4096,
    parameter        HT_MAX_CHAIN   = 16,

    parameter        TOB_N_LEVELS   = 3,
    parameter        TOB_N_SYMBOLS  = 500,

    parameter        N_COMPONENTS   = 500,
    parameter [8:0]  IDX_SYM        = 9'd500,

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
    parameter [31:0] IDX_SYM_ID     = 9'd500
)(
    input  logic              clk,
    input  logic              rst_n,

    input  AXI_TDATA          rx_axis_tdata,
    input  AXI_TKEEP          rx_axis_tkeep,
    input  logic              rx_axis_tvalid,
    input  logic              rx_axis_tlast,
    output logic              rx_axis_tready,

    output AXI_TDATA          tx_axis_tdata,
    output AXI_TKEEP          tx_axis_tkeep,
    output logic              tx_axis_tvalid,
    output logic              tx_axis_tlast,
    input  logic              tx_axis_tready,

    input  logic              tx_enable,

    input  logic                      wt_wr_en,
    input  logic [9-1:0]    wt_wr_addr,
    input  logic [32-1:0]     wt_wr_data,

    input  logic [64-1:0]      threshold,

    output logic              ol_ready,

    output tob_out_t          tob_snapshot,

    output trade_signal_t     trade_signal,

    output logic [64-1:0] computed_index,

    output logic [31:0]       order_count,
    output logic signed [31:0] net_position,
    output logic              pkt_sent
);

    

  hft_pipeline_top hft_pipeline_top( {>>{ clk }}, {>>{ rst_n }}, 
        {>>{ rx_axis_tdata }}, {>>{ rx_axis_tkeep }}, {>>{ rx_axis_tvalid }}, 
        {>>{ rx_axis_tlast }}, {>>{ rx_axis_tready }}, {>>{ tx_axis_tdata }}, 
        {>>{ tx_axis_tkeep }}, {>>{ tx_axis_tvalid }}, {>>{ tx_axis_tlast }}, 
        {>>{ tx_axis_tready }}, {>>{ tx_enable }}, {>>{ wt_wr_en }}, 
        {>>{ wt_wr_addr }}, {>>{ wt_wr_data }}, {>>{ threshold }}, 
        {>>{ ol_ready }}, {>>{ tob_snapshot }}, {>>{ trade_signal }}, 
        {>>{ computed_index }}, {>>{ order_count }}, {>>{ net_position }}, 
        {>>{ pkt_sent }} );
endmodule
`endif
