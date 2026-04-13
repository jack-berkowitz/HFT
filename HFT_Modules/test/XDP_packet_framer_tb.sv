`timescale 1ns/1ps

module xdp_packet_framer_tb;

    localparam DATA_W    = 64;
    localparam KEEP_W    = DATA_W / 8;
    localparam MAX_PKT_B = 256;
    localparam PKT_BITS  = MAX_PKT_B * 8;

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

    xdp_packet_framer #(
        .HEADER_BYTES(58)
    ) dut (
        .clk           (clk),
        .rst_n         (rst_n),
        .s_axis_tdata  (s_axis_tdata),
        .s_axis_tkeep  (s_axis_tkeep),
        .s_axis_tvalid (s_axis_tvalid),
        .s_axis_tlast  (s_axis_tlast),
        .s_axis_tready (s_axis_tready),
        .m_axis_tdata  (m_axis_tdata),
        .m_axis_tkeep  (m_axis_tkeep),
        .m_axis_tvalid (m_axis_tvalid),
        .m_axis_tlast  (m_axis_tlast),
        .m_axis_tready (m_axis_tready)
    );

    initial clk = 0;
    always #2 clk = ~clk;

    logic [PKT_BITS-1:0] pkt_buf;
    integer              pkt_len;

    task build_test_pkt(input integer payload_len);
        integer i;
        pkt_buf = '0;

        for (i = 0; i < 58; i++)
            pkt_buf[i*8 +: 8] = 8'hFF;

        for (i = 0; i < payload_len; i++)
            pkt_buf[(58+i)*8 +: 8] = 8'hA0 + i[7:0];
        pkt_len = 58 + payload_len;
    endtask

    task axis_send(input integer idle_cycles);
        integer beats_total, remainder, bt, bk, bi, bthis;
        logic [63:0] bd;
        logic [7:0]  bkp;

        beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;
        remainder   = pkt_len % KEEP_W;

        for (bt = 0; bt < beats_total; bt++) begin
            if (idle_cycles > 0 && bt > 0) begin
                s_axis_tvalid = 1'b0;
                repeat(idle_cycles) @(posedge clk); #1;
            end

            bd  = '0;
            bkp = '0;
            bi  = bt * KEEP_W;
            bthis = (bt == beats_total - 1 && remainder != 0) ? remainder : KEEP_W;

            for (bk = 0; bk < bthis; bk++) begin
                bd[bk*8 +: 8] = pkt_buf[(bi+bk)*8 +: 8];
                bkp[bk]       = 1'b1;
            end

            s_axis_tdata  = bd;
            s_axis_tkeep  = bkp;
            s_axis_tvalid = 1'b1;
            s_axis_tlast  = (bt == beats_total - 1);

            @(posedge clk);
            while (!s_axis_tready) @(posedge clk);
            #1;
        end

        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
    endtask

    logic [PKT_BITS-1:0] out_buf;
    integer              out_byte_count;
    integer              out_beat_count;
    logic                out_last_seen;

    always @(posedge clk) begin
        if (m_axis_tvalid && m_axis_tready) begin
            for (int b = 0; b < KEEP_W; b++) begin
                if (m_axis_tkeep[b]) begin
                    out_buf[out_byte_count*8 +: 8] = m_axis_tdata[b*8 +: 8];
                    out_byte_count = out_byte_count + 1;
                end
            end
            out_beat_count = out_beat_count + 1;
            if (m_axis_tlast) out_last_seen = 1'b1;
        end
    end

    task reset_capture();
        out_buf        = '0;
        out_byte_count = 0;
        out_beat_count = 0;
        out_last_seen  = 1'b0;
    endtask

    integer fail_count;

    initial begin
        fail_count     = 0;
        m_axis_tready  = 1'b1;
        s_axis_tdata   = '0;
        s_axis_tkeep   = '0;
        s_axis_tvalid  = 1'b0;
        s_axis_tlast   = 1'b0;
        reset_capture();

        rst_n = 0;
        repeat(4) @(posedge clk); #1;
        rst_n = 1;
        repeat(2) @(posedge clk); #1;

        $display("\n=== TEST 1: 39-byte payload (back-to-back) ===");
        reset_capture();
        build_test_pkt(39);
        axis_send(0);

        repeat(30) @(posedge clk); #1;

        $display("  Output: %0d bytes, %0d beats, last=%b",
                 out_byte_count, out_beat_count, out_last_seen);

        if (!out_last_seen) begin
            $display("FAIL t1a: tlast never seen"); fail_count++;
        end

        begin
            logic ok;
            ok = 1;
            for (int i = 0; i < 39 && i < out_byte_count; i++) begin
                if (out_buf[i*8 +: 8] !== (8'hA0 + i[7:0])) begin
                    $display("  MISMATCH byte[%0d]: got 0x%h, exp 0x%h",
                             i, out_buf[i*8 +: 8], 8'hA0 + i[7:0]);
                    ok = 0;
                end
            end
            if (ok && out_byte_count == 39)
                $display("PASS t1: all 39 payload bytes correct");
            else begin
                $display("FAIL t1b: payload mismatch (got %0d bytes)", out_byte_count);
                fail_count++;
            end
        end

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 2: 39-byte payload with idle gaps ===");
        reset_capture();
        build_test_pkt(39);
        axis_send(2);
        repeat(30) @(posedge clk); #1;

        if (!out_last_seen) begin
            $display("FAIL t2a: tlast never seen"); fail_count++;
        end else begin
            logic ok2;
            ok2 = 1;
            for (int i = 0; i < 39 && i < out_byte_count; i++) begin
                if (out_buf[i*8 +: 8] !== (8'hA0 + i[7:0])) ok2 = 0;
            end
            if (ok2 && out_byte_count == 39)
                $display("PASS t2: payload correct with idle gaps");
            else begin
                $display("FAIL t2b: mismatch (got %0d bytes)", out_byte_count);
                fail_count++;
            end
        end

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 3: 6-byte payload (boundary beat only) ===");
        reset_capture();
        build_test_pkt(6);
        axis_send(0);
        repeat(30) @(posedge clk); #1;

        if (out_byte_count == 6 && out_last_seen)
            $display("PASS t3: 6-byte payload correct");
        else begin
            $display("FAIL t3: got %0d bytes, last=%b", out_byte_count, out_last_seen);
            fail_count++;
        end

        repeat(4) @(posedge clk); #1;

        $display("\n=== TEST 4: Two consecutive packets ===");
        reset_capture();
        build_test_pkt(25);
        axis_send(0);
        repeat(30) @(posedge clk); #1;

        if (out_byte_count == 25 && out_last_seen)
            $display("PASS t4a: first packet ok (%0d bytes)", out_byte_count);
        else begin
            $display("FAIL t4a: first pkt %0d bytes last=%b", out_byte_count, out_last_seen);
            fail_count++;
        end

        reset_capture();
        build_test_pkt(39);
        axis_send(0);
        repeat(30) @(posedge clk); #1;

        if (out_byte_count == 39 && out_last_seen)
            $display("PASS t4b: second packet ok (%0d bytes)", out_byte_count);
        else begin
            $display("FAIL t4b: second pkt %0d bytes last=%b", out_byte_count, out_last_seen);
            fail_count++;
        end

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
