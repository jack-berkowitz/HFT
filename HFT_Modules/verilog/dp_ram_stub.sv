module dp_ram #(
    parameter ADDR_W  = 12,
    parameter DATA_W  = 130,
    parameter DEPTH   = 4096
)(
    input  logic                clk,
    input  logic [ADDR_W-1:0]  a_addr,
    output logic [DATA_W-1:0]  a_rdata,
    input  logic [ADDR_W-1:0]  b_addr,
    input  logic [DATA_W-1:0]  b_wdata,
    input  logic               b_wen,
    output logic [DATA_W-1:0]  b_rdata
);
endmodule
