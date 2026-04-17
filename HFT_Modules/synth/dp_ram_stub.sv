// Empty synthesis stub for dp_ram.  Swapped in during dc_shell compilation
// (see *_SYN_SRCS in the Makefile) so DC leaves dp_ram as an unresolved
// black box instead of inferring thousands of flops for BRAM arrays.
// The behavioral verilog/dp_ram.sv is re-linked for post-synth simulation
// via *_SYN_SIM_DEPS.
module dp_ram #(
    parameter ADDR_W  = 12,
    parameter DATA_W  = 130,
    parameter DEPTH   = 4096
)(
    input  logic                clk,
    input  logic                rst_n,
    input  logic [ADDR_W-1:0]  a_addr,
    output logic [DATA_W-1:0]  a_rdata,
    input  logic [ADDR_W-1:0]  b_addr,
    input  logic [DATA_W-1:0]  b_wdata,
    input  logic               b_wen,
    output logic [DATA_W-1:0]  b_rdata
);
endmodule
