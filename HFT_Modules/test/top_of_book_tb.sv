`timescale 1ns/1ps
`include "sys_defs.svh"
// ============================================================
// top_of_book_tb.sv
//
// Unit testbench for top_of_book.
// Drives order_lookup_out_t directly (no filter/framer/demux).
// Tests sorted insert, remove+shift, level sweep, aggregation,
// modify, exec, replace, discard, and symbol isolation.
// ============================================================
module top_of_book_tb;

    localparam N_LEVELS  = `TOB_LEVELS;
    localparam N_SYMBOLS = `TOB_SYMBOLS;
    localparam SYM_W     = $clog2(N_SYMBOLS);

    logic                clk, rst_n;
    order_lookup_out_t   update;
    tob_out_t            tob;

    top_of_book #(
        .N_LEVELS  (N_LEVELS),
        .N_SYMBOLS (N_SYMBOLS)
    ) dut (
        .clk       (clk),
        .rst_n     (rst_n),
        .in_update (update),
        .out_tob   (tob)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    // ----------------------------------------------------------------
    // Helper tasks — build order_lookup_out_t from simple arguments
    // ----------------------------------------------------------------
    task automatic send(
        input [2:0]  mtype,
        input [31:0] sym,
        input [63:0] oid,
        input [31:0] price,
        input [31:0] qty,
        input logic  side,
        input [31:0] old_price,
        input [31:0] old_qty,
        input logic  old_side,
        input [63:0] new_oid
    );
        update                          = '0;
        update.valid                    = 1'b1;
        update.msg_type                 = mtype;
        update.pillar_msg.valid         = 1'b1;
        update.pillar_msg.msg_type      = mtype;
        update.pillar_msg.symbol_index  = sym;
        update.pillar_msg.order_id      = oid;
        update.pillar_msg.new_order_id  = new_oid;
        update.pillar_msg.price         = price;
        update.pillar_msg.qty           = qty;
        update.pillar_msg.side          = side;
        update.symbol_idx               = sym;
        update.old_price                = old_price;
        update.old_qty                  = old_qty;
        update.old_side                 = old_side;
        @(posedge clk); #1;
        update = '0;
        @(posedge clk); #1;      // wait for output to register
    endtask

    task automatic send_add(
        input [31:0] sym, input [63:0] oid,
        input [31:0] price, input [31:0] qty, input logic side
    );
        send(`MSG_ADD, sym, oid, price, qty, side, '0, '0, '0, '0);
    endtask

    task automatic send_del(
        input [31:0] sym, input [63:0] oid,
        input [31:0] old_price, input [31:0] old_qty, input logic old_side
    );
        send(`MSG_DEL, sym, oid, '0, '0, '0, old_price, old_qty, old_side, '0);
    endtask

    task automatic send_exec(
        input [31:0] sym, input [63:0] oid,
        input [31:0] exec_qty,
        input [31:0] old_price, input [31:0] old_qty, input logic old_side
    );
        send(`MSG_EXEC, sym, oid, '0, exec_qty, '0, old_price, old_qty, old_side, '0);
    endtask

    task automatic send_mod(
        input [31:0] sym, input [63:0] oid,
        input [31:0] new_price, input [31:0] new_qty,
        input [31:0] old_price, input [31:0] old_qty, input logic old_side
    );
        send(`MSG_MOD, sym, oid, new_price, new_qty, '0, old_price, old_qty, old_side, '0);
    endtask

    task automatic send_repl(
        input [31:0] sym, input [63:0] oid, input [63:0] new_oid,
        input [31:0] new_price, input [31:0] new_qty,
        input [31:0] old_price, input [31:0] old_qty, input logic old_side
    );
        send(`MSG_REPL, sym, oid, new_price, new_qty, '0,
             old_price, old_qty, old_side, new_oid);
    endtask

    // ----------------------------------------------------------------
    // Display and checking
    // ----------------------------------------------------------------
    integer fail_count, fb;

    task print_bbo();
        $display("    BBO: bid=%0d x %0d  |  ask=%0d x %0d",
                 tob.best_bid.price, tob.best_bid.qty,
                 tob.best_ask.price, tob.best_ask.qty);
    endtask

    task print_book(input [31:0] s);
        $display("    Book for symbol %0d:", s);
        for (int i = 0; i < N_LEVELS; i++)
            $display("      bid[%0d]: %6d x %4d   ask[%0d]: %6d x %4d",
                     i, dut.bid_book[s[SYM_W-1:0]][i].price,
                        dut.bid_book[s[SYM_W-1:0]][i].qty,
                     i, dut.ask_book[s[SYM_W-1:0]][i].price,
                        dut.ask_book[s[SYM_W-1:0]][i].qty);
    endtask

    task check_bid(input [31:0] ep, eq);
        if (tob.best_bid.price !== ep || tob.best_bid.qty !== eq) begin
            $display("    FAIL bid: got %0d x %0d, exp %0d x %0d",
                     tob.best_bid.price, tob.best_bid.qty, ep, eq);
            fail_count++;
        end
    endtask

    task check_ask(input [31:0] ep, eq);
        if (tob.best_ask.price !== ep || tob.best_ask.qty !== eq) begin
            $display("    FAIL ask: got %0d x %0d, exp %0d x %0d",
                     tob.best_ask.price, tob.best_ask.qty, ep, eq);
            fail_count++;
        end
    endtask

    task check_level(
        input logic  side_is_bid,
        input [31:0] s,
        input integer lvl,
        input [31:0] ep, eq
    );
        logic [31:0] gp, gq;
        if (side_is_bid) begin
            gp = dut.bid_book[s[SYM_W-1:0]][lvl].price;
            gq = dut.bid_book[s[SYM_W-1:0]][lvl].qty;
        end else begin
            gp = dut.ask_book[s[SYM_W-1:0]][lvl].price;
            gq = dut.ask_book[s[SYM_W-1:0]][lvl].qty;
        end
        if (gp !== ep || gq !== eq) begin
            $display("    FAIL %s[%0d]: got %0d x %0d, exp %0d x %0d",
                     side_is_bid ? "bid" : "ask", lvl, gp, gq, ep, eq);
            fail_count++;
        end
    endtask

    task pr(input string name);
        if (fail_count == fb)
            $display("  PASS: %s", name);
        else
            $display("  FAIL: %s (%0d errors)", name, fail_count - fb);
    endtask

    // ----------------------------------------------------------------
    // VCD
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("tob_dump.vcd");
        $dumpvars(0, top_of_book_tb);
    end

    // ----------------------------------------------------------------
    // Main test sequence — all on symbol 5 unless noted
    // ----------------------------------------------------------------
    localparam [31:0] SYM = 32'd5;

    initial begin
        fail_count = 0;
        update = '0;

        $display("");
        $display("  ╔═════════════════════════════════════════════════════════════╗");
        $display("  ║  Top-of-Book Unit Test  (N=%0d, SYMS=%0d)                    ║",
                 N_LEVELS, N_SYMBOLS);
        $display("  ╚═════════════════════════════════════════════════════════════╝");

        rst_n = 0; repeat(4) @(posedge clk); #1;
        rst_n = 1; repeat(2) @(posedge clk); #1;

        // ============================================================
        // TEST 1: Add three bid levels — sorted descending
        // ============================================================
        $display("\n--- TEST 1: Add 3 bid levels ---");
        fb = fail_count;

        send_add(SYM, 64'h1, 32'd25000, 32'd100, `SIDE_BUY);
        send_add(SYM, 64'h2, 32'd25200, 32'd200, `SIDE_BUY);
        send_add(SYM, 64'h3, 32'd25100, 32'd150, `SIDE_BUY);

        print_book(SYM); print_bbo();
        check_bid(32'd25200, 32'd200);
        check_level(1, SYM, 1, 32'd25100, 32'd150);
        check_level(1, SYM, 2, 32'd25000, 32'd100);
        pr("3 bid levels sorted desc");

        // ============================================================
        // TEST 2: Add three ask levels — sorted ascending
        // ============================================================
        $display("\n--- TEST 2: Add 3 ask levels ---");
        fb = fail_count;

        send_add(SYM, 64'h4, 32'd25400, 32'd300, `SIDE_SELL);
        send_add(SYM, 64'h5, 32'd25300, 32'd250, `SIDE_SELL);
        send_add(SYM, 64'h6, 32'd25500, 32'd100, `SIDE_SELL);

        print_book(SYM); print_bbo();
        check_ask(32'd25300, 32'd250);
        check_level(0, SYM, 1, 32'd25400, 32'd300);
        check_level(0, SYM, 2, 32'd25500, 32'd100);
        pr("3 ask levels sorted asc");

        // ============================================================
        // TEST 3: Add to existing level — aggregate qty
        // ============================================================
        $display("\n--- TEST 3: Aggregate qty on existing level ---");
        fb = fail_count;

        send_add(SYM, 64'h7, 32'd25200, 32'd50, `SIDE_BUY);

        print_bbo();
        check_bid(32'd25200, 32'd250);   // 200 + 50
        pr("qty aggregation");

        // ============================================================
        // TEST 4: Partial delete from best bid
        // ============================================================
        $display("\n--- TEST 4: Partial delete from best bid ---");
        fb = fail_count;

        send_del(SYM, 64'h2, 32'd25200, 32'd200, `SIDE_BUY);

        print_book(SYM); print_bbo();
        check_bid(32'd25200, 32'd50);     // 250 - 200 = 50
        pr("partial level removal");

        // ============================================================
        // TEST 5: Delete empties a level — shift up
        // ============================================================
        $display("\n--- TEST 5: Delete empties best bid, shift up ---");
        fb = fail_count;

        send_del(SYM, 64'h7, 32'd25200, 32'd50, `SIDE_BUY);

        print_book(SYM); print_bbo();
        check_bid(32'd25100, 32'd150);    // 25200 gone, 25100 promoted
        check_level(1, SYM, 1, 32'd25000, 32'd100);
        check_level(1, SYM, 2, 32'd0, 32'd0);  // empty
        pr("level shift on empty");

        // ============================================================
        // TEST 6: Exec partial fill
        // ============================================================
        $display("\n--- TEST 6: Exec partial fill ---");
        fb = fail_count;

        // Best bid: 25100 x 150
        send_exec(SYM, 64'h3, 32'd80, 32'd25100, 32'd150, `SIDE_BUY);

        print_bbo();
        check_bid(32'd25100, 32'd70);     // 150 - 80
        pr("exec partial fill");

        // ============================================================
        // TEST 7: Exec full fill — sweeps best bid
        // ============================================================
        $display("\n--- TEST 7: Exec full fill sweeps level ---");
        fb = fail_count;

        send_exec(SYM, 64'h3, 32'd70, 32'd25100, 32'd70, `SIDE_BUY);

        print_book(SYM); print_bbo();
        check_bid(32'd25000, 32'd100);    // 25100 swept, 25000 promoted
        pr("exec full fill + shift");

        // ============================================================
        // TEST 8: Modify — price change
        // ============================================================
        $display("\n--- TEST 8: Modify to better price ---");
        fb = fail_count;

        // Only bid: 25000 x 100. Move to 25300 x 80.
        send_mod(SYM, 64'h1, 32'd25300, 32'd80,
                               32'd25000, 32'd100, `SIDE_BUY);

        print_book(SYM); print_bbo();
        check_bid(32'd25300, 32'd80);
        check_level(1, SYM, 1, 32'd0, 32'd0);  // only 1 bid left
        pr("modify to better price");

        // ============================================================
        // TEST 9: Replace order on ask side
        // ============================================================
        $display("\n--- TEST 9: Replace best ask ---");
        fb = fail_count;

        // Best ask: 25300 x 250. Replace to 25250 x 200.
        send_repl(SYM, 64'h5, 64'hFF,
                  32'd25250, 32'd200,
                  32'd25300, 32'd250, `SIDE_SELL);

        print_book(SYM); print_bbo();
        check_ask(32'd25250, 32'd200);
        pr("replace order");

        // ============================================================
        // TEST 10: Insert worse than all 3 levels — discard
        // ============================================================
        $display("\n--- TEST 10: Discard too-deep order ---");
        fb = fail_count;

        // Fill 3 bid levels
        send_add(SYM, 64'hA, 32'd25200, 32'd100, `SIDE_BUY);
        send_add(SYM, 64'hB, 32'd25100, 32'd100, `SIDE_BUY);
        // Bid book: 25300x80, 25200x100, 25100x100
        // Add at 24000 — too deep
        send_add(SYM, 64'hC, 32'd24000, 32'd500, `SIDE_BUY);

        print_book(SYM); print_bbo();
        check_bid(32'd25300, 32'd80);
        check_level(1, SYM, 2, 32'd25100, 32'd100);  // not 24000
        pr("discard too-deep order");

        // ============================================================
        // TEST 11: Full sweep — all bid levels emptied
        // ============================================================
        $display("\n--- TEST 11: Full sweep to empty ---");
        fb = fail_count;

        send_del(SYM, 64'hX0, 32'd25300, 32'd80,  `SIDE_BUY);
        send_del(SYM, 64'hX1, 32'd25200, 32'd100, `SIDE_BUY);
        send_del(SYM, 64'hX2, 32'd25100, 32'd100, `SIDE_BUY);

        print_book(SYM); print_bbo();
        check_bid(32'd0, 32'd0);
        pr("full sweep to empty");

        // ============================================================
        // TEST 12: Add to empty book
        // ============================================================
        $display("\n--- TEST 12: Add to empty book ---");
        fb = fail_count;

        send_add(SYM, 64'hD, 32'd24500, 32'd300, `SIDE_BUY);

        print_bbo();
        check_bid(32'd24500, 32'd300);
        pr("add to empty book");

        // ============================================================
        // TEST 13: Different symbol — isolation
        // ============================================================
        $display("\n--- TEST 13: Symbol isolation ---");
        fb = fail_count;

        send_add(32'd10, 64'hE, 32'd99999, 32'd1, `SIDE_BUY);

        // Symbol 5 must be untouched
        check_level(1, SYM, 0, 32'd24500, 32'd300);
        // Symbol 10 must have its own value
        check_level(1, 32'd10, 0, 32'd99999, 32'd1);
        pr("symbol isolation");

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