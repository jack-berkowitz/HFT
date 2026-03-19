`timescale 1ns / 1ps
`define DATALENGTH 32
`define INPUTMAX 2

// ─── 3-bit flat FSM states ─────────────────────────────────────────────────
// MUST keep as 3-bit so OUTPUTSTREAM = 3'b101 matches the testbench probe.
//
//  ADD_LOAD    3'b011  (testbench sees this as state 3 "ADD")
//  DIV         3'b100
//  OUTPUTSTREAM 3'b101  ← testbench probes for this exact value
//  ADD_SUBMIT  3'b110  (7 ≠ 5, no testbench collision)
//  ADD_WAIT    3'b111  (7 ≠ 5, no testbench collision)
//
// The original "ADD" (3'b011) is now ADD_LOAD; ADD_SUBMIT and ADD_WAIT are
// additional states added to safely pipeline the adder handshake without
// any nested case statements (Bug 8).

`define IDLE         3'b000
`define INPUTSTREAM  3'b001
`define EXP          3'b010
`define ADD_LOAD     3'b011
`define DIV          3'b100
`define OUTPUTSTREAM 3'b101
`define ADD_SUBMIT   3'b110
`define ADD_WAIT     3'b111

// =============================================================
//  Bug fix summary
//
//  Bug 1  ADD OOB: C < 2**INPUTMAX+1 → C <= N
//  Bug 2  Str_Add_a timing: ADD_LOAD deasserts, ADD_SUBMIT
//         asserts after Arg is stable
//  Bug 3  ADD C_add level-count: use Ack_add3 directly
//  Bug 4  DIV OOB: m < N → m <= N
//  Bug 5  OUTPUTSTREAM counter: Counter<=N before entering;
//         emit-then-decrement
//  Bug 6  DIV simultaneous completion: per-divider div_done
//         latch; transition on all done
//  Bug 7  adder.sv negedge rst: changed to posedge rst
//  Bug 8  Nested case VCS malfunction: now use flat 3-bit FSM
//         states — zero nested case statements
//  Bug 9  4-bit state collision with testbench: 3-bit encoding
//         makes ADD_WAIT = 3'b111 = 7 ≠ S_OUTPUTSTREAM = 5
// =============================================================

module softmax (
    input  logic                    Clock,
    input  logic                    Reset,
    input  logic                    Start,
    input  logic [`DATALENGTH-1:0]  Datain,
    input  logic [`INPUTMAX:0]      N,
    output logic [`DATALENGTH-1:0]  Dataout
);

    integer m;

    // ─── Buffers ──────────────────────────────────────────────────────────
    logic [`DATALENGTH-1:0] InputBuffer   [2**`INPUTMAX - 1 : 0];
    logic [`DATALENGTH-1:0] DivBuffer     [2**`INPUTMAX - 1 : 0];
    logic [`DATALENGTH-1:0] OutputBuffer  [2**`INPUTMAX - 1 : 0];

    // ─── Accumulator ──────────────────────────────────────────────────────
    logic [`DATALENGTH-1:0] Acc;
    logic [`DATALENGTH-1:0] Arg;
    logic [`DATALENGTH-1:0] Acc_w;   // combinationally connected to adder output_z

    // ─── Exponential handshake ────────────────────────────────────────────
    logic [3:0]             Str;
    logic [`DATALENGTH-1:0] InputBuffer_w [2**`INPUTMAX - 1 : 0];
    logic [3:0]             Ack;

    // ─── Adder handshake ──────────────────────────────────────────────────
    logic Str_Add_a, Str_Add_b, Str_Add_z;
    logic Ack_add1,  Ack_add2,  Ack_add3;

    // ─── Divider handshake ────────────────────────────────────────────────
    logic        Ack_div1, Ack_div2, Ack_div3;
    logic [3:0]  Ack_div_a, Ack_div_b, Ack_div_z;
    logic [`DATALENGTH-1:0] OutputBuffer_w [2**`INPUTMAX - 1 : 0];

    // ─── FSM state + counters ─────────────────────────────────────────────
    logic [2:0]          NextState;
    logic [`INPUTMAX:0]  Counter;
    logic [`INPUTMAX:0]  C;

    // ─── Per-divider completion latch ─────────────────────────────────────
    logic [3:0] div_done;

    // ─── Module-level combinational: all required dividers done? ──────────
    // Compute outside the always block to avoid local-variable issues in VCS.
    // For N+1 dividers (indices 0..N), check each bit conditionally.
    // INPUTMAX=2 so N is at most 3 — all four slots are always checked.
    logic all_div_done;
    always_comb begin
        all_div_done = div_done[0]
                    && (N < 1 || div_done[1])
                    && (N < 2 || div_done[2])
                    && (N < 3 || div_done[3]);
    end

    // ─── Exponential units ────────────────────────────────────────────────
    genvar i;
    generate
        for (i = 0; i <= 2**`INPUTMAX - 1; i = i + 1) begin : gen_exp
            exponential exp (
                .Clock   (Clock),
                .Reset   (Reset),
                .Str     (Str[i]),
                .Datain  (InputBuffer[i]),
                .Ack     (Ack[i]),
                .DataOut (InputBuffer_w[i])
            );
        end
    endgenerate

    // ─── Adder (iterative sum) ────────────────────────────────────────────
    adder add (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (Arg),
        .input_a_stb  (Str_Add_a),
        .input_a_ack  (Ack_add1),
        .input_b      (Acc),
        .input_b_stb  (Str_Add_b),
        .input_b_ack  (Ack_add2),
        .output_z     (Acc_w),
        .output_z_stb (Ack_add3),
        .output_z_ack (Str_Add_z)
    );

    // ─── Dividers (parallel, one per slot) ───────────────────────────────
    genvar k;
    generate
        for (k = 0; k <= 2**`INPUTMAX - 1; k = k + 1) begin : gen_div
            divider div (
                .clk          (Clock),
                .rst          (Reset),
                .input_a      (DivBuffer[k]),
                .input_a_stb  (Ack_div1),
                .input_a_ack  (Ack_div_a[k]),
                .input_b      (Acc),
                .input_b_stb  (Ack_div2),
                .input_b_ack  (Ack_div_b[k]),
                .output_z     (OutputBuffer_w[k]),
                .output_z_stb (Ack_div_z[k]),
                .output_z_ack (Ack_div3)
            );
        end
    endgenerate

    // ─── Main sequential FSM ─────────────────────────────────────────────
    // IMPORTANT: no nested case statements anywhere in this block.
    always @(posedge Clock or posedge Reset) begin
        if (Reset) begin
            for (m = 0; m <= 2**`INPUTMAX - 1; m = m + 1) begin
                InputBuffer[m]  <= 0;
                OutputBuffer[m] <= 0;
                DivBuffer[m]    <= 0;
            end
            Counter   <= 0;
            C         <= 0;
            Arg       <= 0;
            Dataout   <= 0;
            Acc       <= 0;
            Str       <= 0;
            Ack_div1  <= 0;
            Ack_div2  <= 0;
            Ack_div3  <= 0;
            Str_Add_a <= 0;
            Str_Add_b <= 0;
            Str_Add_z <= 0;
            div_done  <= 0;
            NextState <= `IDLE;
        end
        else begin
            case (NextState)

                // ── IDLE ──────────────────────────────────────────────────
                `IDLE: begin
                    if (Start) NextState <= `INPUTSTREAM;
                    else       NextState <= `IDLE;
                end

                // ── INPUTSTREAM ───────────────────────────────────────────
                `INPUTSTREAM: begin
                    if (Counter <= N) begin
                        InputBuffer[Counter] <= Datain;
                        Counter   <= Counter + 1;
                        NextState <= `INPUTSTREAM;
                    end
                    else begin
                        // Launch exponential units and pre-enable adder ack.
                        Str       <= 4'b1111;
                        Str_Add_z <= 1;
                        // Reset ADD state
                        C         <= 0;
                        Acc       <= 0;
                        NextState <= `EXP;
                    end
                end

                // ── EXP ───────────────────────────────────────────────────
                `EXP: begin
                    if (Ack == 4'b1111) begin
                        for (m = 0; m <= N; m = m + 1)
                            DivBuffer[m] <= InputBuffer_w[m];
                        NextState <= `ADD_LOAD;
                    end
                    else begin
                        NextState <= `EXP;
                    end
                end

                // ── ADD_LOAD ──────────────────────────────────────────────
                // Load DivBuffer[C] into Arg.  Deassert Str_Add_a/b so the
                // adder (which just returned to get_a from put_z) does NOT
                // sample the bus before Arg has settled.  The adder's
                // s_input_a_ack becomes 1 on the NEXT cycle (get_a first
                // cycle sets ack←1 via NBA); we assert stb in ADD_SUBMIT
                // so the capture happens correctly in ADD_WAIT.
                `ADD_LOAD: begin
                    Arg       <= DivBuffer[C];
                    Str_Add_a <= 0;
                    Str_Add_b <= 0;
                    NextState <= `ADD_SUBMIT;
                end

                // ── ADD_SUBMIT ────────────────────────────────────────────
                // Arg is now stable (DivBuffer[C] was loaded last cycle).
                // Assert Str_Add_a/b.  The adder is in get_a with ack=1
                // so it will capture input_a = Arg on the NEXT posedge
                // (= the first cycle of ADD_WAIT).
                `ADD_SUBMIT: begin
                    Str_Add_a <= 1;
                    Str_Add_b <= 1;
                    NextState <= `ADD_WAIT;
                end

                // ── ADD_WAIT ──────────────────────────────────────────────
                // Wait for Ack_add3 (= adder output_z_stb).
                // This is a single-cycle pulse: put_z sets s_output_z_stb←1
                // on cycle T, and on cycle T+1 the last-NBA-wins rule fires
                // both s_output_z_stb←1 and (condition true) ←0 together,
                // net result = 0.  The stb seen by combinational output_z_stb
                // is 1 only during cycle T+1.
                `ADD_WAIT: begin
                    if (Ack_add3) begin
                        Acc       <= Acc_w;
                        Str_Add_a <= 0;
                        Str_Add_b <= 0;
                        if (C == N) begin
                            // Sum complete — launch all dividers.
                            div_done  <= 0;
                            Ack_div3  <= 1;
                            NextState <= `DIV;
                        end
                        else begin
                            C         <= C + 1;
                            NextState <= `ADD_LOAD;
                        end
                    end
                    else begin
                        NextState <= `ADD_WAIT;
                    end
                end

                // ── DIV ───────────────────────────────────────────────────
                // All dividers run in parallel computing DivBuffer[k] / Acc.
                // Each fires Ack_div_z[k] for exactly one cycle when done.
                // Unrolled individual captures avoid loop + local-var issues.
                `DIV: begin
                    Ack_div1 <= 1;
                    Ack_div2 <= 1;

                    if (Ack_div_z[0] && !div_done[0]) begin
                        OutputBuffer[0] <= OutputBuffer_w[0];
                        div_done[0]     <= 1;
                    end
                    if (Ack_div_z[1] && !div_done[1]) begin
                        OutputBuffer[1] <= OutputBuffer_w[1];
                        div_done[1]     <= 1;
                    end
                    if (Ack_div_z[2] && !div_done[2]) begin
                        OutputBuffer[2] <= OutputBuffer_w[2];
                        div_done[2]     <= 1;
                    end
                    if (Ack_div_z[3] && !div_done[3]) begin
                        OutputBuffer[3] <= OutputBuffer_w[3];
                        div_done[3]     <= 1;
                    end

                    // all_div_done is a module-level wire (computed above)
                    if (all_div_done) begin
                        Counter   <= N;
                        NextState <= `OUTPUTSTREAM;
                    end
                    else begin
                        NextState <= `DIV;
                    end
                end

                // ── OUTPUTSTREAM ──────────────────────────────────────────
                // Emit OutputBuffer[N..0] high-index-first.
                // Bug 5: emit current Counter index THEN decrement so that
                // OutputBuffer[0] is emitted before wrapping to IDLE.
                `OUTPUTSTREAM: begin
                    Dataout <= OutputBuffer[Counter];
                    if (Counter == 0) begin
                        Str       <= 0;
                        Ack_div1  <= 0;
                        Ack_div2  <= 0;
                        Ack_div3  <= 0;
                        Str_Add_z <= 0;
                        Acc       <= 0;
                        C         <= 0;
                        Counter   <= 0;
                        div_done  <= 0;
                        NextState <= `IDLE;
                    end
                    else begin
                        Counter   <= Counter - 1;
                        NextState <= `OUTPUTSTREAM;
                    end
                end

                default: NextState <= `IDLE;

            endcase
        end
    end

endmodule