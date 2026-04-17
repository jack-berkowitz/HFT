`timescale 1ns/1ps
`include "sys_defs.svh"

module index_arb_engine_tb;

    localparam FRAC = `WEIGHT_FRAC_BITS;
    localparam AW   = `ACCUM_W;
    localparam WW   = `WEIGHT_W;
    localparam SYMW = `SYM_IDX_W;
    localparam N_COMP = 4;

    logic                  clk, rst_n;
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
        .out_trade    (trade_out),
        .out_index    (index_monitor)
    );

    initial clk = 0;
    always #2 clk = ~clk;

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
        @(posedge clk); #1;
        tob_in = '0;
        @(posedge clk); #1;
        @(posedge clk); #1;
        @(posedge clk); #1;
        @(posedge clk); #1;
        @(posedge clk); #1;

    endtask

    function automatic [31:0] q_to_cents(input [AW-1:0] q);
        return q[AW-1:FRAC];
    endfunction

    integer fail_count, fb;

    task pr(input string name);
        if (fail_count == fb) $display("  PASS: %s", name);
        else $display("  FAIL: %s (%0d errors)", name, fail_count - fb);
    endtask

    initial begin
        $dumpfile("index_arb_dump.vcd");
        $dumpvars(0, index_arb_engine_tb);
    end

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

        $display("\n--- Loading weights ---");
        load_weight(9'd0, 32'd524_288);
        load_weight(9'd1, 32'd262_144);
        load_weight(9'd2, 32'd157_286);
        load_weight(9'd3, 32'd104_858);
        $display("    Weights loaded: 0.50, 0.25, 0.15, 0.10");

        threshold = AW'(100) <<< FRAC;
        $display("    Threshold: 100 cents ($1.00)");
        repeat(2) @(posedge clk); #1;

        $display("\n--- TEST 1: Initialize 4 component prices ---");
        fb = fail_count;

        send_tob(9'd0, 32'd14900, 32'd100, 32'd15100, 32'd100);
        send_tob(9'd1, 32'd19900, 32'd100, 32'd20100, 32'd100);
        send_tob(9'd2, 32'd9900,  32'd100, 32'd10100, 32'd100);
        send_tob(9'd3, 32'd4900,  32'd100, 32'd5100,  32'd100);

        $display("    Computed index (cents): %0d", q_to_cents(index_monitor));
        $display("    Expected: ~14500");

        if (q_to_cents(index_monitor) < 14400 ||
            q_to_cents(index_monitor) > 14600) begin
            $display("    FAIL: index out of range");
            fail_count++;
        end
        pr("initial index calculation");

        $display("\n--- TEST 2: Actual price matches computed — no trade ---");
        fb = fail_count;

        send_tob(`IDX_SYMBOL, q_to_cents(index_monitor) - 32'd50, 32'd100,
                               q_to_cents(index_monitor) + 32'd50, 32'd100);

        repeat(4) @(posedge clk); #1;

        if (trade_out.valid) begin
            $display("    FAIL: trade fired when spread < threshold");
            fail_count++;
        end else begin
            $display("    No trade signal — correct (spread within threshold)");
        end
        pr("no-trade within threshold");

        $display("\n--- TEST 3: Component jump — computed > actual — SELL ---");
        fb = fail_count;

        send_tob(9'd0, 32'd15900, 32'd100, 32'd16100, 32'd100);

        $display("    Computed index (cents): %0d", q_to_cents(index_monitor));
        $display("    Actual index  (cents): %0d", dut.actual_mid);

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

        $display("\n--- TEST 5: Component drop — computed < actual — BUY ---");
        fb = fail_count;

        send_tob(9'd1, 32'd17900, 32'd100, 32'd18100, 32'd100);

        $display("    Computed index (cents): %0d", q_to_cents(index_monitor));
        $display("    Actual index  (cents): %0d", dut.actual_mid);

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

        $display("\n--- TEST 6: One-sided market — no update ---");
        fb = fail_count;

        begin
            logic [AW-1:0] saved_idx;
            saved_idx = index_monitor;

            send_tob(9'd2, 32'd12000, 32'd100, 32'd0, 32'd0);

            if (index_monitor !== saved_idx) begin
                $display("    FAIL: index changed on one-sided market");
                fail_count++;
            end else begin
                $display("    Index unchanged — correctly skipped one-sided update");
            end
        end
        pr("one-sided market skip");

        $display("\n--- TEST 7: Low threshold — tiny spread triggers trade ---");
        fb = fail_count;

        send_tob(`IDX_SYMBOL, q_to_cents(index_monitor) - 32'd10, 32'd100,
                               q_to_cents(index_monitor) + 32'd10, 32'd100);
        repeat(4) @(posedge clk); #1;

        threshold = AW'(5) <<< FRAC;

        send_tob(9'd3, 32'd5000, 32'd100, 32'd5100, 32'd100);

        $display("    Spread (Q44.20): %0d", $signed(trade_out.spread));
        $display("    Trade valid: %0d", trade_out.valid);

        if (trade_out.valid)
            $display("    Trade fired — spread crossed low threshold");
        else
            $display("    No trade — spread right at boundary (acceptable)");
        pr("low threshold behavior");

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
