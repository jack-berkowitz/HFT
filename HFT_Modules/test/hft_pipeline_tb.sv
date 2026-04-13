`timescale 1ns/1ps
`include "sys_defs.svh"

module hft_pipeline_tb;

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

    logic [63:0] rx_tdata;   logic [7:0] rx_tkeep;
    logic        rx_tvalid, rx_tlast, rx_tready;

    logic [63:0] f2f_tdata;  logic [7:0] f2f_tkeep;
    logic        f2f_tvalid, f2f_tlast, f2f_tready;

    logic [63:0] f2d_tdata;  logic [7:0] f2d_tkeep;
    logic        f2d_tvalid, f2d_tlast, f2d_tready;

    pillar_msg_t decoded;

    order_lookup_out_t ol_out;
    logic              ol_ready;

    tob_out_t tob;

    trade_signal_t trade;
    logic [AW-1:0] idx_value;
    logic          wt_wr_en;
    logic [8:0]    wt_wr_addr;
    logic [31:0]   wt_wr_data;
    logic [AW-1:0] threshold;

    logic [63:0] tx_tdata;  logic [7:0] tx_tkeep;
    logic        tx_tvalid, tx_tlast;
    logic [31:0] order_count;
    logic signed [31:0] net_position;
    logic        pkt_sent;

    ipv4_udp_port_filter #(.FILTER_PORT(XDP_PORT)) u_filter (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(rx_tdata),   .s_axis_tkeep(rx_tkeep),
        .s_axis_tvalid(rx_tvalid), .s_axis_tlast(rx_tlast),
        .s_axis_tready(rx_tready),
        .m_axis_tdata(f2f_tdata),   .m_axis_tkeep(f2f_tkeep),
        .m_axis_tvalid(f2f_tvalid), .m_axis_tlast(f2f_tlast),
        .m_axis_tready(f2f_tready)
    );

    xdp_packet_framer #(.HEADER_BYTES(`XDP_CHAIN_FRAMER_HDR)) u_framer (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(f2f_tdata),   .s_axis_tkeep(f2f_tkeep),
        .s_axis_tvalid(f2f_tvalid), .s_axis_tlast(f2f_tlast),
        .s_axis_tready(f2f_tready),
        .m_axis_tdata(f2d_tdata),   .m_axis_tkeep(f2d_tkeep),
        .m_axis_tvalid(f2d_tvalid), .m_axis_tlast(f2d_tlast),
        .m_axis_tready(f2d_tready)
    );

    xdp_msg_demux u_demux (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(f2d_tdata),   .s_axis_tkeep(f2d_tkeep),
        .s_axis_tvalid(f2d_tvalid), .s_axis_tlast(f2d_tlast),
        .s_axis_tready(f2d_tready),
        .out_msg(decoded)
    );

    order_lookup #(
        .TBL_ENTRIES(TB_TBL_ENTRIES)
    ) u_order_lookup (
        .clk(clk), .rst_n(rst_n),
        .ready(ol_ready),
        .msg_in(decoded),
        .out(ol_out)
    );

    top_of_book #(
        .N_SYMBOLS(TB_N_SYMBOLS)
    ) u_tob (
        .clk(clk), .rst_n(rst_n),
        .in_update(ol_out),
        .out_tob(tob)
    );

    index_arb_engine #(
        .N_COMPONENTS(TB_N_COMP),
        .IDX_SYM(TB_IDX)
    ) u_index (
        .clk(clk), .rst_n(rst_n),
        .in_tob(tob),
        .wt_wr_en(wt_wr_en),
        .wt_wr_addr(wt_wr_addr),
        .wt_wr_data(wt_wr_data),
        .threshold(threshold),
        .out_trade(trade),
        .out_index(idx_value)
    );

    order_gen_tx #(
        .ORDER_QTY    (TB_QTY),
        .COOLDOWN_CYC (TB_COOLDOWN),
        .MAX_POS      (32'd10000),
        .PRICE_OFFSET (TB_OFFSET),
        .IDX_SYM_ID   (32'd4)
    ) u_tx (
        .clk(clk), .rst_n(rst_n),
        .enable(1'b1),
        .in_trade(trade),
        .m_axis_tdata(tx_tdata),   .m_axis_tkeep(tx_tkeep),
        .m_axis_tvalid(tx_tvalid), .m_axis_tlast(tx_tlast),
        .m_axis_tready(1'b1),
        .order_count(order_count),
        .net_position(net_position),
        .pkt_sent(pkt_sent)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    initial begin
        $dumpfile("hft_pipeline_dump.vcd");
        $dumpvars(0, hft_pipeline_tb);
    end

    logic [PKT_BITS-1:0] pkt_buf;
    integer pkt_len;

    task pkt_wr_byte(input integer idx, input [7:0] val);
        pkt_buf[idx*8 +: 8] = val;
    endtask
    task pkt_wr_u16_be(input integer idx, input [15:0] val);
        pkt_buf[idx*8 +: 8] = val[15:8]; pkt_buf[(idx+1)*8 +: 8] = val[7:0];
    endtask
    task pkt_wr_u16_le(input integer idx, input [15:0] val);
        pkt_buf[idx*8 +: 8] = val[7:0]; pkt_buf[(idx+1)*8 +: 8] = val[15:8];
    endtask
    task pkt_wr_u32_le(input integer idx, input [31:0] val);
        pkt_buf[idx*8 +: 8] = val[7:0];     pkt_buf[(idx+1)*8 +: 8] = val[15:8];
        pkt_buf[(idx+2)*8 +: 8] = val[23:16]; pkt_buf[(idx+3)*8 +: 8] = val[31:24];
    endtask
    task pkt_wr_u64_le(input integer idx, input [63:0] val);
        for (int kk = 0; kk < 8; kk++) pkt_buf[(idx+kk)*8 +: 8] = val[kk*8 +: 8];
    endtask

    task build_net_hdr(input integer udp_payload_len);
        integer ip_len, udp_len;
        ip_len  = 20 + 8 + udp_payload_len;
        udp_len =      8 + udp_payload_len;
        pkt_wr_byte(0,8'h01); pkt_wr_byte(1,8'h00); pkt_wr_byte(2,8'h5e);
        pkt_wr_byte(3,8'h00); pkt_wr_byte(4,8'h00); pkt_wr_byte(5,8'h01);
        pkt_wr_byte(6,8'hde); pkt_wr_byte(7,8'had); pkt_wr_byte(8,8'hbe);
        pkt_wr_byte(9,8'hef); pkt_wr_byte(10,8'hca); pkt_wr_byte(11,8'hfe);
        pkt_wr_byte(12,8'h08); pkt_wr_byte(13,8'h00);
        pkt_wr_byte(14,8'h45); pkt_wr_byte(15,8'h00);
        pkt_wr_byte(16, ip_len[15:8]); pkt_wr_byte(17, ip_len[7:0]);
        pkt_wr_byte(18,8'h00); pkt_wr_byte(19,8'h00);
        pkt_wr_byte(20,8'h00); pkt_wr_byte(21,8'h00);
        pkt_wr_byte(22,8'h40); pkt_wr_byte(23,8'h11);
        pkt_wr_byte(24,8'h00); pkt_wr_byte(25,8'h00);
        pkt_wr_byte(26,8'hef); pkt_wr_byte(27,8'h01);
        pkt_wr_byte(28,8'h01); pkt_wr_byte(29,8'h01);
        pkt_wr_byte(30,8'hef); pkt_wr_byte(31,8'h01);
        pkt_wr_byte(32,8'h01); pkt_wr_byte(33,8'h01);
        pkt_wr_byte(34,8'hCA); pkt_wr_byte(35,8'hFE);
        pkt_wr_u16_be(36, XDP_PORT);
        pkt_wr_byte(38, udp_len[15:8]); pkt_wr_byte(39, udp_len[7:0]);
        pkt_wr_byte(40,8'h00); pkt_wr_byte(41,8'h00);
    endtask

    task build_xdp_hdr(input [15:0] sz, input [7:0] nm, input [31:0] seq);
        pkt_wr_u16_le(42, sz); pkt_wr_byte(44, 8'd11); pkt_wr_byte(45, nm);
        pkt_wr_u32_le(46, seq); pkt_wr_u32_le(50, 32'h5F3759DF);
        pkt_wr_u32_le(54, 32'h00001234);
    endtask

    task build_msg_common(input [15:0] sz, input [15:0] typ,
                          input [31:0] sym, input [31:0] ssn, input [63:0] oid);
        pkt_wr_u16_le(58, sz); pkt_wr_u16_le(60, typ); pkt_wr_u32_le(62, 32'hAAAA_BBBB);
        pkt_wr_u32_le(66, sym); pkt_wr_u32_le(70, ssn); pkt_wr_u64_le(74, oid);
    endtask

    task build_add_order(input [31:0] seq, input [63:0] oid, input [31:0] price,
                         input [31:0] qty, input [31:0] sym, input [7:0] side);
        localparam SZ = 39;
        pkt_buf = '0; build_net_hdr(16+SZ); build_xdp_hdr(16+SZ, 1, seq);
        build_msg_common(SZ, 16'd100, sym, 32'd1, oid);
        pkt_wr_u32_le(82, price); pkt_wr_u32_le(86, qty); pkt_wr_byte(90, side);
        pkt_wr_byte(91,"A"); pkt_wr_byte(92,"B"); pkt_wr_byte(93,"C");
        pkt_wr_byte(94,"D"); pkt_wr_byte(95,"E"); pkt_wr_byte(96, 0);
        pkt_len = 42+16+SZ;
    endtask

    task build_mod_order(input [31:0] seq, input [63:0] oid, input [31:0] price,
                         input [31:0] qty, input [31:0] sym);
        localparam SZ = 35;
        pkt_buf = '0; build_net_hdr(16+SZ); build_xdp_hdr(16+SZ, 1, seq);
        build_msg_common(SZ, 16'd101, sym, 32'd2, oid);
        pkt_wr_u32_le(82, price); pkt_wr_u32_le(86, qty);
        pkt_wr_byte(90,0); pkt_wr_byte(91,0); pkt_wr_byte(92,0);
        pkt_len = 42+16+SZ;
    endtask

    task axis_send(input integer idle_cycles);
        integer bt, bk, bi, bthis, beats_total, remainder;
        logic [63:0] bd; logic [7:0] bkp;
        beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;
        remainder   = pkt_len % KEEP_W;
        for (bt = 0; bt < beats_total; bt++) begin
            if (idle_cycles > 0 && bt > 0) begin
                rx_tvalid = 0; repeat(idle_cycles) @(posedge clk); #1;
            end
            bd = '0; bkp = '0; bi = bt * KEEP_W;
            bthis = (bt == beats_total-1 && remainder != 0) ? remainder : KEEP_W;
            for (bk = 0; bk < bthis; bk++) begin
                bd[bk*8 +: 8] = pkt_buf[(bi+bk)*8 +: 8]; bkp[bk] = 1'b1;
            end
            rx_tdata = bd; rx_tkeep = bkp;
            rx_tvalid = 1; rx_tlast = (bt == beats_total-1);
            @(posedge clk); while (!rx_tready) @(posedge clk); #1;
        end
        rx_tvalid = 0; rx_tlast = 0; rx_tkeep = '0; rx_tdata = '0;
    endtask

    task send_and_settle;
        axis_send(0);
        repeat(60) @(posedge clk); #1;
    endtask

    logic [7:0]  cap_buf [0:79];
    integer      cap_bytes;
    logic        cap_done;

    always @(posedge clk) begin
        if (tx_tvalid) begin
            for (int b = 0; b < 8; b++)
                if (tx_tkeep[b]) begin
                    cap_buf[cap_bytes] = tx_tdata[b*8 +: 8];
                    cap_bytes = cap_bytes + 1;
                end
            if (tx_tlast) cap_done = 1'b1;
        end
    end

    task reset_cap;
        for (int i = 0; i < 80; i++) cap_buf[i] = 8'd0;
        cap_bytes = 0;
        cap_done  = 1'b0;
    endtask

    function automatic [31:0] cap_u32_le(input integer off);
        return {cap_buf[off+3], cap_buf[off+2], cap_buf[off+1], cap_buf[off]};
    endfunction

    logic        trade_seen;
    logic        trade_dir_cap;
    logic signed [AW-1:0] trade_spread_cap;
    logic [31:0] trade_actual_cap;

    always @(posedge clk) begin
        if (trade.valid && !trade_seen) begin
            trade_seen       = 1'b1;
            trade_dir_cap    = trade.direction;
            trade_spread_cap = trade.spread;
            trade_actual_cap = trade.actual_price;
        end
    end

    task reset_trade_cap;
        trade_seen       = 1'b0;
        trade_dir_cap    = 1'b0;
        trade_spread_cap = '0;
        trade_actual_cap = '0;
    endtask

    task load_weight(input [8:0] addr, input [31:0] w);
        wt_wr_en   = 1'b1;
        wt_wr_addr = addr;
        wt_wr_data = w;
        @(posedge clk); #1;
        wt_wr_en = 1'b0;
    endtask

    integer fail_count;

    initial begin
        integer t;
        fail_count = 0;

        rx_tdata = '0; rx_tkeep = '0; rx_tvalid = 0; rx_tlast = 0;
        pkt_buf = '0; pkt_len = 0;
        wt_wr_en = 1'b0; wt_wr_addr = '0; wt_wr_data = '0;
        threshold = '0;
        reset_cap();
        reset_trade_cap();

        $display("");
        $display("  ╔═════════════════════════════════════════════════════════════╗");
        $display("  ║  HFT Pipeline Full Integration Test                        ║");
        $display("  ║  filter → framer → demux → order_lookup → top_of_book     ║");
        $display("  ║          → index_arb_engine → order_gen_tx                 ║");
        $display("  ╚═════════════════════════════════════════════════════════════╝");

        $display("\n--- PHASE 1: System initialization ---");

        rst_n = 0;
        repeat(6) @(posedge clk); #1;
        rst_n = 1;

        t = 0;
        while (!ol_ready && t < 500) begin @(posedge clk); #1; t++; end
        if (t >= 500) begin
            $display("    FAIL: order_lookup never became ready");
            fail_count++;
        end else begin
            $display("    Hash table initialized, order_lookup ready (%0d cycles)", t);
        end

        for (int i = 0; i < TB_N_COMP; i++)
            load_weight(i[8:0], WEIGHT_Q);
        $display("    Loaded %0d weights = %0d (0.25 in Q12.20)", TB_N_COMP, WEIGHT_Q);

        threshold = THRESH;
        $display("    Threshold = %0d (100 cents in Q44.20)", THRESH);

        repeat(4) @(posedge clk); #1;

        $display("\n--- PHASE 2: Build order book (10 Add Orders) ---");

        build_add_order(32'd1, 64'd1, 32'd10000, TB_QTY, 32'd0, "B");
        send_and_settle();
        $display("    [1]  sym=0 BUY  price=10000");

        build_add_order(32'd2, 64'd2, 32'd10100, TB_QTY, 32'd0, "S");
        send_and_settle();
        $display("    [2]  sym=0 SELL price=10100  → mid=10050");

        build_add_order(32'd3, 64'd3, 32'd20000, TB_QTY, 32'd1, "B");
        send_and_settle();
        $display("    [3]  sym=1 BUY  price=20000");

        build_add_order(32'd4, 64'd4, 32'd20100, TB_QTY, 32'd1, "S");
        send_and_settle();
        $display("    [4]  sym=1 SELL price=20100  → mid=20050");

        build_add_order(32'd5, 64'd5, 32'd30000, TB_QTY, 32'd2, "B");
        send_and_settle();
        $display("    [5]  sym=2 BUY  price=30000");

        build_add_order(32'd6, 64'd6, 32'd30100, TB_QTY, 32'd2, "S");
        send_and_settle();
        $display("    [6]  sym=2 SELL price=30100  → mid=30050");

        build_add_order(32'd7, 64'd7, 32'd40000, TB_QTY, 32'd3, "B");
        send_and_settle();
        $display("    [7]  sym=3 BUY  price=40000");

        build_add_order(32'd8, 64'd8, 32'd40100, TB_QTY, 32'd3, "S");
        send_and_settle();
        $display("    [8]  sym=3 SELL price=40100  → mid=40050");

        build_add_order(32'd9, 64'd9, 32'd25000, TB_QTY, 32'd4, "B");
        send_and_settle();
        $display("    [9]  sym=4 BUY  price=25000  (index bid)");

        build_add_order(32'd10, 64'd10, 32'd25100, TB_QTY, 32'd4, "S");
        send_and_settle();
        $display("    [10] sym=4 SELL price=25100  (index ask, mid=25050)");

        $display("");
        $display("    ── Post-initialization ──");
        $display("    Index accum:  %0d", $signed(u_index.index_accum));
        $display("    Actual mid:   %0d", u_index.actual_mid);
        $display("    Trade seen:   %s", trade_seen ? "YES (BAD)" : "no (correct)");
        $display("    Orders sent:  %0d", order_count);

        if (trade_seen) begin
            fail_count++;
            $display("    FAIL: unexpected trade during book build");
        end
        if (order_count != 0) begin
            fail_count++;
            $display("    FAIL: orders sent during init (%0d)", order_count);
        end
        if (u_index.actual_mid !== 32'd25050) begin
            fail_count++;
            $display("    FAIL: actual_mid expected 25050, got %0d", u_index.actual_mid);
        end

        $display("\n--- PHASE 3: Trigger trade (Modify sym=0 bid → 12000) ---");

        reset_cap();
        reset_trade_cap();

        build_mod_order(32'd11, 64'd1, 32'd12000, TB_QTY, 32'd0);
        axis_send(0);

        t = 0;
        while (!pkt_sent && t < 300) begin @(posedge clk); #1; t++; end

        if (t >= 300) begin
            $display("    FAIL: no output packet generated within 300 cycles");
            fail_count++;
        end else begin
            $display("    Output packet sent (%0d cycles after modify)", t);
        end

        repeat(5) @(posedge clk); #1;

        $display("");
        $display("    ── Trade signal ──");
        if (!trade_seen) begin
            $display("    FAIL: no trade signal captured");
            fail_count++;
        end else begin
            $display("    Direction:    %s", trade_dir_cap ? "SELL" : "BUY");
            $display("    Actual price: %0d", trade_actual_cap);
            $display("    Spread (Q44.20): %0d", $signed(trade_spread_cap));
            $display("    Spread (cents):  ~%0d",
                     $signed(trade_spread_cap) / $signed(64'd1048576));

            if (trade_dir_cap !== 1'b1) begin
                fail_count++;
                $display("    FAIL: expected SELL (1), got BUY (0)");
            end
            if (trade_actual_cap !== 32'd25050) begin
                fail_count++;
                $display("    FAIL: expected actual_price=25050");
            end
        end

        $display("");
        $display("    ── Output packet (%0d bytes) ──", cap_bytes);

        if (cap_bytes !== 74) begin
            fail_count++;
            $display("    FAIL: expected 74 bytes, got %0d", cap_bytes);
        end else begin
            $display("    EtherType:  0x%02h%02h %s",
                     cap_buf[12], cap_buf[13],
                     ({cap_buf[12], cap_buf[13]} == 16'h0800) ? "(OK)" : "(BAD)");
            $display("    IP proto:   0x%02h %s",
                     cap_buf[23],
                     (cap_buf[23] == 8'h11) ? "(UDP, OK)" : "(BAD)");
            $display("    Side:       %s (0x%02h)",
                     (cap_buf[44] == 8'h53) ? "SELL" : "BUY", cap_buf[44]);
            $display("    Symbol:     %0d", cap_u32_le(46));
            $display("    Price:      %0d", cap_u32_le(50));
            $display("    Qty:        %0d", cap_u32_le(54));

            if ({cap_buf[12], cap_buf[13]} !== 16'h0800) begin
                fail_count++; $display("    FAIL: ethertype");
            end
            if (cap_buf[23] !== 8'h11) begin
                fail_count++; $display("    FAIL: ip proto");
            end
            if (cap_buf[44] !== 8'h53) begin
                fail_count++; $display("    FAIL: expected side 'S' (0x53)");
            end
            if (cap_u32_le(46) !== 32'd4) begin
                fail_count++; $display("    FAIL: expected symbol=4");
            end
            if (cap_u32_le(50) !== 32'd25040) begin
                fail_count++;
                $display("    FAIL: expected price=25040 (25050-10), got %0d",
                         cap_u32_le(50));
            end
            if (cap_u32_le(54) !== TB_QTY) begin
                fail_count++; $display("    FAIL: expected qty=%0d", TB_QTY);
            end
        end

        if (order_count !== 32'd1) begin
            fail_count++;
            $display("    FAIL: order_count=%0d, expected 1", order_count);
        end
        if (net_position !== -32'sd100) begin
            fail_count++;
            $display("    FAIL: net_position=%0d, expected -100 (SELL)", net_position);
        end

        repeat(TB_COOLDOWN + 5) @(posedge clk); #1;

        $display("\n--- PHASE 4: Wrong UDP port → expect drop ---");

        reset_cap();
        reset_trade_cap();

        build_add_order(32'd20, 64'd99, 32'd50000, TB_QTY, 32'd0, "B");
        pkt_wr_byte(36, 8'hDE); pkt_wr_byte(37, 8'hAD);

        axis_send(0);
        repeat(60) @(posedge clk); #1;

        if (cap_bytes != 0) begin
            fail_count++;
            $display("    FAIL: output packet generated for dropped input");
        end else begin
            $display("    Correctly dropped (no pipeline output)");
        end

        if (trade_seen) begin
            fail_count++;
            $display("    FAIL: trade signal fired for dropped packet");
        end

        $display("\n--- PHASE 5: Pipeline recovery after drop ---");

        build_add_order(32'd21, 64'd20, 32'd15000, 32'd50, 32'd0, "B");
        axis_send(0);

        t = 0;
        while (!ol_out.valid && t < 300) begin @(posedge clk); #1; t++; end

        if (t >= 300) begin
            fail_count++;
            $display("    FAIL: pipeline did not recover after drop");
        end else begin
            $display("    Pipeline recovered, message processed (%0d cycles)", t);
        end

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

    initial begin
        #1_000_000;
        $display("\nFATAL: Simulation watchdog timeout");
        $finish;
    end

endmodule
