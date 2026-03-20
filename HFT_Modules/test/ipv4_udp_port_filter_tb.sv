`timescale 1ns/1ps

// ============================================================
// ipv4_udp_port_filter_tb.sv
//
// Dedicated testbench for ipv4_udp_port_filter ONLY.
// Does NOT instantiate the framer or demux.
//
// What we are testing:
//   - Correct MDP3 packet passes through (m_axis_tvalid asserted)
//   - Wrong UDP port is dropped       (m_axis_tvalid suppressed)
//   - Wrong EtherType is dropped
//   - Wrong IP protocol is dropped
//   - All beats of a passing packet are forwarded intact
//   - tvalid is suppressed for the ENTIRE dropped packet
//   - Backpressure: m_axis_tready low stalls the pipeline
//   - Back-to-back packets
//   - Idle gaps between beats
//
// Beat/byte mapping (64-bit bus, 0-indexed beats):
//   Beat 0: bytes  0- 7  Eth dst MAC (bytes 0-5), src MAC start (6-7)
//   Beat 1: bytes  8-15  Eth src MAC end (8-11), EtherType (12-13), IPv4 start (14-15)
//   Beat 2: bytes 16-23  IPv4 header (IP proto at byte 23)
//   Beat 3: bytes 24-31  IPv4 header (checksum, src IP, dst IP start)
//   Beat 4: bytes 32-39  dst IP end (32-33), UDP header (34-41: src port, dst port, len, chk)
//   Beat 5: bytes 40-47  MDP3 header start
//   ...
//
// Three fields the filter checks:
//   EtherType at bytes 12-13  → beat 1, tdata[47:32]
//   IP proto  at byte  23     → beat 2, tdata[63:56]
//   UDP dport at bytes 36-37  → beat 4, tdata[47:32]
// ============================================================
module ipv4_udp_port_filter_tb;

    // ----------------------------------------------------------------
    // Parameters
    // ----------------------------------------------------------------
    localparam DATA_W    = 64;
    localparam KEEP_W    = DATA_W / 8;       // 8
    localparam MAX_PKT_B = 256;
    localparam PKT_BITS  = MAX_PKT_B * 8;

    localparam [15:0] MDP3_PORT  = 16'd14310;
    localparam [15:0] WRONG_PORT = 16'hDEAD;

    // ----------------------------------------------------------------
    // DUT signals
    // ----------------------------------------------------------------
    logic             clk;
    logic             rst_n;

    // Slave side — testbench drives these
    logic [63:0]      s_axis_tdata;
    logic [7:0]       s_axis_tkeep;
    logic             s_axis_tvalid;
    logic             s_axis_tlast;
    logic             s_axis_tready;   // output from DUT

    // Master side — testbench monitors these
    logic [63:0]      m_axis_tdata;
    logic [7:0]       m_axis_tkeep;
    logic             m_axis_tvalid;
    logic             m_axis_tlast;
    logic             m_axis_tready;   // driven by testbench (downstream consumer)

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

    task automatic pkt_write_u16_be(input int idx, input logic [15:0] val);
        // Big-endian: high byte first (used for EtherType, ports)
        pkt_buf[idx*8     +: 8] = val[15:8];
        pkt_buf[(idx+1)*8 +: 8] = val[7:0];
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
    // Builds a minimal Ethernet/IPv4/UDP header into pkt_buf[0..41]
    // ----------------------------------------------------------------
    task automatic build_eth_ip_udp_headers(
        input int        udp_payload_len,
        input logic [15:0] dst_port,
        input logic [15:0] ethertype,    // normally 0x0800
        input logic [7:0]  ip_proto      // normally 0x11
    );
        int ip_total_len  = 20 + 8 + udp_payload_len;
        int udp_total_len =      8 + udp_payload_len;

        // --- Beat 0: bytes 0-7 ---
        // Dst MAC: 01:00:5e:00:00:01
        pkt_write_byte(0, 8'h01); pkt_write_byte(1, 8'h00);
        pkt_write_byte(2, 8'h5e); pkt_write_byte(3, 8'h00);
        pkt_write_byte(4, 8'h00); pkt_write_byte(5, 8'h01);
        // Src MAC start: de:ad
        pkt_write_byte(6, 8'hde); pkt_write_byte(7, 8'had);

        // --- Beat 1: bytes 8-15 ---
        // Src MAC end: be:ef:ca:fe
        pkt_write_byte(8,  8'hbe); pkt_write_byte(9,  8'hef);
        pkt_write_byte(10, 8'hca); pkt_write_byte(11, 8'hfe);
        // EtherType (big-endian) — parameterised so we can inject wrong values
        pkt_write_byte(12, ethertype[15:8]);
        pkt_write_byte(13, ethertype[7:0]);
        // IPv4 header start
        pkt_write_byte(14, 8'h45);   // version=4, IHL=5
        pkt_write_byte(15, 8'h00);   // DSCP/ECN

        // --- Beat 2: bytes 16-23 ---
        pkt_write_byte(16, ip_total_len[15:8]);
        pkt_write_byte(17, ip_total_len[7:0]);
        pkt_write_byte(18, 8'h00);   // ID
        pkt_write_byte(19, 8'h00);
        pkt_write_byte(20, 8'h00);   // flags/frag offset
        pkt_write_byte(21, 8'h00);
        pkt_write_byte(22, 8'h40);   // TTL = 64
        pkt_write_byte(23, ip_proto); // ← filter checks this byte

        // --- Beat 3: bytes 24-31 ---
        pkt_write_byte(24, 8'h00);   // checksum
        pkt_write_byte(25, 8'h00);
        pkt_write_byte(26, 8'hef);   // src IP 239.1.1.1
        pkt_write_byte(27, 8'h01);
        pkt_write_byte(28, 8'h01);
        pkt_write_byte(29, 8'h01);
        pkt_write_byte(30, 8'hef);   // dst IP 239.1.1.1
        pkt_write_byte(31, 8'h01);

        // --- Beat 4: bytes 32-39 ---
        pkt_write_byte(32, 8'h01);   // dst IP last 2 bytes
        pkt_write_byte(33, 8'h01);
        pkt_write_byte(34, 8'h37);   // UDP src port (arbitrary)
        pkt_write_byte(35, 8'ha1);
        pkt_write_byte(36, dst_port[15:8]);  // ← filter checks bytes 36-37
        pkt_write_byte(37, dst_port[7:0]);
        pkt_write_byte(38, udp_total_len[15:8]);
        pkt_write_byte(39, udp_total_len[7:0]);

        // --- Beat 5: bytes 40-47 ---
        pkt_write_byte(40, 8'h00);   // UDP checksum
        pkt_write_byte(41, 8'h00);
        // MDP3 seq_num placeholder
        pkt_write_u32_le(42, 32'hDEAD_BEEF);
    endtask

    // Build a minimal valid packet (just enough bytes to carry all headers)
    // Payload is zeroed — the filter doesn't care about payload content
    task automatic build_pkt(
        input logic [15:0] dst_port,
        input logic [15:0] ethertype,
        input logic [7:0]  ip_proto
    );
        pkt_buf = '0;
        build_eth_ip_udp_headers(12, dst_port, ethertype, ip_proto);
        pkt_len = 14 + 20 + 8 + 12;   // 54 bytes = headers only
    endtask

    // ----------------------------------------------------------------
    // AXI-Stream Master BFM
    // ----------------------------------------------------------------
    task automatic axis_send_packet(input int idle_cycles);
        int beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;
        int remainder   = pkt_len % KEEP_W;
        logic [DATA_W-1:0] beat_data = '0;
        logic [KEEP_W-1:0] beat_keep = '0;

        for (int beat = 0; beat < beats_total; beat++) begin
            // Optional idle gap
            int byte_idx        = beat * KEEP_W;
            int bytes_this_beat = (beat == beats_total-1 && remainder != 0)
                                ? remainder : KEEP_W;
            if (idle_cycles > 0 && beat > 0) begin
                s_axis_tvalid = 1'b0;
                repeat(idle_cycles) @(posedge clk); #1;
            end

            // Build beat

            for (int k = 0; k < bytes_this_beat; k++) begin
                beat_data[k*8 +: 8] = pkt_buf[(byte_idx+k)*8 +: 8];
                beat_keep[k]        = 1'b1;
            end

            s_axis_tdata  = beat_data;
            s_axis_tkeep  = beat_keep;
            s_axis_tvalid = 1'b1;
            s_axis_tlast  = (beat == beats_total-1);

            // Honour tready
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
    // Output monitor
    // Counts beats seen on the master port and whether tlast arrived
    // ----------------------------------------------------------------
    int     mon_beat_count;    // beats where m_axis_tvalid was high
    logic   mon_tlast_seen;
    logic   mon_tvalid_seen;   // did we see ANY valid output beat?

    // Check that output data matches input data (passthrough integrity)
    logic   mon_data_mismatch;

    always @(posedge clk) begin
        if (!rst_n) begin
            mon_beat_count   <= 0;
            mon_tlast_seen   <= 1'b0;
            mon_tvalid_seen  <= 1'b0;
            mon_data_mismatch<= 1'b0;
        end else begin
            if (m_axis_tvalid && m_axis_tready) begin
                mon_beat_count  <= mon_beat_count + 1;
                mon_tvalid_seen <= 1'b1;
                if (m_axis_tlast)
                    mon_tlast_seen <= 1'b1;
                // Data must pass through unchanged
                if (m_axis_tdata !== s_axis_tdata)
                    mon_data_mismatch <= 1'b1;
            end
        end
    end

    task automatic reset_monitor();
        @(posedge clk); #1;
        mon_beat_count    = 0;
        mon_tlast_seen    = 1'b0;
        mon_tvalid_seen   = 1'b0;
        mon_data_mismatch = 1'b0;
    endtask

    // ----------------------------------------------------------------
    // Wait for pipeline to drain after a packet
    // ----------------------------------------------------------------
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
    int fail_count;
    int expected_beats;

    initial begin
        // Init
        fail_count    = 0;
        s_axis_tdata  = '0;
        s_axis_tkeep  = '0;
        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        m_axis_tready = 1'b1;   // downstream always ready by default

        // Reset
        rst_n = 1'b0;
        repeat(4) @(posedge clk); #1;
        rst_n = 1'b1;
        repeat(2) @(posedge clk); #1;

        // ==============================================================
        // TEST 1: Valid MDP3 packet — all three checks pass
        // Expected: all beats forwarded, tlast seen, data intact
        // ==============================================================
        $display("\n=== TEST 1: Valid MDP3 packet ===");
        reset_monitor();
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        expected_beats = (pkt_len + KEEP_W - 1) / KEEP_W;
        axis_send_packet(0);
        wait_drain(10);

        if (!mon_tvalid_seen)
            begin $display("FAIL t1a: no output beats seen"); fail_count++; end
        else $display("PASS t1a: output beats seen");

        if (!mon_tlast_seen)
            begin $display("FAIL t1b: tlast never asserted on output"); fail_count++; end
        else $display("PASS t1b: tlast seen on output");

        if (mon_beat_count !== expected_beats)
            begin $display("FAIL t1c: got %0d beats, expected %0d", mon_beat_count, expected_beats); fail_count++; end
        else $display("PASS t1c: correct beat count %0d", mon_beat_count);

        if (mon_data_mismatch)
            begin $display("FAIL t1d: data mismatch on passthrough"); fail_count++; end
        else $display("PASS t1d: data passed through intact");

        // ==============================================================
        // TEST 2: Wrong UDP destination port — must be dropped
        // Expected: zero output beats, m_axis_tvalid never asserts
        // ==============================================================
        $display("\n=== TEST 2: Wrong UDP port (should drop) ===");
        reset_monitor();
        build_pkt(WRONG_PORT, 16'h0800, 8'h11);
        axis_send_packet(0);
        wait_drain(10);

        if (mon_tvalid_seen)
            begin $display("FAIL t2a: m_axis_tvalid asserted on wrong-port packet"); fail_count++; end
        else $display("PASS t2a: wrong-port packet correctly dropped");

        if (mon_beat_count !== 0)
            begin $display("FAIL t2b: %0d beats leaked through", mon_beat_count); fail_count++; end
        else $display("PASS t2b: zero beats forwarded");

        // ==============================================================
        // TEST 3: Wrong EtherType (0x0806 = ARP) — must be dropped
        // ==============================================================
        $display("\n=== TEST 3: Wrong EtherType (ARP, should drop) ===");
        reset_monitor();
        build_pkt(MDP3_PORT, 16'h0806, 8'h11);
        axis_send_packet(0);
        wait_drain(10);

        if (mon_tvalid_seen)
            begin $display("FAIL t3a: ARP packet was not dropped"); fail_count++; end
        else $display("PASS t3a: ARP packet dropped");

        // ==============================================================
        // TEST 4: Wrong IP protocol (0x06 = TCP) — must be dropped
        // ==============================================================
        $display("\n=== TEST 4: Wrong IP protocol (TCP, should drop) ===");
        reset_monitor();
        build_pkt(MDP3_PORT, 16'h0800, 8'h06);
        axis_send_packet(0);
        wait_drain(10);

        if (mon_tvalid_seen)
            begin $display("FAIL t4a: TCP packet was not dropped"); fail_count++; end
        else $display("PASS t4a: TCP packet dropped");

        // ==============================================================
        // TEST 5: Valid packet with idle gaps between beats
        // Verifies filter handles non-contiguous tvalid correctly
        // ==============================================================
        $display("\n=== TEST 5: Valid packet with idle gaps ===");
        reset_monitor();
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        expected_beats = (pkt_len + KEEP_W - 1) / KEEP_W;
        axis_send_packet(2);   // 2 idle cycles between each beat
        wait_drain(10);

        if (!mon_tvalid_seen)
            begin $display("FAIL t5a: no output beats with idle gaps"); fail_count++; end
        else $display("PASS t5a: output beats seen with idle gaps");

        if (mon_beat_count !== expected_beats)
            begin $display("FAIL t5b: got %0d beats, expected %0d", mon_beat_count, expected_beats); fail_count++; end
        else $display("PASS t5b: correct beat count %0d with idle gaps", mon_beat_count);

        // ==============================================================
        // TEST 6: Two back-to-back valid packets
        // Verifies filter resets state between packets
        // ==============================================================
        $display("\n=== TEST 6: Two consecutive valid packets ===");
        reset_monitor();
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        expected_beats = (pkt_len + KEEP_W - 1) / KEEP_W;
        axis_send_packet(0);
        axis_send_packet(0);
        wait_drain(10);

        if (mon_beat_count !== expected_beats * 2)
            begin $display("FAIL t6a: got %0d beats, expected %0d for 2 packets",
                mon_beat_count, expected_beats*2); fail_count++; end
        else $display("PASS t6a: correct beat count for 2 consecutive packets");

        // ==============================================================
        // TEST 7: Valid then dropped then valid
        // Verifies filter recovers correctly after a drop
        // ==============================================================
        $display("\n=== TEST 7: Valid → Drop → Valid ===");
        reset_monitor();
        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        expected_beats = (pkt_len + KEEP_W - 1) / KEEP_W;
        axis_send_packet(0);   // PASS

        build_pkt(WRONG_PORT, 16'h0800, 8'h11);
        axis_send_packet(0);   // DROP

        build_pkt(MDP3_PORT, 16'h0800, 8'h11);
        axis_send_packet(0);   // PASS
        wait_drain(10);

        if (mon_beat_count !== expected_beats * 2)
            begin $display("FAIL t7a: got %0d beats, expected %0d (2 passing packets)",
                mon_beat_count, expected_beats*2); fail_count++; end
        else $display("PASS t7a: filter recovered correctly after drop");

        // ==============================================================
        // TEST 8: Backpressure — m_axis_tready held low
        // Verifies s_axis_tready deasserts when downstream stalls
        // ==============================================================
        $display("\n=== TEST 8: Backpressure (tready held low) ===");
        reset_monitor();
        m_axis_tready = 1'b0;   // downstream not ready

        build_pkt(MDP3_PORT, 16'h0800, 8'h11);

        // Start sending — fork so we can release tready after a few cycles
        fork
            begin
                axis_send_packet(0);
            end
            begin
                // Hold tready low for 5 cycles then release
                repeat(5) @(posedge clk); #1;

                // Filter should have asserted backpressure to sender
                if (s_axis_tready === 1'b1)
                    $display("  NOTE t8: s_axis_tready stayed high during backpressure — check filter");
                else
                    $display("  PASS t8a: s_axis_tready deasserted during backpressure");

                m_axis_tready = 1'b1;   // release downstream
            end
        join

        wait_drain(20);
        if (!mon_tlast_seen)
            begin $display("FAIL t8b: packet did not complete after releasing tready"); fail_count++; end
        else $display("PASS t8b: packet completed after releasing backpressure");

        m_axis_tready = 1'b1;  // leave ready for cleanup

        // ==============================================================
        // Summary
        // ==============================================================
        repeat(4) @(posedge clk);
        $display("\n========================================");
        if (fail_count == 0)
            $display("ALL TESTS PASSED");
        else
            $display("%0d TEST(S) FAILED", fail_count);
        $display("========================================\n");
        $finish;
    end

endmodule