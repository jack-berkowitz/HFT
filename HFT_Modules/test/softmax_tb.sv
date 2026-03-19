`timescale 1ns / 1ps

// ============================================================
//  softmax_tb.sv  — Improved Softmax Testbench (v4)
//
//  State encoding (must match softmax.sv):
//    ADD_LOAD    = 3'b011 (= S_ADD for display only)
//    DIV         = 3'b100
//    OUTPUTSTREAM= 3'b101  ← we collect outputs here
//    ADD_SUBMIT  = 3'b110
//    ADD_WAIT    = 3'b111
// ============================================================

`define S_IDLE         3'b000
`define S_INPUTSTREAM  3'b001
`define S_EXP          3'b010
`define S_ADD_LOAD     3'b011
`define S_DIV          3'b100
`define S_OUTPUTSTREAM 3'b101   // must match softmax.sv OUTPUTSTREAM
`define S_ADD_SUBMIT   3'b110
`define S_ADD_WAIT     3'b111

`define DATALENGTH  32
`define INPUTMAX     2

module softmax_tb;

  logic                    Clock;
  logic                    Reset;
  logic                    Start;
  logic [`DATALENGTH-1:0]  Datain;
  logic [`INPUTMAX:0]      N;
  logic [`DATALENGTH-1:0]  Dataout;

  softmax DUT (
    .Clock   (Clock),
    .Reset   (Reset),
    .Start   (Start),
    .Datain  (Datain),
    .N       (N),
    .Dataout (Dataout)
  );

  initial  Clock = 1'b1;
  always #1 Clock = ~Clock;

  int tests_passed = 0;
  int tests_failed = 0;

  parameter real TOL      = 0.01;
  parameter int  MAX_COLL = 8;
  parameter int  TIMEOUT  = 15000;

  // ── State name helper ─────────────────────────────────────────────────
  function automatic string state_name(input logic [2:0] s);
    case (s)
      3'b000: return "IDLE";
      3'b001: return "INPUTSTREAM";
      3'b010: return "EXP";
      3'b011: return "ADD_LOAD";
      3'b100: return "DIV";
      3'b101: return "OUTPUTSTREAM";
      3'b110: return "ADD_SUBMIT";
      3'b111: return "ADD_WAIT";
      default: return "UNKNOWN";
    endcase
  endfunction

  // ── FP helpers ────────────────────────────────────────────────────────
  function automatic real fp32(input logic [31:0] b);
    return $bitstoshortreal(b);
  endfunction

  function automatic logic fp_ok(input real got, exp, tol);
    real diff, ref_v;
    diff  = (got > exp) ? (got - exp) : (exp - got);
    ref_v = (exp < 0.0) ? -exp : exp;
    if (ref_v < 1e-9) return diff <= tol;
    return (diff / ref_v) <= tol;
  endfunction

  // ── State-change monitor ──────────────────────────────────────────────
  logic [2:0] prev_state;
  always @(posedge Clock) begin
    if (DUT.NextState !== prev_state) begin
      $display("  [DBG @%0t] State: %s → %s",
               $time, state_name(prev_state), state_name(DUT.NextState));
      prev_state <= DUT.NextState;
    end
  end

  // ── ADD-phase cycle watcher ───────────────────────────────────────────
  // Prints one line per Ack_add3 pulse to track each summation.
  always @(posedge Clock) begin
    if (DUT.Ack_add3) begin
      $display("  [ADD @%0t] Ack_add3 pulse: C=%0d Acc_w=0x%08h Acc(pre)=0x%08h",
               $time, DUT.C, DUT.Acc_w, DUT.Acc);
    end
  end

  // ── Reset task ────────────────────────────────────────────────────────
  task do_reset();
    Reset  = 1'b1;
    Start  = 1'b0;
    Datain = '0;
    N      = '0;
    repeat(6) @(posedge Clock);
    Reset  = 1'b0;
    @(posedge Clock);
  endtask

  // ── Main test task ────────────────────────────────────────────────────
  task automatic run_test(
    input string         name,
    input int            n_in,
    input logic [31:0]   in_bits [MAX_COLL],
    input real           exp_vals[MAX_COLL]
  );
    logic [31:0] collected[MAX_COLL];
    int          n_collected, timeout, exp_idx;
    logic        test_ok;
    real         got_v, exp_v;
    string       res_str;

    $display("");
    $display("==========================================================");
    $display("  TEST: %s", name);
    $display("==========================================================");
    $display("  Inputs (%0d):", n_in);
    for (int i = 0; i < n_in; i++)
      $display("    [%0d]  0x%08h  =  %f", i, in_bits[i], fp32(in_bits[i]));
    $display("  Expected outputs:");
    for (int i = 0; i < n_in; i++)
      $display("    [%0d]  %f", i, exp_vals[i]);

    // ── Stream inputs ────────────────────────────────────────────────
    N = n_in - 1;
    @(posedge Clock);
    Start = 1'b1;
    for (int i = 0; i < n_in; i++) begin
      Datain = in_bits[i];
      @(posedge Clock);
    end
    Start  = 1'b0;
    Datain = '0;

    // ── Wait for DIV state to confirm ADD completed ───────────────────
    timeout = TIMEOUT;
    while (DUT.NextState !== `S_DIV && timeout > 0) begin
      @(posedge Clock);
      timeout--;
    end
    if (timeout > 0) begin
      $display("  [DBG] ADD complete → DIV. Acc=0x%08h=%f",
               DUT.Acc, fp32(DUT.Acc));
      $display("  [DBG] DivBuffer: [0]=0x%08h [1]=0x%08h [2]=0x%08h [3]=0x%08h",
               DUT.DivBuffer[0], DUT.DivBuffer[1],
               DUT.DivBuffer[2], DUT.DivBuffer[3]);
    end
    else begin
      $display("  [TIMEOUT waiting for DIV]");
      $display("  [DBG] State=%s  C=%0d  Acc=0x%08h  Ack_add3=%0b",
               state_name(DUT.NextState), DUT.C, DUT.Acc, DUT.Ack_add3);
      $display("  [DBG] Str_Add_a=%0b Str_Add_b=%0b Str_Add_z=%0b",
               DUT.Str_Add_a, DUT.Str_Add_b, DUT.Str_Add_z);
      $display("  [DBG] DivBuffer: [0]=0x%08h [1]=0x%08h [2]=0x%08h [3]=0x%08h",
               DUT.DivBuffer[0], DUT.DivBuffer[1],
               DUT.DivBuffer[2], DUT.DivBuffer[3]);
      $display("  @@@ Failed: %s", name);
      tests_failed++;
      do_reset();
      return;
    end

    // ── Wait for OUTPUTSTREAM ─────────────────────────────────────────
    timeout = TIMEOUT;
    while (DUT.NextState !== `S_OUTPUTSTREAM && timeout > 0) begin
      @(posedge Clock);
      timeout--;
    end
    if (timeout == 0) begin
      $display("  [TIMEOUT waiting for OUTPUTSTREAM]");
      $display("  [DBG] State=%s  div_done=0x%01h",
               state_name(DUT.NextState), DUT.div_done);
      $display("  [DBG] OutputBuffer: [0]=0x%08h [1]=0x%08h [2]=0x%08h [3]=0x%08h",
               DUT.OutputBuffer[0], DUT.OutputBuffer[1],
               DUT.OutputBuffer[2], DUT.OutputBuffer[3]);
      $display("  @@@ Failed: %s", name);
      tests_failed++;
      do_reset();
      return;
    end

    // ── Collect outputs ───────────────────────────────────────────────
    n_collected = 0;
    while (DUT.NextState == `S_OUTPUTSTREAM && n_collected < MAX_COLL) begin
      @(posedge Clock);
      collected[n_collected] = Dataout;
      n_collected++;
    end

    $display("  Collected %0d raw outputs (DUT streams high index first):", n_collected);
    for (int i = 0; i < n_collected; i++)
      $display("    collected[%0d]  0x%08h  =  %f",
               i, collected[i], fp32(collected[i]));

    // ── Compare ───────────────────────────────────────────────────────
    $display("  Comparison (tol = %.1f%%):", TOL * 100.0);
    $display("  %-5s  %-5s  %-12s  %-12s  %-6s",
             "col_i", "exp_i", "expected", "got", "result");

    test_ok = 1'b1;
    for (int j = 0; j < n_in; j++) begin
      exp_idx = n_in - 1 - j;
      exp_v   = exp_vals[exp_idx];
      if (j >= n_collected) begin
        $display("  [%0d]   [%0d]   %10.6f   (missing)   FAIL", j, exp_idx, exp_v);
        test_ok = 1'b0;
      end else begin
        got_v   = fp32(collected[j]);
        res_str = fp_ok(got_v, exp_v, TOL) ? "PASS" : "FAIL";
        if (res_str == "FAIL") test_ok = 1'b0;
        $display("  [%0d]   [%0d]   %10.6f   %10.6f   %s",
                 j, exp_idx, exp_v, got_v, res_str);
      end
    end

    if (test_ok) begin
      $display("  @@@ Passed: %s", name);
      tests_passed++;
    end else begin
      $display("  @@@ Failed: %s", name);
      tests_failed++;
    end

    do_reset();
    #4;
  endtask

  // ── Test vectors ──────────────────────────────────────────────────────
  initial begin
    $dumpfile("softmax_tb.vcd");
    $dumpvars(0, softmax_tb);
    prev_state = 3'bx;

    do_reset();
    #4;

    // Test 1: Uniform [1, 1, 1, 1] → all 0.25
    begin
      logic [31:0] ib[MAX_COLL];
      real         ev[MAX_COLL];
      ib = '{32'h3f800000, 32'h3f800000, 32'h3f800000, 32'h3f800000,
             '0,'0,'0,'0};
      ev = '{0.25, 0.25, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0};
      run_test("Uniform [1, 1, 1, 1]  ->  all 0.25", 4, ib, ev);
    end

    // Test 2: Ascending [1, 2, 3, 4]
    begin
      logic [31:0] ib[MAX_COLL];
      real         ev[MAX_COLL];
      ib = '{32'h3f800000, 32'h40000000, 32'h40400000, 32'h40800000,
             '0,'0,'0,'0};
      ev = '{0.032058, 0.087144, 0.236882, 0.641917, 0.0, 0.0, 0.0, 0.0};
      run_test("Ascending [1, 2, 3, 4]", 4, ib, ev);
    end

    // Test 3: Mostly zeros [0, 0, 0, 1]
    begin
      logic [31:0] ib[MAX_COLL];
      real         ev[MAX_COLL];
      ib = '{32'h00000000, 32'h00000000, 32'h00000000, 32'h3f800000,
             '0,'0,'0,'0};
      ev = '{0.174866, 0.174866, 0.174866, 0.475402, 0.0, 0.0, 0.0, 0.0};
      run_test("Mostly zeros [0, 0, 0, 1]", 4, ib, ev);
    end

    // Test 4: Mixed negatives [-2, 0, 2, 4]
    begin
      logic [31:0] ib[MAX_COLL];
      real         ev[MAX_COLL];
      ib = '{32'hc0000000, 32'h00000000, 32'h40000000, 32'h40800000,
             '0,'0,'0,'0};
      ev = '{0.002145, 0.015840, 0.117019, 0.864996, 0.0, 0.0, 0.0, 0.0};
      run_test("Mixed negatives [-2, 0, 2, 4]", 4, ib, ev);
    end

    // Test 5: Large logit [0, 0, 0, 10]
    begin
      logic [31:0] ib[MAX_COLL];
      real         ev[MAX_COLL];
      ib = '{32'h00000000, 32'h00000000, 32'h00000000, 32'h41200000,
             '0,'0,'0,'0};
      ev = '{0.0000454, 0.0000454, 0.0000454, 0.9998638, 0.0, 0.0, 0.0, 0.0};
      run_test("Large logit [0, 0, 0, 10]", 4, ib, ev);
    end

    // ── Summary ──────────────────────────────────────────────────────────
    $display("");
    $display("==========================================================");
    $display("  FINAL RESULTS");
    $display("  Passed: %0d  |  Failed: %0d  |  Total: %0d",
             tests_passed, tests_failed, tests_passed + tests_failed);
    $display("==========================================================");
    if (tests_failed == 0)
      $display("@@@ Passed");
    else
      $display("@@@ Failed");

    $finish;
  end

endmodule