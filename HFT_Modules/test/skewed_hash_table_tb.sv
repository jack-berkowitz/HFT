`timescale 1ns/1ps

module skewed_hash_table_tb;

    localparam KEY_W       = 32;
    localparam VALUE_W     = 64;
    localparam N_WAYS      = 4;
    localparam TBL_ENTRIES = 2;
    localparam MAX_CHAIN   = 8;

    logic                  clk, rst_n;
    logic                  ready;

    logic                  lookup_valid;
    logic [KEY_W-1:0]      lookup_key;
    logic                  lookup_result_valid;
    logic                  lookup_hit;
    logic [VALUE_W-1:0]    lookup_value;

    logic                  insert_valid;
    logic [KEY_W-1:0]      insert_key;
    logic [VALUE_W-1:0]    insert_value;
    logic                  insert_ready;
    logic                  insert_done;
    logic                  insert_fail;

    logic                  delete_valid;
    logic [KEY_W-1:0]      delete_key;
    logic                  delete_ready;
    logic                  delete_done;
    logic                  delete_not_found;

    skewed_hash_table #(
        .KEY_W       (KEY_W),
        .VALUE_W     (VALUE_W),
        .N_WAYS      (N_WAYS),
        .TBL_ENTRIES (TBL_ENTRIES),
        .MAX_CHAIN   (MAX_CHAIN)
    ) dut (
        .clk                (clk),
        .rst_n              (rst_n),
        .ready              (ready),

        .lookup_valid       (lookup_valid),
        .lookup_key         (lookup_key),
        .lookup_result_valid(lookup_result_valid),
        .lookup_hit         (lookup_hit),
        .lookup_value       (lookup_value),

        .insert_valid       (insert_valid),
        .insert_key         (insert_key),
        .insert_value       (insert_value),
        .insert_ready       (insert_ready),
        .insert_done        (insert_done),
        .insert_fail        (insert_fail),

        .delete_valid       (delete_valid),
        .delete_key         (delete_key),
        .delete_ready       (delete_ready),
        .delete_done        (delete_done),
        .delete_not_found   (delete_not_found)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    initial begin
        $dumpfile("skewed_hash_table_tb.vcd");
        $dumpvars(0, skewed_hash_table_tb);
    end

    integer fail_count;
    integer test_num;

    logic               cap_hits   [0:31];
    logic [VALUE_W-1:0] cap_values [0:31];
    integer             cap_wr;

    always @(posedge clk) begin
        if (lookup_result_valid) begin
            cap_hits[cap_wr]   <= lookup_hit;
            cap_values[cap_wr] <= lookup_value;
            cap_wr             <= cap_wr + 1;
        end
    end

    task wait_ready;
        input integer timeout;
        integer cnt;
        cnt = 0;
        while (!ready && cnt < timeout) begin
            @(posedge clk); #1;
            cnt = cnt + 1;
        end
        if (cnt >= timeout) begin
            $display("FAIL: ready never asserted (timeout %0d cycles)", timeout);
            fail_count = fail_count + 1;
        end
    endtask

    task do_insert;
        input [KEY_W-1:0]    key;
        input [VALUE_W-1:0]  value;
        output logic         success;
        integer timeout;

        wait_ready(200);

        @(posedge clk); #1;
        insert_valid <= 1'b1;
        insert_key   <= key;
        insert_value <= value;

        @(posedge clk); #1;
        insert_valid <= 1'b0;

        timeout = 0;
        while (!insert_done && !insert_fail && timeout < 500) begin
            @(posedge clk); #1;
            timeout = timeout + 1;
        end

        if (timeout >= 500) begin
            $display("  FAIL: insert timeout for key=%h", key);
            success = 1'b0;
            fail_count = fail_count + 1;
        end else if (insert_fail) begin
            success = 1'b0;
        end else begin
            success = 1'b1;
        end
    endtask

    task do_lookup;
        input  [KEY_W-1:0]    key;
        output logic           hit;
        output [VALUE_W-1:0]   value;
        integer timeout;

        @(posedge clk); #1;
        lookup_valid <= 1'b1;
        lookup_key   <= key;

        @(posedge clk); #1;
        lookup_valid <= 1'b0;

        timeout = 0;
        while (!lookup_result_valid && timeout < 20) begin
            @(posedge clk); #1;
            timeout = timeout + 1;
        end

        if (timeout >= 20) begin
            $display("  FAIL: lookup result never valid for key=%h", key);
            hit   = 1'b0;
            value = '0;
            fail_count = fail_count + 1;
        end else begin
            hit   = lookup_hit;
            value = lookup_value;
        end
    endtask

    task do_delete;
        input  [KEY_W-1:0]    key;
        output logic           found;
        integer timeout;

        wait_ready(200);

        @(posedge clk); #1;
        delete_valid <= 1'b1;
        delete_key   <= key;

        @(posedge clk); #1;
        delete_valid <= 1'b0;

        timeout = 0;
        while (!delete_done && !delete_not_found && timeout < 500) begin
            @(posedge clk); #1;
            timeout = timeout + 1;
        end

        if (timeout >= 500) begin
            $display("  FAIL: delete timeout for key=%h", key);
            found = 1'b0;
            fail_count = fail_count + 1;
        end else begin
            found = delete_done;
        end
    endtask

    task check_lookup;
        input [KEY_W-1:0]    key;
        input logic          exp_hit;
        input [VALUE_W-1:0]  exp_value;
        logic                got_hit;
        logic [VALUE_W-1:0]  got_value;

        do_lookup(key, got_hit, got_value);

        if (got_hit !== exp_hit) begin
            $display("  FAIL: key=%h hit=%0b expected=%0b", key, got_hit, exp_hit);
            fail_count = fail_count + 1;
        end else if (exp_hit && got_value !== exp_value) begin
            $display("  FAIL: key=%h value=%h expected=%h", key, got_value, exp_value);
            fail_count = fail_count + 1;
        end else begin
            if (exp_hit)
                $display("  PASS: key=%h -> value=%h", key, got_value);
            else
                $display("  PASS: key=%h -> miss (expected)", key);
        end
    endtask

    logic        ins_ok;
    logic        del_ok;
    logic        lu_hit;
    logic [VALUE_W-1:0] lu_val;
    integer      i;

    logic [KEY_W-1:0]   test_keys   [0:7];
    logic [VALUE_W-1:0] test_values [0:7];

    initial begin
        fail_count   = 0;
        test_num     = 0;
        cap_wr       = 0;

        lookup_valid = 1'b0;
        lookup_key   = '0;
        insert_valid = 1'b0;
        insert_key   = '0;
        insert_value = '0;
        delete_valid = 1'b0;
        delete_key   = '0;

        test_keys[0] = 32'h0000_1001; test_values[0] = 64'hAAAA_BBBB_CCCC_0001;
        test_keys[1] = 32'h0000_2002; test_values[1] = 64'hAAAA_BBBB_CCCC_0002;
        test_keys[2] = 32'h0000_3003; test_values[2] = 64'hAAAA_BBBB_CCCC_0003;
        test_keys[3] = 32'h0000_4004; test_values[3] = 64'hAAAA_BBBB_CCCC_0004;
        test_keys[4] = 32'h0000_5005; test_values[4] = 64'hAAAA_BBBB_CCCC_0005;
        test_keys[5] = 32'h0000_6006; test_values[5] = 64'hAAAA_BBBB_CCCC_0006;
        test_keys[6] = 32'h0000_7007; test_values[6] = 64'hAAAA_BBBB_CCCC_0007;
        test_keys[7] = 32'h0000_8008; test_values[7] = 64'hAAAA_BBBB_CCCC_0008;

        rst_n = 0;
        repeat (4) @(posedge clk); #1;
        rst_n = 1;

        wait_ready(TBL_ENTRIES + 20);
        $display("\n=== Table ready after reset clear ===\n");

        test_num = 1;
        $display("=== TEST %0d: Post-reset ready ===", test_num);
        if (ready)
            $display("  PASS: ready asserted after clear");
        else begin
            $display("  FAIL: ready not asserted");
            fail_count = fail_count + 1;
        end

        repeat (2) @(posedge clk); #1;

        test_num = 2;
        $display("\n=== TEST %0d: Lookup miss on empty table ===", test_num);
        check_lookup(32'hDEAD_BEEF, 1'b0, '0);

        repeat (2) @(posedge clk); #1;

        test_num = 3;
        $display("\n=== TEST %0d: Single insert + lookup hit ===", test_num);
        do_insert(test_keys[0], test_values[0], ins_ok);
        if (!ins_ok) begin
            $display("  FAIL: insert unsuccessful");
            fail_count = fail_count + 1;
        end else begin
            $display("  Insert key=%h value=%h done", test_keys[0], test_values[0]);
        end

        check_lookup(test_keys[0], 1'b1, test_values[0]);

        check_lookup(32'hFFFF_FFFF, 1'b0, '0);

        repeat (2) @(posedge clk); #1;

        test_num = 4;
        $display("\n=== TEST %0d: Multiple inserts + lookup all ===", test_num);

        for (i = 1; i < 8; i = i + 1) begin
            do_insert(test_keys[i], test_values[i], ins_ok);
            if (!ins_ok) begin
                $display("  FAIL: insert %0d unsuccessful (key=%h)", i, test_keys[i]);
                fail_count = fail_count + 1;
            end
        end
        $display("  Inserted keys 1..7");

        for (i = 0; i < 8; i = i + 1) begin
            check_lookup(test_keys[i], 1'b1, test_values[i]);
        end

        repeat (2) @(posedge clk); #1;

        test_num = 5;
        $display("\n=== TEST %0d: Update existing key ===", test_num);

        do_insert(test_keys[0], 64'hFE00_DEAD_BEEF_CAFE, ins_ok);
        if (!ins_ok) begin
            $display("  FAIL: update insert unsuccessful");
            fail_count = fail_count + 1;
        end

        check_lookup(test_keys[0], 1'b1, 64'hFE00_DEAD_BEEF_CAFE);

        check_lookup(test_keys[1], 1'b1, test_values[1]);

        repeat (2) @(posedge clk); #1;

        test_num = 6;
        $display("\n=== TEST %0d: Delete + verify miss ===", test_num);

        do_delete(test_keys[2], del_ok);
        if (!del_ok) begin
            $display("  FAIL: delete reported not found for existing key");
            fail_count = fail_count + 1;
        end else begin
            $display("  Deleted key=%h", test_keys[2]);
        end

        check_lookup(test_keys[2], 1'b0, '0);

        check_lookup(test_keys[3], 1'b1, test_values[3]);

        repeat (2) @(posedge clk); #1;

        test_num = 7;
        $display("\n=== TEST %0d: Delete non-existent key ===", test_num);

        do_delete(32'hBAD0_BAD0, del_ok);
        if (del_ok) begin
            $display("  FAIL: delete reported found for non-existent key");
            fail_count = fail_count + 1;
        end else begin
            $display("  PASS: delete correctly reported not_found");
        end

        repeat (2) @(posedge clk); #1;

        test_num = 8;
        $display("\n=== TEST %0d: Back-to-back lookups ===", test_num);
        begin
            logic [KEY_W-1:0]   bb_keys [0:4];
            logic               bb_exp_hit [0:4];
            logic [VALUE_W-1:0] bb_exp_val [0:4];
            integer bb_idx;
            integer bb_base;

            bb_keys[0] = test_keys[0]; bb_exp_hit[0] = 1'b1; bb_exp_val[0] = 64'hFE00_DEAD_BEEF_CAFE;
            bb_keys[1] = test_keys[1]; bb_exp_hit[1] = 1'b1; bb_exp_val[1] = test_values[1];
            bb_keys[2] = test_keys[2]; bb_exp_hit[2] = 1'b0; bb_exp_val[2] = '0;
            bb_keys[3] = test_keys[3]; bb_exp_hit[3] = 1'b1; bb_exp_val[3] = test_values[3];
            bb_keys[4] = test_keys[4]; bb_exp_hit[4] = 1'b1; bb_exp_val[4] = test_values[4];

            bb_base = cap_wr;

            for (bb_idx = 0; bb_idx < 5; bb_idx = bb_idx + 1) begin
                @(posedge clk); #1;
                lookup_valid <= 1'b1;
                lookup_key   <= bb_keys[bb_idx];
            end
            @(posedge clk); #1;
            lookup_valid <= 1'b0;

            repeat (4) @(posedge clk); #1;

            if ((cap_wr - bb_base) < 5) begin
                $display("  FAIL: only %0d/5 lookup results captured", cap_wr - bb_base);
                fail_count = fail_count + 1;
            end else begin
                for (bb_idx = 0; bb_idx < 5; bb_idx = bb_idx + 1) begin
                    if (cap_hits[bb_base + bb_idx] !== bb_exp_hit[bb_idx]) begin
                        $display("  FAIL: bb[%0d] key=%h hit=%0b expected=%0b",
                            bb_idx, bb_keys[bb_idx],
                            cap_hits[bb_base + bb_idx], bb_exp_hit[bb_idx]);
                        fail_count = fail_count + 1;
                    end else if (bb_exp_hit[bb_idx] &&
                                cap_values[bb_base + bb_idx] !== bb_exp_val[bb_idx]) begin
                        $display("  FAIL: bb[%0d] key=%h value=%h expected=%h",
                            bb_idx, bb_keys[bb_idx],
                            cap_values[bb_base + bb_idx], bb_exp_val[bb_idx]);
                        fail_count = fail_count + 1;
                    end else begin
                        $display("  PASS: bb[%0d] key=%h hit=%0b value=%h",
                            bb_idx, bb_keys[bb_idx],
                            cap_hits[bb_base + bb_idx], cap_values[bb_base + bb_idx]);
                    end
                end
            end
        end

        repeat (2) @(posedge clk); #1;

        test_num = 9;
        $display("\n=== TEST %0d: Insert after delete (reuse slot) ===", test_num);

        do_insert(32'hFACE_F00D, 64'h1234_5678_9ABC_DEF0, ins_ok);
        if (!ins_ok) begin
            $display("  FAIL: insert after delete unsuccessful");
            fail_count = fail_count + 1;
        end

        check_lookup(32'hFACE_F00D, 1'b1, 64'h1234_5678_9ABC_DEF0);

        check_lookup(test_keys[2], 1'b0, '0);

        repeat (2) @(posedge clk); #1;

        test_num = 10;
        $display("\n=== TEST %0d: Bulk insert (32 keys) ===", test_num);
        begin
            integer bulk_fails;
            logic   bulk_ins_ok;
            bulk_fails = 0;

            for (i = 0; i < 32; i = i + 1) begin
                do_insert(
                    32'hA000_0000 + i,
                    {32'hB00C_0000, 32'(i)},
                    bulk_ins_ok
                );
                if (!bulk_ins_ok) begin
                    $display("  WARN: bulk insert %0d failed (eviction chain full)", i);
                    bulk_fails = bulk_fails + 1;
                end
            end

            $display("  Inserted %0d / 32 keys (%0d eviction failures)",
                32 - bulk_fails, bulk_fails);

            for (i = 0; i < 32; i = i + 1) begin
                do_lookup(32'hA000_0000 + i, lu_hit, lu_val);
                if (lu_hit) begin
                    if (lu_val !== {32'hB00C_0000, 32'(i)}) begin
                        $display("  FAIL: bulk key %0d value mismatch %h", i, lu_val);
                        fail_count = fail_count + 1;
                    end
                end

            end
            $display("  Bulk lookup verification complete");
        end

        repeat (4) @(posedge clk); #1;

        $display("\n========================================");
        if (fail_count == 0)
            $display("ALL TESTS PASSED (%0d tests)", test_num);
        else
            $display("%0d FAILURE(S) across %0d tests", fail_count, test_num);
        $display("========================================\n");
        $finish;
    end

    initial begin
        #100_000;
        $display("\nFATAL: Simulation watchdog timeout");
        $finish;
    end

endmodule
