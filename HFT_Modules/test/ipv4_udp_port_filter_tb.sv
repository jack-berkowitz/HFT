`timescale 1ns/1ps

module ipv4_udp_port_filter_tb;

    localparam DATA_W    = 64;
    localparam KEEP_W    = DATA_W / 8;
    localparam MAX_PKT_B = 256;
    localparam PKT_BITS  = MAX_PKT_B * 8;

    localparam [15:0] XDP_PORT   = 16'd11101;
    localparam [15:0] WRONG_PORT = 16'hDEAD;

    localparam int DECISION_BEAT = 5;

    logic        clk, rst_n;

    logic [63:0] s_axis_tdata;
    logic [7:0]  s_axis_tkeep;
    logic        s_axis_tvalid;
    logic        s_axis_tlast;
    logic        s_axis_tready;

    logic [63:0] m_axis_tdata;
    logic [7:0]  m_axis_tkeep;
    logic        m_axis_tvalid;
    logic        m_axis_tlast;
    logic        m_axis_tready;

    ipv4_udp_port_filter #(
        .FILTER_PORT (XDP_PORT)
    ) dut (
        .clk            (clk),
        .rst_n          (rst_n),
        .s_axis_tdata   (s_axis_tdata),
        .s_axis_tkeep   (s_axis_tkeep),
        .s_axis_tvalid  (s_axis_tvalid),
        .s_axis_tlast   (s_axis_tlast),
        .s_axis_tready  (s_axis_tready),
        .m_axis_tdata   (m_axis_tdata),
        .m_axis_tkeep   (m_axis_tkeep),
        .m_axis_tvalid  (m_axis_tvalid),
        .m_axis_tlast   (m_axis_tlast),
        .m_axis_tready  (m_axis_tready)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    logic [PKT_BITS-1:0] pkt_buf;
    integer              pkt_len;

    task pkt_write_byte(input integer idx, input logic [7:0] val);
        pkt_buf[idx*8 +: 8] = val;
    endtask

    task pkt_write_u16_be(input integer idx, input logic [15:0] val);

        pkt_buf[idx*8     +: 8] = val[15:8];
        pkt_buf[(idx+1)*8 +: 8] = val[7:0];
    endtask

    task build_eth_ip_udp(input integer udp_payload_len, input logic [15:0] dst_port);
        integer ip_total_len, udp_total_len;
        ip_total_len  = 20 + 8 + udp_payload_len;
        udp_total_len =      8 + udp_payload_len;

        pkt_buf = '0;

        pkt_write_byte(0,  8'h01); pkt_write_byte(1,  8'h00);
        pkt_write_byte(2,  8'h5e); pkt_write_byte(3,  8'h00);
        pkt_write_byte(4,  8'h00); pkt_write_byte(5,  8'h01);

        pkt_write_byte(6,  8'hde); pkt_write_byte(7,  8'had);
        pkt_write_byte(8,  8'hbe); pkt_write_byte(9,  8'hef);
        pkt_write_byte(10, 8'hca); pkt_write_byte(11, 8'hfe);

        pkt_write_byte(12, 8'h08); pkt_write_byte(13, 8'h00);

        pkt_write_byte(14, 8'h45);
        pkt_write_byte(15, 8'h00);
        pkt_write_byte(16, ip_total_len[15:8]);
        pkt_write_byte(17, ip_total_len[7:0]);
        pkt_write_byte(18, 8'h00); pkt_write_byte(19, 8'h00);
        pkt_write_byte(20, 8'h00); pkt_write_byte(21, 8'h00);
        pkt_write_byte(22, 8'h40);
        pkt_write_byte(23, 8'h11);
        pkt_write_byte(24, 8'h00); pkt_write_byte(25, 8'h00);
        pkt_write_byte(26, 8'hef); pkt_write_byte(27, 8'h01);
        pkt_write_byte(28, 8'h01); pkt_write_byte(29, 8'h01);
        pkt_write_byte(30, 8'hef); pkt_write_byte(31, 8'h01);
        pkt_write_byte(32, 8'h01); pkt_write_byte(33, 8'h01);

        pkt_write_byte(34, 8'hCA); pkt_write_byte(35, 8'hFE);
        pkt_write_u16_be(36, dst_port);
        pkt_write_byte(38, udp_total_len[15:8]);
        pkt_write_byte(39, udp_total_len[7:0]);
        pkt_write_byte(40, 8'h00); pkt_write_byte(41, 8'h00);
    endtask

    task build_xdp_pkt_hdr(input [15:0] pkt_size, input [7:0] num_msgs, input [31:0] seq_num);
        pkt_buf[42*8 +: 8]  = pkt_size[7:0];
        pkt_buf[43*8 +: 8]  = pkt_size[15:8];
        pkt_buf[44*8 +: 8]  = 8'd11;
        pkt_buf[45*8 +: 8]  = num_msgs;
        pkt_buf[46*8 +: 8]  = seq_num[7:0];
        pkt_buf[47*8 +: 8]  = seq_num[15:8];
        pkt_buf[48*8 +: 8]  = seq_num[23:16];
        pkt_buf[49*8 +: 8]  = seq_num[31:24];

        pkt_buf[50*8 +: 32] = 32'hAABBCCDD;
        pkt_buf[54*8 +: 32] = 32'h11223344;
    endtask

    task build_valid_xdp_pkt(input logic [15:0] dst_port);
        localparam XDP_MSG_LEN = 39;
        localparam UDP_PAYLOAD = 16 + XDP_MSG_LEN;
        integer i;

        build_eth_ip_udp(UDP_PAYLOAD, dst_port);
        build_xdp_pkt_hdr(16'd16 + 16'd39, 8'd1, 32'd1001);

        pkt_buf[58*8 +: 8] = XDP_MSG_LEN[7:0];
        pkt_buf[59*8 +: 8] = XDP_MSG_LEN[15:8];

        pkt_buf[60*8 +: 8] = 8'd100;
        pkt_buf[61*8 +: 8] = 8'd0;

        for (i = 62; i < 58 + XDP_MSG_LEN; i++)
            pkt_buf[i*8 +: 8] = i[7:0];

        pkt_len = 42 + 16 + XDP_MSG_LEN;
    endtask

    integer beat, byte_idx, bytes_this_beat, k;
    logic [63:0] beat_data;
    logic [7:0]  beat_keep;

    task axis_send_packet(input integer idle_cycles);
        integer beats_total, remainder;
        beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;
        remainder   = pkt_len % KEEP_W;

        for (beat = 0; beat < beats_total; beat++) begin
            if (idle_cycles > 0 && beat > 0) begin
                s_axis_tvalid = 1'b0;
                repeat(idle_cycles) @(posedge clk); #1;
            end

            beat_data = '0;
            beat_keep = '0;
            byte_idx  = beat * KEEP_W;
            bytes_this_beat = (beat == beats_total - 1 && remainder != 0)
                              ? remainder : KEEP_W;

            for (k = 0; k < bytes_this_beat; k++) begin
                beat_data[k*8 +: 8] = pkt_buf[(byte_idx+k)*8 +: 8];
                beat_keep[k]        = 1'b1;
            end

            s_axis_tdata  = beat_data;
            s_axis_tkeep  = beat_keep;
            s_axis_tvalid = 1'b1;
            s_axis_tlast  = (beat == beats_total - 1);

            @(posedge clk);
            while (!s_axis_tready) @(posedge clk);
            #1;
        end

        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        s_axis_tkeep  = '0;
        s_axis_tdata  = '0;
    endtask

    integer mon_beat_count;
    logic   mon_tlast_seen;
    logic   trace_en;

    always @(posedge clk) begin
        if (trace_en && s_axis_tvalid && s_axis_tready) begin
            $display("  %0tps  sV=%b sR=%b sL=%b  sD=%h  sK=%b  |  mV=%b mR=%b mL=%b  mD=%h  mK=%b",
                     $time,
                     s_axis_tvalid, s_axis_tready, s_axis_tlast,
                     s_axis_tdata, s_axis_tkeep,
                     m_axis_tvalid, m_axis_tready, m_axis_tlast,
                     m_axis_tdata, m_axis_tkeep);
        end
        if (m_axis_tvalid && m_axis_tready) begin
            mon_beat_count++;
            if (m_axis_tlast) mon_tlast_seen = 1'b1;
        end
    end

    task wait_drain(input integer cycles);
        repeat(cycles) @(posedge clk); #1;
    endtask

    integer fail_count;

    initial begin
        fail_count      = 0;
        mon_beat_count  = 0;
        mon_tlast_seen  = 1'b0;
        trace_en        = 1'b0;

        s_axis_tdata  = '0;
        s_axis_tkeep  = '0;
        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        m_axis_tready = 1'b1;

        rst_n = 0;
        repeat(4) @(posedge clk); #1;
        rst_n = 1;
        repeat(2) @(posedge clk); #1;

        $display("\n=== TEST 1: Valid XDP packet ===");
        mon_beat_count = 0;
        mon_tlast_seen = 1'b0;
        trace_en       = 1'b1;

        build_valid_xdp_pkt(XDP_PORT);
        axis_send_packet(0);
        wait_drain(10);
        trace_en = 1'b0;

        if (mon_beat_count == 0) begin
            $display("FAIL t1a: no output beats seen"); fail_count++;
        end else
            $display("PASS t1a: %0d output beats forwarded", mon_beat_count);

        if (!mon_tlast_seen) begin
            $display("FAIL t1b: tlast never asserted"); fail_count++;
        end else
            $display("PASS t1b: tlast seen on output");

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 2: Wrong UDP port ===");
        mon_beat_count = 0;
        mon_tlast_seen = 1'b0;

        build_valid_xdp_pkt(WRONG_PORT);
        axis_send_packet(0);
        wait_drain(10);

        if (mon_beat_count == 0)
            $display("PASS t2a: wrong-port packet correctly dropped");
        else begin
            $display("FAIL t2a: %0d beats leaked through", mon_beat_count); fail_count++;
        end

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 3: Wrong EtherType (ARP) ===");
        mon_beat_count = 0;

        build_valid_xdp_pkt(XDP_PORT);

        pkt_buf[12*8 +: 8] = 8'h08;
        pkt_buf[13*8 +: 8] = 8'h06;
        axis_send_packet(0);
        wait_drain(10);

        if (mon_beat_count == 0) $display("PASS t3a: ARP packet dropped");
        else begin $display("FAIL t3a: ARP leaked"); fail_count++; end

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 4: Wrong IP proto (TCP) ===");
        mon_beat_count = 0;

        build_valid_xdp_pkt(XDP_PORT);
        pkt_buf[23*8 +: 8] = 8'h06;
        axis_send_packet(0);
        wait_drain(10);

        if (mon_beat_count == 0) $display("PASS t4a: TCP packet dropped");
        else begin $display("FAIL t4a: TCP leaked"); fail_count++; end

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 5: Valid packet with idle gaps ===");
        mon_beat_count = 0;
        mon_tlast_seen = 1'b0;

        build_valid_xdp_pkt(XDP_PORT);
        axis_send_packet(2);
        wait_drain(10);

        if (mon_beat_count == 0) begin
            $display("FAIL t5a: no output beats with idle gaps"); fail_count++;
        end else
            $display("PASS t5a: %0d beats forwarded with idle gaps", mon_beat_count);

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 6: Two consecutive valid packets ===");
        mon_beat_count = 0;

        build_valid_xdp_pkt(XDP_PORT);
        axis_send_packet(0);
        build_valid_xdp_pkt(XDP_PORT);
        axis_send_packet(0);
        wait_drain(10);

        if (mon_beat_count == 0) begin
            $display("FAIL t6a: no beats from 2 packets"); fail_count++;
        end else
            $display("PASS t6a: %0d beats from 2 packets", mon_beat_count);

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 7: Valid → Drop → Valid ===");
        mon_beat_count = 0;

        build_valid_xdp_pkt(XDP_PORT);
        axis_send_packet(0);
        build_valid_xdp_pkt(WRONG_PORT);
        axis_send_packet(0);
        build_valid_xdp_pkt(XDP_PORT);
        axis_send_packet(0);
        wait_drain(10);

        if (mon_beat_count == 0) begin
            $display("FAIL t7a: no beats from valid→drop→valid"); fail_count++;
        end else
            $display("PASS t7a: %0d beats (2 valid packets)", mon_beat_count);

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 8: Backpressure test ===");
        mon_beat_count = 0;
        mon_tlast_seen = 1'b0;

        build_valid_xdp_pkt(XDP_PORT);

        fork
            axis_send_packet(0);
            begin

                repeat(8) @(posedge clk); #1;
                m_axis_tready = 1'b0;
                repeat(5) @(posedge clk); #1;
                if (s_axis_tready === 1'b1)
                    $display("  FAIL t8a: s_axis_tready stayed high during backpressure");
                else
                    $display("  PASS t8a: backpressure propagated");
                m_axis_tready = 1'b1;
            end
        join

        wait_drain(20);

        if (!mon_tlast_seen) begin
            $display("FAIL t8b: packet did not complete after backpressure"); fail_count++;
        end else
            $display("PASS t8b: packet completed after releasing backpressure");

        m_axis_tready = 1'b1;

        repeat(4) @(posedge clk);
        $display("\n========================================");
        if (fail_count == 0)
            $display("@@@ Passed");
        else
            $display("@@@ Failed: %0d TEST(S) FAILED", fail_count);
        $display("========================================\n");
        $finish;
    end

endmodule
