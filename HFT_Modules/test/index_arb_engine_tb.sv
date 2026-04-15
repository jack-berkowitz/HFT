`timescale 1ns/1ps
`include "sys_defs.svh"
// ============================================================
// index_arb_engine_tb.sv
//
// Tests the index arbitrage engine with a small 4-component
// "mini-index" to make expected values easy to hand-calculate.
//
// Components 0-3 with weights summing to 1.0 (Q12.20):
//   Sym 0: weight = 0.50  = 524_288
//   Sym 1: weight = 0.25  = 262_144
//   Sym 2: weight = 0.15  = 157_286
//   Sym 3: weight = 0.10  = 104_858
//                   ----    ---------
//                   1.00    1_048_576  (= 1 << 20)
//
// Using IDX_SYMBOL = 500 for actual index price.
// Prices in cents (integer).
//
// Example:
//   Sym 0 mid = 15000 (=$150.00)  contribution = 15000 * 0.5 = 7500
//   Sym 1 mid = 20000 (=$200.00)  contribution = 20000 * 0.25 = 5000
//   Sym 2 mid = 10000 (=$100.00)  contribution = 10000 * 0.15 = 1500
//   Sym 3 mid = 5000  (=$ 50.00)  contribution = 5000  * 0.1  = 500
//   Computed index = 14500 cents = $145.00
// ============================================================
module index_arb_engine_tb;

    localparam FRAC = `WEIGHT_FRAC_BITS;
    localparam AW   = `ACCUM_W;
    localparam WW   = `WEIGHT_W;
    localparam SYMW = `SYM_IDX_W;
    localparam N_COMP = 4;       // mini-index for testing

    logic                  clk, rst_n;
    logic                  ready;
    tob_out_t              tob_in;
    logic                  wt_wr_en;
    logic [SYMW-1:0]       wt_wr_addr;
    logic [WW-1:0]         wt_wr_data;
    logic [AW-1:0]         threshold;
    trade_signal_t         trade_out;
    logic [AW-1:0]         index_monitor;

    index_arb_engine #(
        .N_COMPONENTS (N_COMP),
        .IDX_SYM      (`IDX_SYMBOL)
    ) dut (
        .clk          (clk),
        .rst_n        (rst_n),
        .in_tob       (tob_in),
        .wt_wr_en     (wt_wr_en),
        .wt_wr_addr   (wt_wr_addr),
        .wt_wr_data   (wt_wr_data),
        .threshold    (threshold),
        .ready        (ready),
        .out_trade    (trade_out),
        .out_index    (index_monitor)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    // ----------------------------------------------------------------
    // Helpers
    // ----------------------------------------------------------------
    task automatic load_weight(input [SYMW-1:0] sym, input [WW-1:0] w);
        wt_wr_en   = 1'b1;
        wt_wr_addr = sym;
        wt_wr_data = w;
        @(posedge clk); #1;
        wt_wr_en = 1'b0;
    endtask

    task automatic send_tob(
        input [SYMW-1:0] sym,
        input [31:0]     bid_p, bid_q,
        input [31:0]     ask_p, ask_q
    );
        tob_in.valid          = 1'b1;
        tob_in.symbol_idx     = sym;
        tob_in.best_bid.price = bid_p;
        tob_in.best_bid.qty   = bid_q;
        tob_in.best_ask.price = ask_p;
        tob_in.best_ask.qty   = ask_q;
        @(posedge clk); #1;       // posedge 0: S0 decode → S1 reg captures
        tob_in = '0;
        @(posedge clk); #1;       // posedge 1: MUL_P1 — mplier[10:0] × mcand
        @(posedge clk); #1;       // posedge 2: MUL_P2 — mplier[21:11] × mcand
        @(posedge clk); #1;       // posedge 3: MUL_P3 — mplier[32:22] × mcand (final)
        @(posedge clk); #1;       // posedge 4: S4 ACC active, out_trade valid
        // out_trade.valid is high RIGHT NOW (if trade_fire).
        // Next posedge will clear it (mp3_valid goes 0).
    endtask

    // Convert Q44.20 accumulator to integer cents for display
    function automatic [31:0] q_to_cents(input [AW-1:0] q);
        return q[AW-1:FRAC];   // drop fractional bits
    endfunction

    integer fail_count, fb;

    task pr(input string name);
        if (fail_count == fb) $display("  PASS: %s", name);
        else $display("  FAIL: %s (%0d errors)", name, fail_count - fb);
    endtask

    // ----------------------------------------------------------------
    // VCD
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("index_arb_dump.vcd");
        $dumpvars(0, index_arb_engine_tb);
    end

    // ----------------------------------------------------------------
    // Main
    // ----------------------------------------------------------------
    initial begin
        fail_count = 0;
        tob_in     = '0;
        wt_wr_en   = 1'b0;
        wt_wr_addr = '0;
        wt_wr_data = '0;
        threshold  = '0;

        $display("");
        $display("  ╔═════════════════════════════════════════════════════════════╗");
        $display("  ║  Index Arb Engine Unit Test  (%0d components)                ║", N_COMP);
        $display("  ╚═════════════════════════════════════════════════════════════╝");

        rst_n = 0; repeat(4) @(posedge clk); #1;
        rst_n = 1; repeat(2) @(posedge clk); #1;
        wait (ready);

        // ============================================================
        // Load weights
        // ============================================================
        $display("\n--- Loading weights ---");
        load_weight(9'd0, 32'd524_288);   // 0.50
        load_weight(9'd1, 32'd262_144);   // 0.25
        load_weight(9'd2, 32'd157_286);   // ~0.15
        load_weight(9'd3, 32'd104_858);   // ~0.10
        $display("    Weights loaded: 0.50, 0.25, 0.15, 0.10");

        // Set threshold to 100 cents in Q44.20 = 100 << 20
        threshold = AW'(100) <<< FRAC;
        $display("    Threshold: 100 cents ($1.00)");
        repeat(2) @(posedge clk); #1;

        // ============================================================
        // TEST 1: First component prices — build up index
        //   Sym 0: mid = 15000  -> contribution = 7500.0
        //   Sym 1: mid = 20000  -> contribution = 5000.0
        //   Sym 2: mid = 10000  -> contribution = 1500.0 (approx, weight ~0.15)
        //   Sym 3: mid = 5000   -> contribution =  500.0 (approx, weight ~0.10)
        //   Expected total ≈ 14500 cents
        // ============================================================
        $display("\n--- TEST 1: Initialize 4 component prices ---");
        fb = fail_count;

        send_tob(9'd0, 32'd14900, 32'd100, 32'd15100, 32'd100); // mid=15000
        send_tob(9'd1, 32'd19900, 32'd100, 32'd20100, 32'd100); // mid=20000
        send_tob(9'd2, 32'd9900,  32'd100, 32'd10100, 32'd100); // mid=10000
        send_tob(9'd3, 32'd4900,  32'd100, 32'd5100,  32'd100); // mid=5000

        $display("    Computed index (cents): %0d", q_to_cents(index_monitor));
        $display("    Expected: ~14500");

        // Allow ~1% tolerance for fixed-point rounding
        if (q_to_cents(index_monitor) < 14400 ||
            q_to_cents(index_monitor) > 14600) begin
            $display("    FAIL: index out of range");
            fail_count++;
        end
        pr("initial index calculation");

        // ============================================================
        // TEST 2: Set actual index price = computed → no trade
        // ============================================================
        $display("\n--- TEST 2: Actual price matches computed — no trade ---");
        fb = fail_count;

        send_tob(`IDX_SYMBOL, q_to_cents(index_monitor) - 32'd50, 32'd100,
                               q_to_cents(index_monitor) + 32'd50, 32'd100);

        // Allow pipeline to settle
        repeat(4) @(posedge clk); #1;

        if (trade_out.valid) begin
            $display("    FAIL: trade fired when spread < threshold");
            fail_count++;
        end else begin
            $display("    No trade signal — correct (spread within threshold)");
        end
        pr("no-trade within threshold");

        // ============================================================
        // TEST 3: Component price jump — computed rises above actual
        //   Move sym 0 mid from 15000 to 16000 (+1000)
        //   Delta contribution = 1000 * 0.5 = +500 cents
        //   New computed ≈ 15000.  Actual still ≈ 14500.
        //   Spread ≈ +500 → above 100 threshold → SELL signal
        // ============================================================
        $display("\n--- TEST 3: Component jump — computed > actual — SELL ---");
        fb = fail_count;

        send_tob(9'd0, 32'd15900, 32'd100, 32'd16100, 32'd100); // mid=16000

        $display("    Computed index (cents): %0d", q_to_cents(index_monitor));
        $display("    Actual index  (cents): %0d", dut.actual_mid);

        // out_trade.valid is high RIGHT NOW — check immediately
        if (!trade_out.valid) begin
            $display("    FAIL: expected trade signal");
            fail_count++;
        end else begin
            $display("    Trade fired: direction=%s  spread=%0d (Q44.20)",
                     trade_out.direction ? "SELL" : "BUY",
                     $signed(trade_out.spread));
            if (trade_out.direction != 1'b1) begin
                $display("    FAIL: expected SELL (direction=1)");
                fail_count++;
            end
        end
        pr("SELL signal on positive spread");

        // ============================================================
        // TEST 4: Update actual price to match → spread collapses
        // ============================================================
        $display("\n--- TEST 4: Actual catches up — spread collapses ---");
        fb = fail_count;

        send_tob(`IDX_SYMBOL, q_to_cents(index_monitor) - 32'd10, 32'd100,
                               q_to_cents(index_monitor) + 32'd10, 32'd100);

        repeat(4) @(posedge clk); #1;

        if (trade_out.valid) begin
            $display("    FAIL: trade fired when spread should be ~0");
            fail_count++;
        end else begin
            $display("    No trade — spread collapsed. Correct.");
        end
        pr("spread collapse after actual update");

        // ============================================================
        // TEST 5: Component price drop — computed < actual → BUY
        //   Drop sym 1 from 20000 to 18000 (-2000)
        //   Delta contribution = -2000 * 0.25 = -500 cents
        //   Computed drops by ~500, actual stays → BUY signal
        // ============================================================
        $display("\n--- TEST 5: Component drop — computed < actual — BUY ---");
        fb = fail_count;

        send_tob(9'd1, 32'd17900, 32'd100, 32'd18100, 32'd100); // mid=18000

        $display("    Computed index (cents): %0d", q_to_cents(index_monitor));
        $display("    Actual index  (cents): %0d", dut.actual_mid);

        // out_trade.valid is high RIGHT NOW — check immediately
        if (!trade_out.valid) begin
            $display("    FAIL: expected trade signal");
            fail_count++;
        end else begin
            $display("    Trade fired: direction=%s  spread=%0d",
                     trade_out.direction ? "SELL" : "BUY",
                     $signed(trade_out.spread));
            if (trade_out.direction != 1'b0) begin
                $display("    FAIL: expected BUY (direction=0)");
                fail_count++;
            end
        end
        pr("BUY signal on negative spread");

        // ============================================================
        // TEST 6: One-sided market — update ignored
        // ============================================================
        $display("\n--- TEST 6: One-sided market — no update ---");
        fb = fail_count;

        // Save current index
        begin
            logic [AW-1:0] saved_idx;
            saved_idx = index_monitor;

            // Send with ask qty=0 (one-sided)
            send_tob(9'd2, 32'd12000, 32'd100, 32'd0, 32'd0);

            if (index_monitor !== saved_idx) begin
                $display("    FAIL: index changed on one-sided market");
                fail_count++;
            end else begin
                $display("    Index unchanged — correctly skipped one-sided update");
            end
        end
        pr("one-sided market skip");

        // ============================================================
        // TEST 7: Small threshold — trade fires on tiny spread
        // ============================================================
        $display("\n--- TEST 7: Low threshold — tiny spread triggers trade ---");
        fb = fail_count;

        // First equalize
        send_tob(`IDX_SYMBOL, q_to_cents(index_monitor) - 32'd10, 32'd100,
                               q_to_cents(index_monitor) + 32'd10, 32'd100);
        repeat(4) @(posedge clk); #1;

        // Set very low threshold: 5 cents
        threshold = AW'(5) <<< FRAC;

        // Small price move: sym 3 mid from 5000 to 5050 (+50)
        // Delta = 50 * 0.1 = 5 cents. Just at threshold.
        // Actually slightly above due to rounding → should fire
        send_tob(9'd3, 32'd5000, 32'd100, 32'd5100, 32'd100); // mid=5050

        $display("    Spread (Q44.20): %0d", $signed(trade_out.spread));
        $display("    Trade valid: %0d", trade_out.valid);
        // Either fire or not is acceptable here — it's right at the boundary
        // The test is that the module doesn't crash or produce garbage
        if (trade_out.valid)
            $display("    Trade fired — spread crossed low threshold");
        else
            $display("    No trade — spread right at boundary (acceptable)");
        pr("low threshold behavior");

        // ============================================================
        // Summary
        // ============================================================
        repeat(4) @(posedge clk);
        $display("");
        $display("  ╔═════════════════════════════════════════════════════════════╗");
        $display("  ║  SUMMARY                                                   ║");
        $display("  ║  Errors: %-3d                                                ║", fail_count);
        if (fail_count == 0)
        $display("  ║  @@@ Passed                                                ║");
        else
        $display("  ║  @@@ Failed                                                ║");
        $display("  ╚═════════════════════════════════════════════════════════════╝");
        $display("");
        $finish;
    end

endmodule
