`timescale 1ns/1ps
`include "sys_defs.svh"

module order_gen_tx_tb;

    localparam [47:0] TB_SRC_MAC  = 48'hDE_AD_BE_EF_CA_FE;
    localparam [47:0] TB_DST_MAC  = 48'h01_00_5E_00_00_01;
    localparam [31:0] TB_SRC_IP   = {8'd10, 8'd0, 8'd0, 8'd1};
    localparam [31:0] TB_DST_IP   = {8'd10, 8'd0, 8'd0, 8'd2};
    localparam [15:0] TB_SRC_PORT = 16'd12345;
    localparam [15:0] TB_DST_PORT = 16'd54321;
    localparam [31:0] TB_QTY      = 32'd100;
    localparam [31:0] TB_COOLDOWN = 32'd20;
    localparam [31:0] TB_MAX_POS  = 32'd500;
    localparam [31:0] TB_OFFSET   = 32'd10;
    localparam [31:0] TB_IDX_SYM  = 32'd500;
    localparam        AW          = `ACCUM_W;

    logic              clk, rst_n, enable;
    trade_signal_t     trade_in;
    logic [63:0]       m_axis_tdata;
    logic [7:0]        m_axis_tkeep;
    logic              m_axis_tvalid, m_axis_tlast, m_axis_tready;
    logic [31:0]       order_count;
    logic signed [31:0] net_position;
    logic              pkt_sent;

    order_gen_tx #(
        .SRC_MAC      (TB_SRC_MAC),
        .DST_MAC      (TB_DST_MAC),
        .SRC_IP       (TB_SRC_IP),
        .DST_IP       (TB_DST_IP),
        .SRC_PORT     (TB_SRC_PORT),
        .DST_PORT     (TB_DST_PORT),
        .ORDER_QTY    (TB_QTY),
        .COOLDOWN_CYC (TB_COOLDOWN),
        .MAX_POS      (TB_MAX_POS),
        .PRICE_OFFSET (TB_OFFSET),
        .IDX_SYM_ID   (TB_IDX_SYM)
    ) dut (
        .clk          (clk),
        .rst_n        (rst_n),
        .enable       (enable),
        .in_trade     (trade_in),
        .m_axis_tdata (m_axis_tdata),
        .m_axis_tkeep (m_axis_tkeep),
        .m_axis_tvalid(m_axis_tvalid),
        .m_axis_tlast (m_axis_tlast),
        .m_axis_tready(m_axis_tready),
        .order_count  (order_count),
        .net_position (net_position),
        .pkt_sent     (pkt_sent)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    initial begin
        $dumpfile("order_gen_tx_dump.vcd");
        $dumpvars(0, order_gen_tx_tb);
    end

    logic [7:0]  cap_buf [0:79];
    integer      cap_bytes;
    integer      cap_beats;
    logic        cap_last;

    always @(posedge clk) begin
        if (m_axis_tvalid && m_axis_tready) begin
            for (int b = 0; b < 8; b++) begin
                if (m_axis_tkeep[b]) begin
                    cap_buf[cap_bytes] = m_axis_tdata[b*8 +: 8];
                    cap_bytes = cap_bytes + 1;
                end
            end
            cap_beats = cap_beats + 1;
            if (m_axis_tlast) cap_last = 1'b1;
        end
    end

    task reset_cap;
        for (int i = 0; i < 80; i++) cap_buf[i] = 8'd0;
        cap_bytes = 0;
        cap_beats = 0;
        cap_last  = 1'b0;
    endtask

    task automatic fire_trade(
        input logic      dir,
        input [31:0]     actual,
        input [63:0]     spread_bits
    );
        trade_in                = '0;
        trade_in.valid          = 1'b1;
        trade_in.direction      = dir;
        trade_in.actual_price   = actual;
        trade_in.spread         = spread_bits;
        trade_in.computed_index = '0;
        @(posedge clk); #1;
        trade_in = '0;
    endtask

    task automatic wait_pkt(output logic timed_out);
        integer t;
        timed_out = 0; t = 0;
        while (!cap_last && t < 200) begin @(posedge clk); #1; t++; end
        if (t >= 200) timed_out = 1;
        @(posedge clk); #1;
    endtask

    task wait_cd;
        repeat(TB_COOLDOWN + 5) @(posedge clk); #1;
    endtask

    function automatic [15:0] cap_u16_be(input integer off);
        return {cap_buf[off], cap_buf[off+1]};
    endfunction

    function automatic [31:0] cap_u32_le(input integer off);
        return {cap_buf[off+3], cap_buf[off+2], cap_buf[off+1], cap_buf[off]};
    endfunction

    function automatic [63:0] cap_u64_le(input integer off);
        logic [63:0] v;
        for (int i = 0; i < 8; i++) v[i*8 +: 8] = cap_buf[off+i];
        return v;
    endfunction

    integer fail_count, fb;

    task pr(input string name);
        if (fail_count == fb) $display("  PASS: %s", name);
        else $display("  FAIL: %s (%0d errors)", name, fail_count - fb);
    endtask

    task chk_eq(input string nm, input [63:0] got, exp);
        if (got !== exp) begin
            $display("    FAIL %s: got %0d (0x%0h), exp %0d (0x%0h)",
                     nm, got, got, exp, exp);
            fail_count++;
        end
    endtask

    task chk_byte(input string nm, input [7:0] got, exp);
        if (got !== exp) begin
            $display("    FAIL %s: got 0x%02h, exp 0x%02h", nm, got, exp);
            fail_count++;
        end
    endtask

    logic timed_out;

    initial begin
        fail_count    = 0;
        trade_in      = '0;
        enable        = 1'b1;
        m_axis_tready = 1'b1;
        reset_cap();

        $display("");
        $display("  ╔═════════════════════════════════════════════════════════════╗");
        $display("  ║  Order Gen TX Unit Test                                    ║");
        $display("  ╚═════════════════════════════════════════════════════════════╝");

        rst_n = 0; repeat(4) @(posedge clk); #1;
        rst_n = 1; repeat(2) @(posedge clk); #1;

        $display("\n--- TEST 1: Post-reset state ---");
        fb = fail_count;
        chk_eq("order_count",  order_count,  32'd0);
        chk_eq("net_position", net_position, 32'd0);
        if (m_axis_tvalid) begin fail_count++; $display("    FAIL: tvalid high"); end
        pr("post-reset idle");

        $display("\n--- TEST 2: BUY order (full packet check) ---");
        fb = fail_count;
        reset_cap();

        fire_trade(1'b0, 32'd15000, 64'h0000_0000_0012_3456);
        wait_pkt(timed_out);

        if (timed_out) begin
            $display("    FAIL: packet timed out"); fail_count++;
        end else begin
            $display("    Captured %0d bytes, %0d beats", cap_bytes, cap_beats);
            chk_eq("pkt_size",     cap_bytes,            32'd74);
            chk_eq("beats",       cap_beats,            32'd10);

            chk_byte("dst_mac[0]", cap_buf[0],  TB_DST_MAC[47:40]);
            chk_byte("dst_mac[5]", cap_buf[5],  TB_DST_MAC[7:0]);
            chk_byte("src_mac[0]", cap_buf[6],  TB_SRC_MAC[47:40]);
            chk_byte("src_mac[5]", cap_buf[11], TB_SRC_MAC[7:0]);
            chk_eq("ethertype",    cap_u16_be(12), 16'h0800);

            chk_byte("ipv4_ver",   cap_buf[14], 8'h45);
            chk_eq("ip_total_len", cap_u16_be(16), 16'd60);
            chk_byte("ip_ttl",     cap_buf[22], 8'h40);
            chk_byte("ip_proto",   cap_buf[23], 8'h11);

            if (cap_u16_be(24) == 16'h0000) begin
                fail_count++; $display("    FAIL: ip_checksum is zero");
            end
            chk_byte("src_ip[0]",  cap_buf[26], TB_SRC_IP[31:24]);
            chk_byte("src_ip[3]",  cap_buf[29], TB_SRC_IP[7:0]);
            chk_byte("dst_ip[0]",  cap_buf[30], TB_DST_IP[31:24]);
            chk_byte("dst_ip[3]",  cap_buf[33], TB_DST_IP[7:0]);

            chk_eq("udp_sport",    cap_u16_be(34), TB_SRC_PORT);
            chk_eq("udp_dport",    cap_u16_be(36), TB_DST_PORT);
            chk_eq("udp_len",      cap_u16_be(38), 16'd40);

            chk_byte("msg_type_lo", cap_buf[42], 8'h01);
            chk_byte("msg_type_hi", cap_buf[43], 8'h00);
            chk_byte("side",       cap_buf[44], 8'h42);
            chk_eq("symbol_id",    cap_u32_le(46), TB_IDX_SYM);
            chk_eq("price",        cap_u32_le(50), 32'd15010);
            chk_eq("qty",          cap_u32_le(54), TB_QTY);
            chk_eq("client_oid",   cap_u64_le(58), 64'd1);
            chk_eq("spread",       cap_u64_le(66), 64'h0000_0000_0012_3456);

            chk_eq("order_count",  order_count,  32'd1);
            chk_eq("net_position", net_position, 32'sd100);
        end
        pr("BUY order packet");

        wait_cd();

        $display("\n--- TEST 3: SELL order ---");
        fb = fail_count;
        reset_cap();

        fire_trade(1'b1, 32'd15000, 64'hAAAA_BBBB_CCCC_DDDD);
        wait_pkt(timed_out);

        if (timed_out) begin
            $display("    FAIL: packet timed out"); fail_count++;
        end else begin
            chk_byte("side",       cap_buf[44], 8'h53);
            chk_eq("price",        cap_u32_le(50), 32'd14990);
            chk_eq("client_oid",   cap_u64_le(58), 64'd2);
            chk_eq("spread",       cap_u64_le(66), 64'hAAAA_BBBB_CCCC_DDDD);
            chk_eq("order_count",  order_count,  32'd2);
            chk_eq("net_position", net_position, 32'sd0);
        end
        pr("SELL order packet");

        wait_cd();

        $display("\n--- TEST 4: Cooldown blocks second order ---");
        fb = fail_count;

        reset_cap();
        fire_trade(1'b0, 32'd16000, 64'd0);
        wait_pkt(timed_out);
        if (timed_out) begin fail_count++; $display("    FAIL: first pkt timeout"); end

        reset_cap();
        fire_trade(1'b0, 32'd16000, 64'd0);
        repeat(5) @(posedge clk); #1;

        if (cap_bytes != 0) begin
            fail_count++;
            $display("    FAIL: packet sent during cooldown");
        end else begin
            $display("    Cooldown correctly blocked second order");
        end

        chk_eq("order_count", order_count, 32'd3);
        pr("cooldown enforcement");

        wait_cd();

        $display("\n--- TEST 5: Post-cooldown order ---");
        fb = fail_count;
        reset_cap();

        fire_trade(1'b1, 32'd16000, 64'd0);
        wait_pkt(timed_out);

        if (timed_out) begin fail_count++; $display("    FAIL: timeout"); end
        else chk_eq("pkt_size", cap_bytes, 32'd74);

        chk_eq("order_count",  order_count,  32'd4);
        chk_eq("net_position", net_position, 32'sd0);
        pr("post-cooldown order");

        wait_cd();

        $display("\n--- TEST 6: Enable gate ---");
        fb = fail_count;
        enable = 1'b0;
        reset_cap();

        fire_trade(1'b0, 32'd18000, 64'd0);
        repeat(15) @(posedge clk); #1;

        if (cap_bytes != 0) begin
            fail_count++;
            $display("    FAIL: packet sent while disabled");
        end else begin
            $display("    Enable gate blocked order");
        end

        enable = 1'b1;
        chk_eq("order_count", order_count, 32'd4);
        pr("enable gate");

        repeat(2) @(posedge clk); #1;

        $display("\n--- TEST 7: Backpressure ---");
        fb = fail_count;
        reset_cap();

        fork
            fire_trade(1'b0, 32'd19000, 64'd0);
            begin

                repeat(4) @(posedge clk); #1;
                m_axis_tready = 1'b0;
                repeat(10) @(posedge clk); #1;
                m_axis_tready = 1'b1;
            end
        join

        wait_pkt(timed_out);

        if (timed_out) begin
            fail_count++;
            $display("    FAIL: timeout during backpressure");
        end else begin
            chk_eq("pkt_size", cap_bytes, 32'd74);
            $display("    Packet completed after backpressure release");
        end

        chk_eq("order_count", order_count, 32'd5);
        pr("backpressure handling");

        wait_cd();

        $display("\n--- TEST 8: Position limit ---");
        fb = fail_count;

        while (net_position + $signed(TB_QTY) <= $signed(TB_MAX_POS)) begin
            reset_cap();
            fire_trade(1'b0, 32'd20000, 64'd0);
            wait_pkt(timed_out);
            if (timed_out) begin fail_count++; $display("    FAIL: fill timeout"); break; end
            wait_cd();
        end

        $display("    Position: %0d (limit: +/-%0d)", net_position, TB_MAX_POS);

        reset_cap();
        fire_trade(1'b0, 32'd20000, 64'd0);
        repeat(15) @(posedge clk); #1;

        if (cap_bytes != 0) begin
            fail_count++;
            $display("    FAIL: order sent beyond position limit");
        end else begin
            $display("    Position limit correctly blocked BUY");
        end
        pr("position limit");

        $display("\n--- TEST 9: SELL allowed at max long position ---");
        fb = fail_count;
        reset_cap();

        fire_trade(1'b1, 32'd20000, 64'd0);
        wait_pkt(timed_out);

        if (timed_out) begin
            fail_count++;
            $display("    FAIL: SELL blocked at max long");
        end else begin
            $display("    SELL accepted at max position (pos now %0d)", net_position);
        end
        pr("opposing direction at limit");

        wait_cd();

        $display("\n--- TEST 10: SELL price floor ---");
        fb = fail_count;
        reset_cap();

        fire_trade(1'b1, 32'd5, 64'd0);
        wait_pkt(timed_out);

        if (timed_out) begin
            fail_count++;
            $display("    FAIL: timeout");
        end else begin
            chk_eq("floor_price", cap_u32_le(50), 32'd1);
            $display("    SELL price floored to 1 (actual 5, offset 10)");
        end
        pr("sell price floor");

        wait_cd();

        $display("\n--- TEST 11: OID sequence ---");
        fb = fail_count;
        begin
            integer expected_cnt;
            expected_cnt = order_count;

            reset_cap();
            fire_trade(1'b0, 32'd25000, 64'd0);
            wait_pkt(timed_out);

            if (timed_out) begin fail_count++; end
            else begin

                $display("    OID in packet: %0d (expected %0d)",
                         cap_u64_le(58), expected_cnt + 1);
                chk_eq("oid", cap_u64_le(58), 64'(expected_cnt + 1));
            end
        end
        pr("OID sequence");

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
        #500_000;
        $display("\nFATAL: Simulation watchdog timeout");
        $finish;
    end

endmodule
