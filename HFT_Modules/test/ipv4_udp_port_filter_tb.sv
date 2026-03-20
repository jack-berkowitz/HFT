`timescale 1ns/1ps

// ============================================================
// ipv4_udp_port_filter_tb.sv
// ============================================================
module ipv4_udp_port_filter_tb;

    // ----------------------------------------------------------------
    // Parameters
    // ----------------------------------------------------------------
    localparam DATA_W    = 64;
    localparam KEEP_W    = DATA_W / 8;
    localparam MAX_PKT_B = 256;
    localparam PKT_BITS  = MAX_PKT_B * 8;

    localparam [15:0] MDP3_PORT    = 16'd14310;
    localparam [15:0] WRONG_PORT   = 16'hDEAD;

    // Must match BEAT_UDP_DPORT in ipv4_udp_port_filter.sv
    localparam int DECISION_BEAT = 4;

    // ----------------------------------------------------------------
    // DUT signals
    // ----------------------------------------------------------------
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

    // ----------------------------------------------------------------
    // DUT instantiation
    // ----------------------------------------------------------------
    ipv4_udp_port_filter #(
        .FILTER_PORT (MDP3_PORT)
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

    // ----------------------------------------------------------------
    // Clock: 250 MHz
    // ----------------------------------------------------------------
    initial clk = 0;
    always #2 clk = ~clk;

    // ----------------------------------------------------------------
    // Packet buffer
    // ----------------------------------------------------------------
    logic [PKT_BITS-1:0] pkt_buf;
    integer              pkt_len;

    // ----------------------------------------------------------------
    // Packet write helpers
    // ----------------------------------------------------------------
    task automatic pkt_write_byte(input int idx, input logic [7:0] val);
        pkt_buf[idx*8 +: 8] = val;
    endtask

    task automatic pkt_write_u16_le(input int idx, input logic [15:0] val);
        pkt_buf[idx*8     +: 8] = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
    endtask

    task automatic pkt_write_u32_le(input int idx, input logic [31:0] val);
        for (int k = 0; k < 4; k++)
            pkt_buf[(idx+k)*8 +: 8] = val[k*8 +: 8];
    endtask

    // ----------------------------------------------------------------
    // Header builder
    // ----------------------------------------------------------------
    task automatic build_eth_ip_udp_headers(
        input int          udp_payload_len,
        input logic [15:0] dst_port,
        input logic [15:0] ethertype,
        input logic [7:0]  ip_proto
    );
        int ip_total_len  = 20 + 8 + udp_payload_len;
        int udp_total_len =      8 + udp_payload_len;

        pkt_write_byte(0,  8'h01); pkt_write_byte(1,  8'h00);
        pkt_write_byte(2,  8'h5e); pkt_write_byte(3,  8'h00);
        pkt_write_byte(4,  8'h00); pkt_write_byte(5,  8'h01);
        pkt_write_byte(6,  8'hde); pkt_write_byte(7,  8'had);

        pkt_write_byte(8,  8'hbe); pkt_write_byte(9,  8'hef);
        pkt_write_byte(10, 8'hca); pkt_write_byte(11, 8'hfe);
        pkt_write_byte(12, ethertype[15:8]);
        pkt_write_byte(13, ethertype[7:0]);
        pkt_write_byte(14, 8'h45);
        pkt_write_byte(15, 8'h00);

        pkt_write_byte(16, ip_total_len[15:8]);
        pkt_write_byte(17, ip_total_len[7:0]);
        pkt_write_byte(18, 8'h00); pkt_write_byte(19, 8'h00);
        pkt_write_byte(20, 8'h00); pkt_write_byte(21, 8'h00);
        pkt_write_byte(22, 8'h40);
        pkt_write_byte(23, ip_proto);

        pkt_write_byte(24, 8'h00); pkt_write_byte(25, 8'h00);
        pkt_write_byte(26, 8'hef); pkt_write_byte(27, 8'h01);
        pkt_write_byte(28, 8'h01); pkt_write_byte(29, 8'h01);
        pkt_write_byte(30, 8'hef); pkt_write_byte(31, 8'h01);

        pkt_write_byte(32, 8'h01); pkt_write_byte(33, 8'h01);
        pkt_write_byte(34, 8'h37); pkt_write_byte(35, 8'ha1);
        pkt_write_byte(36, dst_port[15:8]);
        pkt_write_byte(37, dst_port[7:0]);
        pkt_write_byte(38, udp_total_len[15:8]);
        pkt_write_byte(39, udp_total_len[7:0]);

        pkt_write_byte(40, 8'h00); pkt_write_byte(41, 8'h00);
        pkt_write_u32_le(42, 32'hDEAD_BEEF);
    endtask

    task automatic build_pkt(
        input logic [15:0] dst_port,
        input logic [15:0] ethertype,
        input logic [7:0]  ip_proto
    );
        pkt_buf = '0;
        build_eth_ip_udp_headers(12, dst_port, ethertype, ip_proto);
        pkt_len = 14 + 20 + 8 + 12;   // 54 bytes → 7 beats total, 2 forwarded
    endtask

    function automatic int forwarded_beats(input int len);
        int total = (len + KEEP_W - 1) / KEEP_W;
        int fwd   = total - (DECISION_BEAT + 1);
        return (fwd < 0) ? 0 : fwd;
    endfunction

    // ----------------------------------------------------------------
    // AXI-Stream Master BFM
    // ----------------------------------------------------------------
    task automatic axis_send_packet(input int idle_cycles);
        int beats_total     = (pkt_len + KEEP_W - 1) / KEEP_W;
        int remainder       = pkt_len % KEEP_W;
        logic [DATA_W-1:0] beat_data;
        logic [KEEP_W-1:0] beat_keep;

        for (int beat = 0; beat < beats_total; beat++) begin
            int byte_idx        = beat * KEEP_W;
            int bytes_this_beat = (beat == beats_total-1 && remainder != 0)
                                  ? remainder : KEEP_W;

            if (idle_cycles > 0 && beat > 0) begin
                s_axis_tvalid = 1'b0;
                repeat(idle_cycles) @(posedge clk); #1;
            end

            beat_data = '0;
            beat_keep = '0;
            for (int k = 0; k < bytes_this_beat; k++) begin
                beat_data[k*8 +: 8] = pkt_buf[(byte_idx+k)*8 +: 8];
                beat_keep[k]        = 1'b1;
            end

            s_axis_tdata  = beat_data;
            s_axis_tkeep  = beat_keep;
            s_axis_tvalid = 1'b1;
            s_axis_tlast  = (beat == beats_total-1);

            @(posedge clk);
            while (!s_axis_tready) @(posedge clk);
            #1;
        end

        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        s_axis_tkeep  = '0;
        s_axis_tdata  = '0;
    endtask

    // ----------------------------------------------------------------
    // IO Tracer
    //
    // Prints one line per active clock cycle showing:
    //   [Time] slave-side signals | master-side signals | decoded annotation
    //
    // The annotation column decodes header fields on the beats the filter
    // actually inspects, and marks them OK or * (bad) so byte-ordering
    // bugs are immediately visible without opening a waveform viewer.
    //
    // beat_count is inferred from slave-side transfers so no internal
    // DUT signals need to be probed.
    // ----------------------------------------------------------------
    logic trace_en;
    int   trace_beat;

    logic s_xfer, m_xfer;
    assign s_xfer = s_axis_tvalid & s_axis_tready;
    assign m_xfer = m_axis_tvalid & m_axis_tready;

    // Infer beat index from observed slave transfers
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            trace_beat <= 0;
        else if (s_xfer)
            trace_beat <= s_axis_tlast ? 0 : trace_beat + 1;
    end

    // Decode the header field that lives on the given beat and format it
    function automatic string beat_annotation(
        input int          beat,
        input logic [63:0] tdata
    );
        logic [15:0] ethertype, udp_dport;
        logic [7:0]  ip_proto;
        string s;
        case (beat)
            1: begin
                // EtherType big-endian: MSB=byte12 at tdata[39:32], LSB=byte13 at [47:40]
                ethertype = {tdata[39:32], tdata[47:40]};
                $sformat(s, "beat1: EtherType=0x%04h  (%s)",
                    ethertype,
                    (ethertype == 16'h0800) ? "IPv4 OK" : "NOT IPv4 ***");
            end
            2: begin
                // IP proto: byte23 = tdata[63:56]
                ip_proto = tdata[63:56];
                $sformat(s, "beat2: IP_proto=0x%02h      (%s)",
                    ip_proto,
                    (ip_proto == 8'h11) ? "UDP OK" : "NOT UDP ***");
            end
            4: begin
                // UDP dport big-endian: MSB=byte36 at tdata[39:32], LSB=byte37 at [47:40]
                udp_dport = {tdata[39:32], tdata[47:40]};
                $sformat(s, "beat4: UDP_dport=%5d (0x%04h) (%s)",
                    udp_dport, udp_dport,
                    (udp_dport == MDP3_PORT) ? "MDP3 OK  <-- PASS/DROP decision" :
                                               "WRONG PORT <-- will DROP ***");
            end
            default: $sformat(s, "beat%0d: (no filter check)", beat);
        endcase
        return s;
    endfunction

    task automatic print_trace_header();
        $display("  %-8s  %-67s  %-67s  %s",
            "Time(ps)",
            "---------- SLAVE IN (TB → DUT) ----------",
            "---------- MASTER OUT (DUT → DS) ---------",
            "--- Annotation ---");
        $display("  %-8s  %-4s %-4s %-4s  bt  %-18s %-8s  %-4s %-4s %-4s %-18s %-8s  %s",
            "", "sV","sRdy","sL","s_tdata[63:0]","s_keep",
            "mV","mRdy","mL","m_tdata[63:0]","m_keep", "");
        $display("  %s", {"----------------------------------------------------------------",
                           "----------------------------------------------------------------"});
    endtask

    // One trace line per active cycle
    always @(posedge clk) begin
        if (trace_en && (s_axis_tvalid || m_axis_tvalid)) begin
            $display("  %8t  %4b %4b %4b  %2d  %16h  %08b  %4b %4b %4b  %16h  %08b  %s",
                $time,
                s_axis_tvalid, s_axis_tready, s_axis_tlast,
                trace_beat,
                s_axis_tdata,  s_axis_tkeep,
                m_axis_tvalid, m_axis_tready, m_axis_tlast,
                m_axis_tdata,  m_axis_tkeep,
                (s_axis_tvalid ? beat_annotation(trace_beat, s_axis_tdata) : "-"));
        end
    end

    // ----------------------------------------------------------------
    // Output monitor
    // ----------------------------------------------------------------
    int   mon_beat_count;
    logic mon_tlast_seen;
    logic mon_tvalid_seen;
    logic mon_data_mismatch;

    always @(posedge clk) begin
        if (!rst_n) begin
            mon_beat_count    <= 0;
            mon_tlast_seen    <= 1'b0;
            mon_tvalid_seen   <= 1'b0;
            mon_data_mismatch <= 1'b0;
        end else if (m_xfer) begin
            mon_beat_count  <= mon_beat_count + 1;
            mon_tvalid_seen <= 1'b1;
            if (m_axis_tlast)    mon_tlast_seen    <= 1'b1;
            if (m_axis_tdata !== s_axis_tdata) mon_data_mismatch <= 1'b1;
        end
    end

    task automatic reset_monitor();
        @(posedge clk); #1;
        mon_beat_count    = 0;
        mon_tlast_seen    = 1'b0;
        mon_tvalid_seen   = 1'b0;
        mon_data_mismatch = 1'b0;
    endtask

    task automatic wait_drain(input int cycles);
        repeat(cycles) @(posedge clk); #1;
    endtask

    // ----------------------------------------------------------------
    // VCD
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("ipv4_udp_port_filter_dump.vcd");
        $dumpvars(0, ipv4_udp_port_filter_tb);
    end

    // ----------------------------------------------------------------
    // Test runner
    // ----------------------------------------------------------------
    int  fail_count;
    int  exp_beats;

    initial begin
        fail_count    = 0;
        trace_en      = 1'b0;
        s_axis_tdata  = '0;
        s_axis_tkeep  = '0;
        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        m_axis_tready = 1'b1;

        rst_n = 1'b0;
        repeat(4) @(posedge clk); #1;
        rst_n = 1'b1;
        repeat(2) @(posedge clk); #1;

        // ==============================================================
        // TEST 1: Valid MDP3 packet — back-to-back beats
        // 54 bytes → 7 beats. Beats 0-4 inspected, beats 5-6 forwarded.
        // ==============================================================
        $display("\n=== TEST 1: Valid MDP3 packet ===");
        $display("  Sending: EtherType=0x0800, IP_proto=0x11, UDP_dport=%0d", MDP3_PORT);
        $display("  Expect : filter reaches PASS, 2 beats forwarded, tlast seen");
        print_trace_header();
        reset_monitor();
        trace_en  = 1'b1;
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        exp_beats = forwarded_beats(pkt_len);   // 2
        axis_send_packet(0);
        wait_drain(10);
        trace_en  = 1'b0;

        if (!mon_tvalid_seen)
            begin $display("FAIL t1a: no output beats seen"); fail_count++; end
        else $display("PASS t1a: output beats seen");

        if (!mon_tlast_seen)
            begin $display("FAIL t1b: tlast never asserted on output"); fail_count++; end
        else $display("PASS t1b: tlast seen on output");

        if (mon_beat_count !== exp_beats)
            begin $display("FAIL t1c: got %0d forwarded beats, expected %0d",
                mon_beat_count, exp_beats); fail_count++; end
        else $display("PASS t1c: correct forwarded beat count %0d", mon_beat_count);

        if (mon_data_mismatch)
            begin $display("FAIL t1d: data mismatch on passthrough"); fail_count++; end
        else $display("PASS t1d: data passed through intact");

        // ==============================================================
        // TEST 2: Wrong UDP destination port → DROP
        // ==============================================================
        $display("\n=== TEST 2: Wrong UDP port (should drop) ===");
        $display("  Sending: UDP_dport=0xDEAD");
        $display("  Expect : filter reaches DROP, 0 beats forwarded");
        print_trace_header();
        reset_monitor();
        trace_en  = 1'b1;
        build_pkt(WRONG_PORT, 16'h0800, 8'h11);
        axis_send_packet(0);
        wait_drain(10);
        trace_en  = 1'b0;

        if (mon_tvalid_seen)
            begin $display("FAIL t2a: m_axis_tvalid asserted on wrong-port packet"); fail_count++; end
        else $display("PASS t2a: wrong-port packet correctly dropped");

        if (mon_beat_count !== 0)
            begin $display("FAIL t2b: %0d beats leaked through", mon_beat_count); fail_count++; end
        else $display("PASS t2b: zero beats forwarded");

        // ==============================================================
        // TEST 3: Wrong EtherType (ARP 0x0806) → DROP
        // ==============================================================
        $display("\n=== TEST 3: Wrong EtherType (ARP, should drop) ===");
        $display("  Sending: EtherType=0x0806");
        $display("  Expect : filter reaches DROP after beat 1, 0 beats forwarded");
        print_trace_header();
        reset_monitor();
        trace_en  = 1'b1;
        build_pkt(MDP3_PORT, 16'h0806, 8'h11);
        axis_send_packet(0);
        wait_drain(10);
        trace_en  = 1'b0;

        if (mon_tvalid_seen)
            begin $display("FAIL t3a: ARP packet was not dropped"); fail_count++; end
        else $display("PASS t3a: ARP packet dropped");

        // ==============================================================
        // TEST 4: Wrong IP protocol (TCP 0x06) → DROP
        // ==============================================================
        $display("\n=== TEST 4: Wrong IP protocol (TCP, should drop) ===");
        $display("  Sending: IP_proto=0x06");
        $display("  Expect : filter reaches DROP after beat 2, 0 beats forwarded");
        print_trace_header();
        reset_monitor();
        trace_en  = 1'b1;
        build_pkt(MDP3_PORT, 16'h0800, 8'h06);
        axis_send_packet(0);
        wait_drain(10);
        trace_en  = 1'b0;

        if (mon_tvalid_seen)
            begin $display("FAIL t4a: TCP packet was not dropped"); fail_count++; end
        else $display("PASS t4a: TCP packet dropped");

        // ==============================================================
        // TEST 5: Valid packet with idle gaps between beats
        // ==============================================================
        $display("\n=== TEST 5: Valid packet with idle gaps ===");
        $display("  Sending: valid MDP3 with 2-cycle idle gaps between beats");
        $display("  Expect : same as T1 — 2 forwarded beats");
        print_trace_header();
        reset_monitor();
        trace_en  = 1'b1;
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        exp_beats = forwarded_beats(pkt_len);
        axis_send_packet(2);
        wait_drain(10);
        trace_en  = 1'b0;

        if (!mon_tvalid_seen)
            begin $display("FAIL t5a: no output beats with idle gaps"); fail_count++; end
        else $display("PASS t5a: output beats seen with idle gaps");

        if (mon_beat_count !== exp_beats)
            begin $display("FAIL t5b: got %0d beats, expected %0d",
                mon_beat_count, exp_beats); fail_count++; end
        else $display("PASS t5b: correct forwarded beat count %0d with idle gaps", mon_beat_count);

        // ==============================================================
        // TEST 6: Two consecutive valid packets
        // ==============================================================
        $display("\n=== TEST 6: Two consecutive valid packets ===");
        $display("  Sending: 2 valid MDP3 packets back-to-back");
        $display("  Expect : 4 total forwarded beats (2 per packet)");
        print_trace_header();
        reset_monitor();
        trace_en  = 1'b1;
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        exp_beats = forwarded_beats(pkt_len) * 2;
        axis_send_packet(0);
        axis_send_packet(0);
        wait_drain(10);
        trace_en  = 1'b0;

        if (mon_beat_count !== exp_beats)
            begin $display("FAIL t6a: got %0d beats, expected %0d for 2 packets",
                mon_beat_count, exp_beats); fail_count++; end
        else $display("PASS t6a: correct forwarded beat count for 2 consecutive packets");

        // ==============================================================
        // TEST 7: Valid → Drop → Valid
        // ==============================================================
        $display("\n=== TEST 7: Valid → Drop → Valid ===");
        $display("  Sending: pkt1=PASS, pkt2=DROP(wrong port), pkt3=PASS");
        $display("  Expect : 4 total forwarded beats (0 from dropped packet)");
        print_trace_header();
        reset_monitor();
        trace_en  = 1'b1;
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        exp_beats = forwarded_beats(pkt_len) * 2;
        axis_send_packet(0);
        build_pkt(WRONG_PORT, 16'h0800, 8'h11);
        axis_send_packet(0);
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        axis_send_packet(0);
        wait_drain(10);
        trace_en  = 1'b0;

        if (mon_beat_count !== exp_beats)
            begin $display("FAIL t7a: got %0d beats, expected %0d (2 passing packets)",
                mon_beat_count, exp_beats); fail_count++; end
        else $display("PASS t7a: filter recovered correctly after drop");

        // ==============================================================
        // TEST 8: Backpressure
        // ==============================================================
        $display("\n=== TEST 8: Backpressure (m_axis_tready held low) ===");
        $display("  Sending: valid MDP3, but hold m_axis_tready=0 for first 5 cycles");
        $display("  Expect : s_axis_tready deasserts; packet completes after tready released");
        print_trace_header();
        reset_monitor();
        trace_en      = 1'b1;
        m_axis_tready = 1'b0;
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);

        fork
            begin
                axis_send_packet(0);
            end
            begin
                repeat(5) @(posedge clk); #1;
                if (s_axis_tready === 1'b1)
                    $display("  FAIL t8a: s_axis_tready stayed high during backpressure");
                else
                    $display("  PASS t8a: s_axis_tready deasserted during backpressure");
                m_axis_tready = 1'b1;
            end
        join

        wait_drain(20);
        trace_en = 1'b0;

        if (!mon_tlast_seen)
            begin $display("FAIL t8b: packet did not complete after releasing tready"); fail_count++; end
        else $display("PASS t8b: packet completed after releasing backpressure");

        m_axis_tready = 1'b1;

        // ==============================================================
        // Summary
        // ==============================================================
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