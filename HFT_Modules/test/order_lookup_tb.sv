`include "sys_defs.svh"

// ============================================================================
// order_lookup_tb.sv
//
// Module-level testbench for order_lookup.sv
//
// Tests:
//   1.  Post-reset ready
//   2.  Add order — insert and verify output fields
//   3.  Modify order — verify old values returned, side preserved
//   4.  Exec order (partial fill) — verify qty reduced
//   5.  Exec order (full fill) — verify entry removed
//   6.  Delete order — verify old values returned, entry removed
//   7.  Add + Replace — verify old deleted, new inserted
//   8.  Modify on non-existent order — miss passthrough
//   9.  Delete on non-existent order — miss passthrough
//  10.  Multiple adds then deletes — verify table state
// ============================================================================

module order_lookup_tb;

    // ----------------------------------------------------------------
    // DUT parameters
    // ----------------------------------------------------------------
    localparam N_WAYS      = 4;
    localparam TBL_ENTRIES = 64;
    localparam MAX_CHAIN   = 8;

    // ----------------------------------------------------------------
    // DUT signals
    // ----------------------------------------------------------------
    logic              clk, rst_n;
    logic              ready;
    pillar_msg_t       msg_in;
    order_lookup_out_t out;

    // ----------------------------------------------------------------
    // DUT
    // ----------------------------------------------------------------
    order_lookup #(
        .N_WAYS      (N_WAYS),
        .TBL_ENTRIES (TBL_ENTRIES),
        .MAX_CHAIN   (MAX_CHAIN)
    ) dut (
        .clk    (clk),
        .rst_n  (rst_n),
        .ready  (ready),
        .msg_in (msg_in),
        .out    (out)
    );

    // ----------------------------------------------------------------
    // Clock: 250 MHz
    // ----------------------------------------------------------------
    initial clk = 0;
    always #2 clk = ~clk;

    // ----------------------------------------------------------------
    // VCD
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("order_lookup_tb.vcd");
        $dumpvars(0, order_lookup_tb);
    end

    // ----------------------------------------------------------------
    // Scoreboard
    // ----------------------------------------------------------------
    integer fail_count;
    integer test_num;

    // ----------------------------------------------------------------
    // Helper: wait for ready with timeout
    // ----------------------------------------------------------------
    task wait_ready;
        input integer timeout;
        integer cnt;
        cnt = 0;
        while (!ready && cnt < timeout) begin
            @(posedge clk); #1;
            cnt = cnt + 1;
        end
        if (cnt >= timeout) begin
            $display("  FAIL: ready timeout (%0d cycles)", timeout);
            fail_count = fail_count + 1;
        end
    endtask

    // ----------------------------------------------------------------
    // Helper: build a zeroed pillar_msg_t, then fill common fields
    // ----------------------------------------------------------------
    task build_msg;
        output pillar_msg_t m;
        input  [2:0]        msg_type;
        input  [63:0]       order_id;
        input  [31:0]       price;
        input  [31:0]       qty;
        input               side;
        input  [31:0]       symbol_index;

        m               = '0;
        m.valid         = 1'b1;
        m.msg_type      = msg_type;
        m.order_id      = order_id;
        m.price         = price;
        m.qty           = qty;
        m.side          = side;
        m.symbol_index  = symbol_index;
    endtask

    // ----------------------------------------------------------------
    // Helper: send a message and wait for output valid
    // ----------------------------------------------------------------
    task send_and_wait;
        input  pillar_msg_t       m;
        output order_lookup_out_t result;
        integer timeout;

        wait_ready(500);

        // Drive message for one cycle
        @(posedge clk); #1;
        msg_in <= m;

        @(posedge clk); #1;
        msg_in <= '0;

        // Wait for out.valid
        timeout = 0;
        while (!out.valid && timeout < 500) begin
            @(posedge clk); #1;
            timeout = timeout + 1;
        end

        if (timeout >= 500) begin
            $display("  FAIL: output valid timeout");
            fail_count = fail_count + 1;
            result = '0;
        end else begin
            result = out;
        end
    endtask

    // ----------------------------------------------------------------
    // Main test sequence
    // ----------------------------------------------------------------
    pillar_msg_t       stimulus;
    order_lookup_out_t result;

    initial begin
        fail_count = 0;
        test_num   = 0;
        msg_in     = '0;

        // ============================================================
        // Reset
        // ============================================================
        rst_n = 0;
        repeat (4) @(posedge clk); #1;
        rst_n = 1;

        // Wait for hash table BRAM clear
        wait_ready(TBL_ENTRIES + 100);
        $display("\n=== Table ready after reset ===\n");

        // ============================================================
        // TEST 1: Post-reset ready
        // ============================================================
        test_num = 1;
        $display("=== TEST %0d: Post-reset ready ===", test_num);
        if (ready)
            $display("  PASS: ready asserted");
        else begin
            $display("  FAIL: ready not asserted");
            fail_count = fail_count + 1;
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 2: Add order (msg_type = 0)
        //   Insert order_id=0x1001, price=5000, qty=100, side=bid(0)
        //   Expect: output with msg_type=0, old values all zero
        // ============================================================
        test_num = 2;
        $display("\n=== TEST %0d: Add order ===", test_num);

        build_msg(stimulus,
            `MSG_ADD,     // msg_type = 0
            64'h0000_0000_0000_1001,     // order_id
            32'd5000,                    // price
            32'd100,                     // qty
            1'b0,                        // side: bid
            32'd42                       // symbol_index
        );
        send_and_wait(stimulus, result);

        if (result.msg_type !== `MSG_ADD) begin
            $display("  FAIL: msg_type=%0d expected=%0d", result.msg_type, `MSG_ADD);
            fail_count = fail_count + 1;
        end else if (result.old_price !== 32'd0 || result.old_qty !== 32'd0) begin
            $display("  FAIL: Add should have zero old values, got price=%0d qty=%0d",
                result.old_price, result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Add order emitted, old values zero, symbol_idx=%0d", result.symbol_idx);
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 3: Modify order (msg_type = 1)
        //   Modify order_id=0x1001 to price=5500, qty=150
        //   Expect: old_price=5000, old_qty=100, old_side=0 (bid)
        //   Side should be preserved in hash table.
        // ============================================================
        test_num = 3;
        $display("\n=== TEST %0d: Modify order ===", test_num);

        build_msg(stimulus,
            `MSG_MOD,
            64'h0000_0000_0000_1001,
            32'd5500,                    // new price
            32'd150,                     // new qty
            1'b0,                        // side ignored for modify
            32'd42
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd5000) begin
            $display("  FAIL: old_price=%0d expected=5000", result.old_price);
            fail_count = fail_count + 1;
        end else if (result.old_qty !== 32'd100) begin
            $display("  FAIL: old_qty=%0d expected=100", result.old_qty);
            fail_count = fail_count + 1;
        end else if (result.old_side !== 1'b0) begin
            $display("  FAIL: old_side=%0b expected=0 (bid)", result.old_side);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Modify returned old_price=5000 old_qty=100 old_side=bid");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 4: Exec order — partial fill (msg_type = 3)
        //   Exec 50 shares on order_id=0x1001 (currently qty=150)
        //   Expect: old_price=5500, old_qty=150, remaining=100
        // ============================================================
        test_num = 4;
        $display("\n=== TEST %0d: Exec order (partial fill) ===", test_num);

        build_msg(stimulus,
            `MSG_EXEC,
            64'h0000_0000_0000_1001,
            32'd5500,                    // price (for exec logging)
            32'd50,                      // executed qty
            1'b0,
            32'd42
        );
        stimulus.trade_id = 32'd9001;    // set trade_id for exec
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd5500) begin
            $display("  FAIL: old_price=%0d expected=5500", result.old_price);
            fail_count = fail_count + 1;
        end else if (result.old_qty !== 32'd150) begin
            $display("  FAIL: old_qty=%0d expected=150", result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Exec partial, old_price=5500 old_qty=150 (remaining=100)");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 5: Verify partial exec updated qty
        //   Modify order_id=0x1001 to see what's in the table now.
        //   Expect: old_qty=100 (150-50), old_price=5500
        // ============================================================
        test_num = 5;
        $display("\n=== TEST %0d: Verify qty after partial exec ===", test_num);

        build_msg(stimulus,
            `MSG_MOD,
            64'h0000_0000_0000_1001,
            32'd5500,
            32'd100,
            1'b0,
            32'd42
        );
        send_and_wait(stimulus, result);

        if (result.old_qty !== 32'd100) begin
            $display("  FAIL: old_qty=%0d expected=100 (after exec of 50)", result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Table qty=100 after partial exec");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 6: Exec order — full fill (msg_type = 3)
        //   Exec remaining 100 shares on order_id=0x1001
        //   Entry should be deleted from hash table.
        // ============================================================
        test_num = 6;
        $display("\n=== TEST %0d: Exec order (full fill) ===", test_num);

        build_msg(stimulus,
            `MSG_EXEC,
            64'h0000_0000_0000_1001,
            32'd5500,
            32'd100,                     // exec entire remaining qty
            1'b0,
            32'd42
        );
        stimulus.trade_id = 32'd9002;
        send_and_wait(stimulus, result);

        if (result.old_qty !== 32'd100) begin
            $display("  FAIL: old_qty=%0d expected=100", result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Exec full fill, old_qty=100, entry should be removed");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 7: Verify entry removed after full exec
        //   Modify on the same order_id should miss.
        //   Expect: old values all zero (miss passthrough)
        // ============================================================
        test_num = 7;
        $display("\n=== TEST %0d: Verify removal after full exec ===", test_num);

        build_msg(stimulus,
            `MSG_MOD,
            64'h0000_0000_0000_1001,
            32'd6000,
            32'd200,
            1'b0,
            32'd42
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd0 && result.old_qty !== 32'd0) begin
            $display("  FAIL: expected miss (old=0), got old_price=%0d old_qty=%0d",
                result.old_price, result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Order not found after full exec (miss)");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 8: Add + Delete (msg_type = 2)
        //   Add order_id=0x2002, then delete it.
        //   Expect: delete returns old values, subsequent lookup misses.
        // ============================================================
        test_num = 8;
        $display("\n=== TEST %0d: Add then Delete ===", test_num);

        // Add
        build_msg(stimulus,
            `MSG_ADD,
            64'h0000_0000_0000_2002,
            32'd7777,
            32'd300,
            1'b1,                        // side: ask
            32'd99
        );
        send_and_wait(stimulus, result);
        $display("  Add order 0x2002 done");

        // Delete
        build_msg(stimulus,
            `MSG_DEL,
            64'h0000_0000_0000_2002,
            32'd0,
            32'd0,
            1'b0,
            32'd99
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd7777) begin
            $display("  FAIL: old_price=%0d expected=7777", result.old_price);
            fail_count = fail_count + 1;
        end else if (result.old_qty !== 32'd300) begin
            $display("  FAIL: old_qty=%0d expected=300", result.old_qty);
            fail_count = fail_count + 1;
        end else if (result.old_side !== 1'b1) begin
            $display("  FAIL: old_side=%0b expected=1 (ask)", result.old_side);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Delete returned old_price=7777 old_qty=300 old_side=ask");
        end

        // Verify it's gone — modify should miss
        build_msg(stimulus,
            `MSG_MOD,
            64'h0000_0000_0000_2002,
            32'd8888,
            32'd1,
            1'b0,
            32'd99
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd0) begin
            $display("  FAIL: order still in table after delete, old_price=%0d", result.old_price);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Order not found after delete");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 9: Replace order (msg_type = 4)
        //   Add order_id=0x3003, then replace with new_order_id=0x3004
        //   Expect: old values from 0x3003, new entry under 0x3004
        // ============================================================
        test_num = 9;
        $display("\n=== TEST %0d: Replace order ===", test_num);

        // Add original
        build_msg(stimulus,
            `MSG_ADD,
            64'h0000_0000_0000_3003,
            32'd1200,
            32'd500,
            1'b0,                        // bid
            32'd55
        );
        send_and_wait(stimulus, result);
        $display("  Add order 0x3003 done");

        // Replace: old=0x3003, new=0x3004, new price=1300, new qty=450
        build_msg(stimulus,
            `MSG_REPL,
            64'h0000_0000_0000_3003,     // old order_id
            32'd1300,                    // new price
            32'd450,                     // new qty
            1'b0,
            32'd55
        );
        stimulus.new_order_id = 64'h0000_0000_0000_3004;
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd1200) begin
            $display("  FAIL: old_price=%0d expected=1200", result.old_price);
            fail_count = fail_count + 1;
        end else if (result.old_qty !== 32'd500) begin
            $display("  FAIL: old_qty=%0d expected=500", result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Replace returned old_price=1200 old_qty=500");
        end

        // Verify old order_id is gone
        build_msg(stimulus,
            `MSG_MOD,
            64'h0000_0000_0000_3003,
            32'd9999,
            32'd1,
            1'b0,
            32'd55
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd0) begin
            $display("  FAIL: old order_id still in table, old_price=%0d", result.old_price);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Old order_id 0x3003 removed");
        end

        // Verify new order_id exists with correct values
        // (Modify 0x3004 to read its current state)
        build_msg(stimulus,
            `MSG_MOD,
            64'h0000_0000_0000_3004,
            32'd1300,
            32'd450,
            1'b0,
            32'd55
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd1300) begin
            $display("  FAIL: new order old_price=%0d expected=1300", result.old_price);
            fail_count = fail_count + 1;
        end else if (result.old_qty !== 32'd450) begin
            $display("  FAIL: new order old_qty=%0d expected=450", result.old_qty);
            fail_count = fail_count + 1;
        end else if (result.old_side !== 1'b0) begin
            $display("  FAIL: new order old_side=%0b expected=0 (bid, carried from old)",
                result.old_side);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: New order_id 0x3004 has price=1300 qty=450 side=bid");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 10: Modify on non-existent order
        //   Should emit with old values = 0 (miss)
        // ============================================================
        test_num = 10;
        $display("\n=== TEST %0d: Modify non-existent order ===", test_num);

        build_msg(stimulus,
            `MSG_MOD,
            64'hDEAD_BEEF_DEAD_BEEF,
            32'd1111,
            32'd1,
            1'b0,
            32'd77
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd0 || result.old_qty !== 32'd0) begin
            $display("  FAIL: expected miss, got old_price=%0d old_qty=%0d",
                result.old_price, result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Non-existent modify emitted with zero old values");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 11: Delete non-existent order
        // ============================================================
        test_num = 11;
        $display("\n=== TEST %0d: Delete non-existent order ===", test_num);

        build_msg(stimulus,
            `MSG_DEL,
            64'hBAD0_BAD0_BAD0_BAD0,
            32'd0,
            32'd0,
            1'b0,
            32'd77
        );
        send_and_wait(stimulus, result);

        if (result.old_price !== 32'd0 || result.old_qty !== 32'd0) begin
            $display("  FAIL: expected miss, got old_price=%0d old_qty=%0d",
                result.old_price, result.old_qty);
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: Non-existent delete emitted with zero old values");
        end

        repeat (2) @(posedge clk); #1;

        // ============================================================
        // TEST 12: Multiple adds, then bulk verify via modify
        //   Add 8 orders across two symbols, bid and ask sides.
        //   Then modify each to read back old values.
        // ============================================================
        test_num = 12;
        $display("\n=== TEST %0d: Bulk add + verify ===", test_num);
        begin
            integer i;
            logic [63:0] oid;
            logic [31:0] exp_price, exp_qty;
            logic        exp_side;

            // Add 8 orders
            for (i = 0; i < 8; i = i + 1) begin
                build_msg(stimulus,
                    `MSG_ADD,
                    64'hA000_0000_0000_0000 + i,   // order_id
                    32'd1000 + i * 100,             // price
                    32'd10 + i * 10,                // qty
                    i[0],                           // alternate bid/ask
                    32'd200 + (i / 4)               // two symbols
                );
                send_and_wait(stimulus, result);
            end
            $display("  Inserted 8 orders");

            // Verify each via modify (reads old, writes same values back)
            for (i = 0; i < 8; i = i + 1) begin
                oid       = 64'hA000_0000_0000_0000 + i;
                exp_price = 32'd1000 + i * 100;
                exp_qty   = 32'd10 + i * 10;
                exp_side  = i[0];

                build_msg(stimulus,
                    `MSG_MOD,
                    oid,
                    exp_price,        // write back same price
                    exp_qty,          // write back same qty
                    1'b0,
                    32'd200 + (i / 4)
                );
                send_and_wait(stimulus, result);

                if (result.old_price !== exp_price) begin
                    $display("  FAIL: order[%0d] old_price=%0d expected=%0d",
                        i, result.old_price, exp_price);
                    fail_count = fail_count + 1;
                end else if (result.old_qty !== exp_qty) begin
                    $display("  FAIL: order[%0d] old_qty=%0d expected=%0d",
                        i, result.old_qty, exp_qty);
                    fail_count = fail_count + 1;
                end else if (result.old_side !== exp_side) begin
                    $display("  FAIL: order[%0d] old_side=%0b expected=%0b",
                        i, result.old_side, exp_side);
                    fail_count = fail_count + 1;
                end else begin
                    $display("  PASS: order[%0d] price=%0d qty=%0d side=%0b",
                        i, result.old_price, result.old_qty, result.old_side);
                end
            end
        end

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // Summary
        // ============================================================
        $display("\n========================================");
        if (fail_count == 0)
            $display("ALL TESTS PASSED (%0d tests)", test_num);
        else
            $display("%0d FAILURE(S) across %0d tests", fail_count, test_num);
        $display("========================================\n");
        $finish;
    end

    // ----------------------------------------------------------------
    // Watchdog
    // ----------------------------------------------------------------
    initial begin
        #500_000;
        $display("\nFATAL: Simulation watchdog timeout");
        $finish;
    end

endmodule