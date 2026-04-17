// Behavioral dual-port RAM used for both RTL sim and post-synth sim.
//
// Synopsys DC will happily infer thousands of flops for wide memory arrays
// unless the cell is stubbed during synthesis; we therefore replace this
// file with synth/dp_ram_stub.sv for synthesis (via the per-module
// *_SYN_SRCS list in the Makefile) and re-link this behavioral model for
// post-synth simulation (via *_SYN_SIM_DEPS).
//
// X-prop hardening:
//   VCS's +xprop=tmerge is far more pessimistic than iverilog about X
//   propagation. The registered outputs `a_rdata` and `b_rdata` hold X
//   for an indeterminate number of cycles after reset until they get a
//   first write to their read address — long enough for the X to leak
//   into downstream pipelines before the upstream `ready` gate takes
//   effect.  Reset them to zero synchronously so the RAM never emits X.
//   (mem[] itself is still X-valued, but all paths into mem[] are gated
//   by caller-level S_CLEAR state machines that walk every address and
//   write zero before raising `ready`.)

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
    logic [DATA_W-1:0] mem [0:DEPTH-1];

    // Initialize mem[] to zero at t=0 so that reads issued during the first
    // few cycles after reset (before caller-level S_CLEAR has walked every
    // address) return a defined value rather than X.  This runs only in
    // simulation — dp_ram is stubbed out during synthesis — so there is no
    // synthesis implication from the initial block.
    //
    // NOTE: the `mem`-writing block below uses plain `always` rather than
    // `always_ff` because VCS flags ICPD ("Illegal combination of
    // procedural drivers") when a variable is written from both an
    // `always_ff` block and an `initial` block.  Plain `always` at a
    // posedge is still inferred as a flop by DC.
    initial begin
        for (int i = 0; i < DEPTH; i++)
            mem[i] = '0;
    end

    always_ff @(posedge clk) begin
        if (!rst_n)
            a_rdata <= '0;
        else
            a_rdata <= mem[a_addr];
    end

    always @(posedge clk) begin
        if (!rst_n) begin
            b_rdata <= '0;
        end else begin
            if (b_wen)
                mem[b_addr] <= b_wdata;
            b_rdata <= mem[b_addr];
        end
    end
endmodule
