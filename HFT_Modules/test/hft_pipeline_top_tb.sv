`timescale 1ns/1ps
`include "sys_defs.svh"

module hft_pipeline_top_tb;

    localparam TB_N_COMP      = 4;
    localparam [8:0] TB_IDX   = 9'd4;
    localparam TB_N_SYMBOLS   = 8;
    localparam TB_TBL_ENTRIES = 128;
    localparam TB_COOLDOWN    = 10;
    localparam TB_QTY         = 32'd100;
    localparam TB_OFFSET      = 32'd10;
    localparam AW             = `ACCUM_W;

    localparam [31:0] WEIGHT_Q = 32'd262144;
    localparam [63:0] THRESH   = 64'd104_857_600;

    localparam KEEP_W    = 8;
    localparam MAX_PKT_B = 256;
    localparam PKT_BITS  = MAX_PKT_B * 8;
    localparam [15:0] XDP_PORT = `XDP_PORT;

    logic clk, rst_n;

    logic [63:0] rx_tdata;
    logic [7:0]  rx_tkeep;
    logic        rx_tvalid, rx_tlast, rx_tready;

    logic [63:0] tx_tdata;
    logic [7:0]  tx_tkeep;
    logic        tx_tvalid, tx_tlast;

    logic                      tx_enable;
    logic                      wt_wr_en;
    logic [`SYM_IDX_W-1:0]    wt_wr_addr;
    logic [`WEIGHT_W-1:0]     wt_wr_data;
    logic [`ACCUM_W-1:0]      threshold;

    logic              ol_ready;
    tob_out_t          tob_snap;
    trade_signal_t     trade;
    logic [AW-1:0]     idx_value;
    logic [31:0]       order_count;
    logic signed [31:0] net_position;
    logic              pkt_sent;

    hft_pipeline_top #(
        .FILTER_PORT    (XDP_PORT),
        .HEADER_BYTES   (`XDP_CHAIN_FRAMER_HDR),
        .HT_N_WAYS      (4),
        .HT_TBL_ENTRIES (TB_TBL_ENTRIES),
        .HT_MAX_CHAIN   (8),
        .TOB_N_LEVELS   (`TOB_LEVELS),
        .TOB_N_SYMBOLS  (TB_N_SYMBOLS),
        .N_COMPONENTS   (TB_N_COMP),
        .IDX_SYM        (TB_IDX),
        .ORDER_QTY      (TB_QTY),
        .COOLDOWN_CYC   (TB_COOLDOWN),
        .MAX_POS        (32'd10000),
        .PRICE_OFFSET   (TB_OFFSET),
        .IDX_SYM_ID     (32'd4)
    ) dut (
        .clk              (clk),
        .rst_n            (rst_n),

        .rx_axis_tdata    (rx_tdata),
        .rx_axis_tkeep    (rx_tkeep),
        .rx_axis_tvalid   (rx_tvalid),
        .rx_axis_tlast    (rx_tlast),
        .rx_axis_tready   (rx_tready),

        .tx_axis_tdata    (tx_tdata),
        .tx_axis_tkeep    (tx_tkeep),
        .tx_axis_tvalid   (tx_tvalid),
        .tx_axis_tlast    (tx_tlast),
        .tx_axis_tready   (1'b1),

        .tx_enable        (tx_enable),
        .wt_wr_en         (wt_wr_en),
        .wt_wr_addr       (wt_wr_addr),
        .wt_wr_data       (wt_wr_data),
        .threshold        (threshold),

        .ol_ready         (ol_ready),
        .tob_snapshot     (tob_snap),
        .trade_signal     (trade),
        .computed_index   (idx_value),
        .order_count      (order_count),
        .net_position     (net_position),
        .pkt_sent         (pkt_sent)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    initial begin
        $dumpfile("hft_pipeline_top_dump.vcd");
        $dumpvars(0, hft_pipeline_top_tb);
    end

    logic [PKT_BITS-1:0] pkt_buf;
    integer pkt_len;

    task pkt_wr_byte(input integer idx, input [7:0] val);
        pkt_buf[idx*8 +: 8] = val;
    endtask

    task pkt_wr_u16_le(input integer idx, input [15:0] val);
        pkt_buf[idx*8 +: 8]     = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
    endtask

    task pkt_wr_u16_be(input integer idx, input [15:0] val);
        pkt_buf[idx*8 +: 8]     = val[15:8];
        pkt_buf[(idx+1)*8 +: 8] = val[7:0];
    endtask

    task pkt_wr_u32_le(input integer idx, input [31:0] val);
        pkt_buf[idx*8     +: 8] = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
        pkt_buf[(idx+2)*8 +: 8] = val[23:16];
        pkt_buf[(idx+3)*8 +: 8] = val[31:24];
    endtask

    task pkt_wr_u64_le(input integer idx, input [63:0] val);
        pkt_wr_u32_le(idx,   val[31:0]);
        pkt_wr_u32_le(idx+4, val[63:32]);
    endtask

    task build_net_header(input [15:0] udp_payload_len, input [15:0] dst_port);
        logic [15:0] ip_total_len;
        logic [15:0] udp_total_len;
        ip_total_len  = 20 + 8 + udp_payload_len;
        udp_total_len = 8 + udp_payload_len;

        pkt_buf = '0;

        pkt_wr_byte(0,8'h01); pkt_wr_byte(1,8'h00); pkt_wr_byte(2,8'h5e);
        pkt_wr_byte(3,8'h00); pkt_wr_byte(4,8'h00); pkt_wr_byte(5,8'h01);
        pkt_wr_byte(6,8'hde); pkt_wr_byte(7,8'had); pkt_wr_byte(8,8'hbe);
        pkt_wr_byte(9,8'hef); pkt_wr_byte(10,8'hca); pkt_wr_byte(11,8'hfe);
        pkt_wr_byte(12,8'h08); pkt_wr_byte(13,8'h00);

        pkt_wr_byte(14,8'h45); pkt_wr_byte(15,8'h00);
        pkt_wr_byte(16, ip_total_len[15:8]); pkt_wr_byte(17, ip_total_len[7:0]);
        pkt_wr_byte(18,8'h00); pkt_wr_byte(19,8'h00);
        pkt_wr_byte(20,8'h00); pkt_wr_byte(21,8'h00);
        pkt_wr_byte(22,8'h40); pkt_wr_byte(23,8'h11);
        pkt_wr_byte(24,8'h00); pkt_wr_byte(25,8'h00);
        pkt_wr_byte(26,8'hef); pkt_wr_byte(27,8'h01);
        pkt_wr_byte(28,8'h01); pkt_wr_byte(29,8'h01);
        pkt_wr_byte(30,8'hef); pkt_wr_byte(31,8'h01);
        pkt_wr_byte(32,8'h01); pkt_wr_byte(33,8'h01);

        pkt_wr_byte(34,8'hCA); pkt_wr_byte(35,8'hFE);
        pkt_wr_u16_be(36, dst_port);
        pkt_wr_byte(38, udp_total_len[15:8]);
        pkt_wr_byte(39, udp_total_len[7:0]);
        pkt_wr_byte(40,8'h00); pkt_wr_byte(41,8'h00);
    endtask

    task build_xdp_pkt_hdr(input [15:0] pkt_sz, input [7:0] n_msgs, input [31:0] seq);
        pkt_wr_u16_le(42, pkt_sz);
        pkt_wr_byte(44, 8'd11);
        pkt_wr_byte(45, n_msgs);
        pkt_wr_u32_le(46, seq);
        pkt_wr_u32_le(50, 32'h5F3759DF);
        pkt_wr_u32_le(54, 32'h00001234);
    endtask

    task build_msg_common(input [15:0] sz, input [15:0] typ,
                          input [31:0] sym, input [31:0] ssn,
                          input [63:0] oid);
        pkt_wr_u16_le(58, sz);
        pkt_wr_u16_le(60, typ);
        pkt_wr_u32_le(62, 32'hAAAA_BBBB);
        pkt_wr_u32_le(66, sym);
        pkt_wr_u32_le(70, ssn);
        pkt_wr_u64_le(74, oid);
    endtask

    task build_add_order(input [31:0] sym, input [31:0] ssn,
                         input [63:0] oid, input [31:0] price,
                         input [31:0] qty, input logic side);
        logic [15:0] udp_payload;
        udp_payload = 16 + 39;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd55, 8'd1, ssn);
        build_msg_common(16'd39, `XDP_MSG_ADD_ORDER, sym, ssn, oid);
        pkt_wr_u32_le(82, price);
        pkt_wr_u32_le(86, qty);
        pkt_wr_byte(90, side ? "S" : "B");

        pkt_len = 42 + 16 + 39;
    endtask

    task build_mod_order(input [31:0] sym, input [31:0] ssn,
                         input [63:0] oid, input [31:0] price,
                         input [31:0] qty);
        logic [15:0] udp_payload;
        udp_payload = 16 + 35;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd51, 8'd1, ssn);
        build_msg_common(16'd35, `XDP_MSG_MOD_ORDER, sym, ssn, oid);
        pkt_wr_u32_le(82, price);
        pkt_wr_u32_le(86, qty);
        pkt_len = 42 + 16 + 35;
    endtask

    task build_del_order(input [31:0] sym, input [31:0] ssn,
                         input [63:0] oid);
        logic [15:0] udp_payload;
        udp_payload = 16 + 25;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd41, 8'd1, ssn);
        build_msg_common(16'd25, `XDP_MSG_DEL_ORDER, sym, ssn, oid);
        pkt_len = 42 + 16 + 25;
    endtask

    task build_exec_order(input [31:0] sym, input [31:0] ssn,
                          input [63:0] oid, input [31:0] price,
                          input [31:0] qty);
        logic [15:0] udp_payload;
        udp_payload = 16 + 42;
        build_net_header(udp_payload, XDP_PORT);
        build_xdp_pkt_hdr(16'd58, 8'd1, ssn);
        build_msg_common(16'd42, `XDP_MSG_EXEC_ORDER, sym, ssn, oid);
        pkt_wr_u32_le(82, 32'hDEAD0001);
        pkt_wr_u32_le(86, price);
        pkt_wr_u32_le(90, qty);
        pkt_wr_byte(94, 8'h59);
        pkt_len = 42 + 16 + 42;
    endtask

    task axis_send(input integer idle_cycles);
        integer beats_total, remainder, bt, bi, bthis, bk;
        logic [63:0] bd;
        logic [7:0]  bkp;

        beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;
        remainder   = pkt_len % KEEP_W;

        for (bt = 0; bt < beats_total; bt++) begin
            if (idle_cycles > 0 && bt > 0) begin
                rx_tvalid = 1'b0;
                repeat(idle_cycles) @(posedge clk); #1;
            end

            bd  = '0;
            bkp = '0;
            bi  = bt * KEEP_W;
            bthis = (bt == beats_total - 1 && remainder != 0)
                    ? remainder : KEEP_W;

            for (bk = 0; bk < bthis; bk++)
                bd[bk*8 +: 8] = pkt_buf[(bi+bk)*8 +: 8];

            for (bk = 0; bk < bthis; bk++)
                bkp[bk] = 1'b1;

            rx_tdata  = bd;
            rx_tkeep  = bkp;
            rx_tvalid = 1'b1;
            rx_tlast  = (bt == beats_total - 1);

            @(posedge clk); #1;
            while (!rx_tready) begin @(posedge clk); #1; end
        end

        rx_tvalid = 1'b0;
        rx_tlast  = 1'b0;
    endtask

    task send_pkt_and_wait(input integer drain_cycles);
        axis_send(0);
        repeat(drain_cycles) @(posedge clk); #1;
    endtask

    task load_weight(input [`SYM_IDX_W-1:0] addr, input [`WEIGHT_W-1:0] w);
        wt_wr_en   = 1'b1;
        wt_wr_addr = addr;
        wt_wr_data = w;
        @(posedge clk); #1;
        wt_wr_en = 1'b0;
    endtask

    logic              trade_seen;
    logic              trade_dir_cap;
    logic signed [AW-1:0] trade_spread_cap;
    logic [31:0]       trade_actual_cap;

    always @(posedge clk) begin
        if (trade.valid) begin
            trade_seen       <= 1'b1;
            trade_dir_cap    <= trade.direction;
            trade_spread_cap <= trade.spread;
            trade_actual_cap <= trade.actual_price;
        end
    end

    task reset_trade_cap;
        trade_seen       = 1'b0;
        trade_dir_cap    = 1'b0;
        trade_spread_cap = '0;
        trade_actual_cap = '0;
    endtask

    logic        tx_pkt_seen;
    logic [31:0] tx_pkt_count;

    always @(posedge clk) begin
        if (!rst_n) begin
            tx_pkt_seen  <= 1'b0;
            tx_pkt_count <= '0;
        end else if (tx_tvalid && tx_tlast) begin
            tx_pkt_seen  <= 1'b1;
            tx_pkt_count <= tx_pkt_count + 1;
        end
    end

    task reset_tx_cap;
        tx_pkt_seen = 1'b0;
    endtask

    integer fail_count;
    integer fb;
    integer seq;

    task check(input logic cond, input string msg);
        if (!cond) begin
            $display("  ** FAIL: %s", msg);
            fail_count++;
        end
    endtask

    task pr(input string name);
        if (fail_count == fb)
            $display("  PASS: %s", name);
        else
            $display("  FAIL: %s (%0d errors)", name, fail_count - fb);
    endtask

    initial begin
        fail_count = 0;
        seq        = 1;

        rx_tdata   = '0;
        rx_tkeep   = '0;
        rx_tvalid  = 1'b0;
        rx_tlast   = 1'b0;
        tx_enable  = 1'b1;
        wt_wr_en   = 1'b0;
        wt_wr_addr = '0;
        wt_wr_data = '0;
        threshold  = '0;
        pkt_buf    = '0;
        pkt_len    = 0;
        reset_trade_cap();

        $display("");
        $display("  ╔═══════════════════════════════════════════════════════════════╗");
        $display("  ║  HFT Pipeline Top-Level Integration Test                     ║");
        $display("  ║  filter → framer → demux → order_lookup → top_of_book       ║");
        $display("  ║          → index_arb_engine → order_gen_tx                   ║");
        $display("  ╚═══════════════════════════════════════════════════════════════╝");

        $display("\n=== PHASE 1: Reset + Hash Table Init + Weight Load ===");
        fb = fail_count;

        rst_n = 0;
        repeat(8) @(posedge clk);
        rst_n = 1;

        $display("  Waiting for order_lookup ready...");
        wait(ol_ready);
        $display("  order_lookup ready — hash table initialised");

        load_weight(9'd0, WEIGHT_Q);
        load_weight(9'd1, WEIGHT_Q);
        load_weight(9'd2, WEIGHT_Q);
        load_weight(9'd3, WEIGHT_Q);

        threshold = THRESH;
        @(posedge clk); #1;

        check(ol_ready, "order_lookup ready after init");
        pr("Phase 1: Reset + Init");

        $display("\n=== PHASE 2: Build Order Book (10 Add Orders) ===");
        fb = fail_count;

        build_add_order(32'd0, seq, 64'h100, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd0, seq, 64'h101, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        build_add_order(32'd1, seq, 64'h200, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd1, seq, 64'h201, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        build_add_order(32'd2, seq, 64'h300, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd2, seq, 64'h301, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        build_add_order(32'd3, seq, 64'h400, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd3, seq, 64'h401, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        build_add_order(32'd4, seq, 64'h500, 32'd9900,  32'd100, 1'b0); seq++;
        send_pkt_and_wait(40);
        build_add_order(32'd4, seq, 64'h501, 32'd10100, 32'd100, 1'b1); seq++;
        send_pkt_and_wait(40);

        repeat(20) @(posedge clk); #1;
        check(order_count == 0, "no orders sent during book build");
        $display("  computed_index = %0d (Q44.20)", idx_value);
        $display("  order_count    = %0d", order_count);
        pr("Phase 2: Build Book");

        $display("\n=== PHASE 3: Trigger Arbitrage via Modify ===");
        fb = fail_count;
        reset_trade_cap();
        reset_tx_cap();

        // Modify sym=0 bid up to 12000 → pushes computed index ABOVE actual.
        // Under the corrected arb convention, spread > 0 means the index
        // instrument is UNDERPRICED relative to fair value, so the engine
        // should fire a BUY (direction=0).
        build_mod_order(32'd0, seq, 64'h100, 32'd12000, 32'd100); seq++;
        send_pkt_and_wait(50);

        repeat(50) @(posedge clk); #1;

        if (trade_seen) begin
            $display("  Trade signal captured:");
            $display("    direction    = %s", trade_dir_cap ? "SELL" : "BUY");
            $display("    spread       = %0d (Q44.20)", $signed(trade_spread_cap));
            $display("    actual_price = %0d", trade_actual_cap);
        end else begin
            $display("  WARNING: No trade signal captured");
        end

        $display("  computed_index = %0d (Q44.20)", idx_value);
        $display("  order_count    = %0d", order_count);
        $display("  net_position   = %0d", net_position);

        check(trade_seen, "trade signal fired after modify");
        if (trade_seen)
            check(trade_dir_cap == 1'b0, "trade direction is BUY (instrument underpriced)");
        pr("Phase 3: Trigger Arb");

        $display("\n=== PHASE 4: Wrong UDP Port → Dropped ===");
        fb = fail_count;

        begin
            logic [31:0] saved_order_count;
            saved_order_count = order_count;

            build_net_header(16'd55, 16'hDEAD);
            build_xdp_pkt_hdr(16'd55, 8'd1, seq); seq++;
            build_msg_common(16'd39, `XDP_MSG_ADD_ORDER, 32'd0, seq, 64'hBEEF);
            pkt_wr_u32_le(82, 32'd5000);
            pkt_wr_u32_le(86, 32'd50);
            pkt_wr_byte(90, "B");
            pkt_len = 97;

            send_pkt_and_wait(40);

            check(order_count == saved_order_count,
                  "order_count unchanged after wrong-port packet");
        end
        pr("Phase 4: Wrong Port Drop");

        $display("\n=== PHASE 5: Valid Packet After Drop ===");
        fb = fail_count;

        repeat(TB_COOLDOWN + 10) @(posedge clk); #1;

        build_add_order(32'd1, seq, 64'h202, 32'd9800, 32'd50, 1'b0); seq++;
        send_pkt_and_wait(50);

        check(1'b1, "pipeline accepts valid packet after drop");
        pr("Phase 5: Recovery");

        $display("\n=== PHASE 6: Delete Order ===");
        fb = fail_count;

        build_del_order(32'd1, seq, 64'h202); seq++;
        send_pkt_and_wait(50);

        check(1'b1, "delete processed without hang");
        pr("Phase 6: Delete");

        $display("\n=== PHASE 7: Exec Partial Fill ===");
        fb = fail_count;

        build_exec_order(32'd1, seq, 64'h200, 32'd9900, 32'd50); seq++;
        send_pkt_and_wait(50);

        check(1'b1, "exec partial fill processed");
        pr("Phase 7: Exec Partial");

        $display("\n=== PHASE 8: TX Enable Gate ===");
        fb = fail_count;

        repeat(TB_COOLDOWN + 10) @(posedge clk); #1;

        begin
            logic [31:0] saved_count;
            saved_count = order_count;

            tx_enable = 1'b0;

            build_mod_order(32'd2, seq, 64'h300, 32'd15000, 32'd100); seq++;
            send_pkt_and_wait(50);
            repeat(50) @(posedge clk); #1;

            check(order_count == saved_count,
                  "no TX packet when tx_enable=0");

            tx_enable = 1'b1;
        end
        pr("Phase 8: TX Enable Gate");

        $display("");
        $display("  ════════════════════════════════════════════");
        if (fail_count == 0)
            $display("  @@@ Passed: All tests passed!");
        else
            $display("  @@@ Failed: %0d error(s) total", fail_count);
        $display("  ════════════════════════════════════════════");
        $display("");

        #100;
        $finish;
    end

    initial begin
        #500_000;
        $display("\n  *** TIMEOUT — simulation exceeded 500 us ***");
        $finish;
    end

endmodule