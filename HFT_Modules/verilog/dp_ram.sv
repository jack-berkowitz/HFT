module dp_ram #(
    parameter ADDR_W  = 12,
    parameter DATA_W  = 130,
    parameter DEPTH   = 4096
)(
    input  logic                clk,
    // Port A — read only
    input  logic [ADDR_W-1:0]  a_addr,
    output logic [DATA_W-1:0]  a_rdata,
    // Port B — read/write
    input  logic [ADDR_W-1:0]  b_addr,
    input  logic [DATA_W-1:0]  b_wdata,
    input  logic               b_wen,
    output logic [DATA_W-1:0]  b_rdata
);
    logic [DATA_W-1:0] mem [0:DEPTH-1];

    always_ff @(posedge clk) begin
        a_rdata <= mem[a_addr];
    end

    always_ff @(posedge clk) begin
        if (b_wen)
            mem[b_addr] <= b_wdata;
        b_rdata <= mem[b_addr];
    end
endmodule