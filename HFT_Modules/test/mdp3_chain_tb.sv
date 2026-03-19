`timescale 1ns/1ps

// ============================================================
// mdp3_chain_tb.sv
//
// Full-chain testbench for:
//   ipv4_udp_port_filter -> mdp3_packet_framer -> sbe_template_demux
//
// AXI-Stream bus width : 64-bit (10G Corundum config)
// Output interface     : Flat decoded signals
//
// Packet memory layout injected into AXI-Stream (little-endian on wire):
//   [0 :13]  Ethernet header  (14 bytes)
//   [14:33]  IPv4 header      (20 bytes, no options)
//   [34:41]  UDP header        (8 bytes)
//   [42:45]  MDP3 packet header(4 bytes: seq_num)
//   [46:47]  SBE block_length  (2 bytes)
//   [48:49]  SBE template_id   (2 bytes)
//   [50:51]  SBE schema_id     (2 bytes)
//   [52:53]  SBE version       (2 bytes)
//   [54+ ]   SBE payload       (variable)
// ============================================================
module mdp3_chain_tb;

    // ----------------------------------------------------------------
    // Parameters
    // ----------------------------------------------------------------
    localparam DATA_W      = 64;           // AXI-Stream bus width (bits)
    localparam KEEP_W      = DATA_W / 8;   // tkeep width (bytes)
    localparam MAX_PKT_B   = 256;          // max packet size in bytes

    // CME MDP3 UDP port (standard multicast port)
    localparam [15:0] MDP3_PORT = 16'd14310;

    // SBE Template IDs (CME MDP3 spec)
    localparam [15:0] TMPL_NEW_ORDER  = 16'd38;   // MDIncrementalRefreshBook
    localparam [15:0] TMPL_MOD_ORDER  = 16'd39;
    localparam [15:0] TMPL_DEL_ORDER  = 16'd40;

    // ----------------------------------------------------------------
    // DUT interface signals
    // ----------------------------------------------------------------
    logic               clk, rst_n;

    // AXI-Stream input (from MAC)
    logic [DATA_W-1:0]  s_axis_tdata;
    logic [KEEP_W-1:0]  s_axis_tkeep;
    logic               s_axis_tvalid;
    logic               s_axis_tlast;
    logic               s_axis_tready;   // driven by DUT chain

    // Flat decoded output signals (from sbe_template_demux)
    logic               out_valid;         // a complete message was decoded
    logic [15:0]        out_template_id;
    logic [63:0]        out_order_id;
    logic [63:0]        out_price;         // price in CME fixed-point (mantissa)
    logic [31:0]        out_qty;
    logic               out_side;          // 0=bid, 1=ask
    logic [31:0]        out_security_id;

    // ----------------------------------------------------------------
    // DUT instantiation
    // NOTE: Replace this block once your RTL modules exist.
    //       The port names here define the expected interface contract.
    // ----------------------------------------------------------------
    // ----------------------------------------------------------------
    // Internal wires between the three stages
    // Named clearly to show which module drives them
    // ----------------------------------------------------------------

    // filter → framer
    logic [63:0]  filter_to_framer_tdata;
    logic [7:0]   filter_to_framer_tkeep;
    logic         filter_to_framer_tvalid;
    logic         filter_to_framer_tlast;
    logic         filter_to_framer_tready;  // driven by framer, consumed by filter

    // framer → demux
    logic [63:0]  framer_to_demux_tdata;
    logic [7:0]   framer_to_demux_tkeep;
    logic         framer_to_demux_tvalid;
    logic         framer_to_demux_tlast;
    logic         framer_to_demux_tready;  // driven by demux, consumed by framer

    // ----------------------------------------------------------------
    // Stage 1: IPv4/UDP Port Filter
    // Inspects EtherType, IP protocol, and UDP dst port
    // Suppresses tvalid on non-MDP3 packets
    // ----------------------------------------------------------------
    ipv4_udp_port_filter #(
        .FILTER_PORT    (MDP3_PORT)       // 16'd14310
    ) u_filter (
        .clk            (clk),
        .rst_n          (rst_n),

        // Slave port — from Corundum MAC RX
        .s_axis_tdata   (s_axis_tdata),
        .s_axis_tkeep   (s_axis_tkeep),
        .s_axis_tvalid  (s_axis_tvalid),
        .s_axis_tlast   (s_axis_tlast),
        .s_axis_tready  (s_axis_tready),  // backpressure to MAC

        // Master port — to framer (MDP3 packets only)
        .m_axis_tdata   (filter_to_framer_tdata),
        .m_axis_tkeep   (filter_to_framer_tkeep),
        .m_axis_tvalid  (filter_to_framer_tvalid),
        .m_axis_tlast   (filter_to_framer_tlast),
        .m_axis_tready  (filter_to_framer_tready)
    );

    // ----------------------------------------------------------------
    // Stage 2: MDP3 Packet Framer
    // Strips 46-byte network header (Eth+IPv4+UDP+MDP3 seq)
    // Handles split beat at header/payload boundary
    // Outputs clean SBE byte stream starting at byte 0
    // ----------------------------------------------------------------
    mdp3_packet_framer #(
        .HEADER_BYTES   (46)              // 14+20+8+4
    ) u_framer (
        .clk            (clk),
        .rst_n          (rst_n),

        // Slave port — from filter
        .s_axis_tdata   (filter_to_framer_tdata),
        .s_axis_tkeep   (filter_to_framer_tkeep),
        .s_axis_tvalid  (filter_to_framer_tvalid),
        .s_axis_tlast   (filter_to_framer_tlast),
        .s_axis_tready  (filter_to_framer_tready),  // backpressure to filter

        // Master port — to demux (pure SBE payload)
        .m_axis_tdata   (framer_to_demux_tdata),
        .m_axis_tkeep   (framer_to_demux_tkeep),
        .m_axis_tvalid  (framer_to_demux_tvalid),
        .m_axis_tlast   (framer_to_demux_tlast),
        .m_axis_tready  (framer_to_demux_tready)
    );

    // ----------------------------------------------------------------
    // Stage 3: SBE Template Demux
    // Reads template_id from SBE message header bytes 2-3
    // Routes payload to correct parser (Add/Mod/Del)
    // Outputs flat decoded trading fields
    // ----------------------------------------------------------------
    sbe_template_demux #(
        .TMPL_ADD       (16'd38),         // MDIncrementalRefreshBook Add
        .TMPL_MOD       (16'd39),         // MDIncrementalRefreshBook Modify
        .TMPL_DEL       (16'd40)          // MDIncrementalRefreshBook Delete
    ) u_demux (
        .clk            (clk),
        .rst_n          (rst_n),

        // Slave port — from framer (pure SBE payload)
        .s_axis_tdata   (framer_to_demux_tdata),
        .s_axis_tkeep   (framer_to_demux_tkeep),
        .s_axis_tvalid  (framer_to_demux_tvalid),
        .s_axis_tlast   (framer_to_demux_tlast),
        .s_axis_tready  (framer_to_demux_tready),  // backpressure to framer

        // Flat decoded outputs — to Team B KV lookup
        .out_valid       (out_valid),
        .out_template_id (out_template_id),
        .out_order_id    (out_order_id),
        .out_price       (out_price),
        .out_qty         (out_qty),
        .out_side        (out_side),
        .out_security_id (out_security_id),

        // Parser enable routing (optional — only needed if parsers are separate modules)
        .enable_add      (enable_add),
        .enable_mod      (enable_mod),
        .enable_del      (enable_del)
    );

    // ----------------------------------------------------------------
    // Clock: 250 MHz (standard Corundum user logic clock)
    // ----------------------------------------------------------------
    initial clk = 0;
    always #2 clk = ~clk;   // 4ns period = 250 MHz

    // ----------------------------------------------------------------
    // Packet buffer — byte array, Icarus-safe (flat logic vector)
    // Max 256 bytes = 2048 bits
    // ----------------------------------------------------------------
    localparam PKT_BITS = MAX_PKT_B * 8;
    logic [PKT_BITS-1:0] pkt_buf;
    integer              pkt_len;   // valid bytes in pkt_buf

    // Helper: write one byte into pkt_buf at position idx
    task pkt_write_byte;
        input integer       idx;
        input logic [7:0]   val;
        pkt_buf[idx*8 +: 8] = val;
    endtask

    // Helper: write a 16-bit little-endian word
    task pkt_write_u16_le;
        input integer        idx;
        input logic [15:0]   val;
        pkt_buf[idx*8     +: 8] = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
    endtask

    // Helper: write a 32-bit little-endian word
    task pkt_write_u32_le;
        input integer        idx;
        input logic [31:0]   val;
        pkt_buf[idx*8     +: 8] = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
        pkt_buf[(idx+2)*8 +: 8] = val[23:16];
        pkt_buf[(idx+3)*8 +: 8] = val[31:24];
    endtask

    // Helper: write a 64-bit little-endian word
    task pkt_write_u64_le;
        input integer        idx;
        input logic [63:0]   val;
        integer k;
        for (k = 0; k < 8; k = k+1)
            pkt_buf[(idx+k)*8 +: 8] = val[k*8 +: 8];
    endtask

    // ----------------------------------------------------------------
    // Ethernet / IP / UDP header builder
    //
    // Builds a minimal real header into pkt_buf[0..41]
    // udp_payload_len = number of bytes AFTER the UDP header
    // ----------------------------------------------------------------
    task build_eth_ip_udp_headers;
        input integer        udp_payload_len;
        input logic [15:0]   dst_port;
        integer ip_total_len;
        integer udp_total_len;
        integer k;
        ip_total_len  = 20 + 8 + udp_payload_len;
        udp_total_len =      8 + udp_payload_len;

        // --- Ethernet header (14 bytes, indices 0-13) ---
        // Dst MAC: 01:00:5e:00:00:01  (multicast)
        pkt_write_byte(0,  8'h01);
        pkt_write_byte(1,  8'h00);
        pkt_write_byte(2,  8'h5e);
        pkt_write_byte(3,  8'h00);
        pkt_write_byte(4,  8'h00);
        pkt_write_byte(5,  8'h01);
        // Src MAC: de:ad:be:ef:ca:fe
        pkt_write_byte(6,  8'hde);
        pkt_write_byte(7,  8'had);
        pkt_write_byte(8,  8'hbe);
        pkt_write_byte(9,  8'hef);
        pkt_write_byte(10, 8'hca);
        pkt_write_byte(11, 8'hfe);
        // EtherType: 0x0800 (IPv4) — big-endian
        pkt_write_byte(12, 8'h08);
        pkt_write_byte(13, 8'h00);

        // --- IPv4 header (20 bytes, indices 14-33) ---
        pkt_write_byte(14, 8'h45);           // version=4, IHL=5
        pkt_write_byte(15, 8'h00);           // DSCP/ECN
        pkt_write_byte(16, ip_total_len[15:8]);  // total length MSB
        pkt_write_byte(17, ip_total_len[7:0]);   // total length LSB
        pkt_write_byte(18, 8'h00);           // ID
        pkt_write_byte(19, 8'h00);
        pkt_write_byte(20, 8'h00);           // flags/fragment offset
        pkt_write_byte(21, 8'h00);
        pkt_write_byte(22, 8'h40);           // TTL = 64
        pkt_write_byte(23, 8'h11);           // protocol = UDP
        pkt_write_byte(24, 8'h00);           // checksum (zeroed for sim)
        pkt_write_byte(25, 8'h00);
        pkt_write_byte(26, 8'hef);           // src IP: 239.1.1.1
        pkt_write_byte(27, 8'h01);
        pkt_write_byte(28, 8'h01);
        pkt_write_byte(29, 8'h01);
        pkt_write_byte(30, 8'hef);           // dst IP: 239.1.1.1
        pkt_write_byte(31, 8'h01);
        pkt_write_byte(32, 8'h01);
        pkt_write_byte(33, 8'h01);

        // --- UDP header (8 bytes, indices 34-41) ---
        pkt_write_byte(34, 8'h37);           // src port MSB (arbitrary)
        pkt_write_byte(35, 8'ha1);           // src port LSB
        pkt_write_byte(36, dst_port[15:8]);  // dst port MSB
        pkt_write_byte(37, dst_port[7:0]);   // dst port LSB
        pkt_write_byte(38, udp_total_len[15:8]);
        pkt_write_byte(39, udp_total_len[7:0]);
        pkt_write_byte(40, 8'h00);           // checksum (zeroed)
        pkt_write_byte(41, 8'h00);
    endtask

    // ----------------------------------------------------------------
    // MDP3 + SBE header builder (starts at byte 42)
    //
    // Writes:
    //   [42-45] MDP3 packet header: seq_num (4 bytes LE)
    //   [46-47] SBE block_length   (2 bytes LE)
    //   [48-49] SBE template_id    (2 bytes LE)
    //   [50-51] SBE schema_id      (2 bytes LE)  hardcoded = 1
    //   [52-53] SBE version        (2 bytes LE)  hardcoded = 8
    // Returns: next free byte index (54)
    // ----------------------------------------------------------------
    task build_mdp3_sbe_header;
        input logic [31:0]   seq_num;
        input logic [15:0]   template_id;
        input logic [15:0]   block_length;
        pkt_write_u32_le(42, seq_num);
        pkt_write_u16_le(46, block_length);
        pkt_write_u16_le(48, template_id);
        pkt_write_u16_le(50, 16'd1);    // schema_id
        pkt_write_u16_le(52, 16'd8);    // version
    endtask

    // ----------------------------------------------------------------
    // Build a complete Add Order (New) message
    // SBE payload (from byte 54):
    //   [54-61]  order_id     (8 bytes LE)
    //   [62-69]  price        (8 bytes LE, mantissa only)
    //   [70-73]  qty          (4 bytes LE)
    //   [74-77]  security_id  (4 bytes LE)
    //   [78]     side         (1 byte: 0=bid, 1=ask)
    // Total payload after UDP = 4 + 8 + (54-42) = MDP3_hdr + SBE_hdr + fields
    // ----------------------------------------------------------------
    task build_add_order_pkt;
        input logic [31:0]   seq_num;
        input logic [63:0]   order_id;
        input logic [63:0]   price;
        input logic [31:0]   qty;
        input logic [31:0]   security_id;
        input logic          side;

        localparam SBE_PAYLOAD_LEN = 25;   // fields after SBE msg header
        localparam SBE_MSG_HDR_LEN = 8;    // block_len+tmpl+schema+ver
        localparam MDP3_HDR_LEN    = 4;    // seq_num
        localparam UDP_DATA_LEN    = MDP3_HDR_LEN + SBE_MSG_HDR_LEN + SBE_PAYLOAD_LEN;

        pkt_buf = '0;
        build_eth_ip_udp_headers(UDP_DATA_LEN, MDP3_PORT);
        build_mdp3_sbe_header(seq_num, TMPL_NEW_ORDER, SBE_PAYLOAD_LEN);
        pkt_write_u64_le(54, order_id);
        pkt_write_u64_le(62, price);
        pkt_write_u32_le(70, qty);
        pkt_write_u32_le(74, security_id);
        pkt_write_byte  (78, {7'b0, side});
        pkt_len = 14 + 20 + 8 + UDP_DATA_LEN;  // 79 bytes total
    endtask

    // ----------------------------------------------------------------
    // AXI-Stream Master BFM
    // Streams pkt_buf as 64-bit beats with correct tkeep and tlast
    // Inserts 0-2 idle cycles between beats to stress tvalid toggling
    // ----------------------------------------------------------------
    integer beat, byte_idx, bytes_this_beat, k;
    logic [DATA_W-1:0]  beat_data;
    logic [KEEP_W-1:0]  beat_keep;

    task axis_send_packet;
        input integer idle_cycles;   // idle beats between valid bursts (0=back-to-back)
        integer beats_total;
        integer remainder;

        beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;  // ceil(pkt_len / 8)
        remainder   = pkt_len % KEEP_W;

        for (beat = 0; beat < beats_total; beat = beat+1) begin
            // Optional idle cycles
            if (idle_cycles > 0 && beat > 0) begin
                s_axis_tvalid = 1'b0;
                repeat(idle_cycles) @(posedge clk); #1;
            end

            // Build beat data (little-endian: byte 0 on LSB)
            beat_data = '0;
            beat_keep = '0;
            byte_idx  = beat * KEEP_W;

            if (beat == beats_total - 1) begin
                // Last beat: only fill valid bytes
                bytes_this_beat = (remainder == 0) ? KEEP_W : remainder;
            end else begin
                bytes_this_beat = KEEP_W;
            end

            for (k = 0; k < bytes_this_beat; k = k+1) begin
                beat_data[k*8 +: 8] = pkt_buf[(byte_idx+k)*8 +: 8];
                beat_keep[k]        = 1'b1;
            end

            // Drive AXI-Stream signals
            s_axis_tdata  = beat_data;
            s_axis_tkeep  = beat_keep;
            s_axis_tvalid = 1'b1;
            s_axis_tlast  = (beat == beats_total - 1) ? 1'b1 : 1'b0;

            // Wait for tready
            @(posedge clk);
            while (!s_axis_tready) @(posedge clk);
            #1;
        end

        // Deassert after packet
        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        s_axis_tkeep  = '0;
        s_axis_tdata  = '0;
    endtask

    // ----------------------------------------------------------------
    // Output capture — latch decoded fields when out_valid pulses
    // ----------------------------------------------------------------
    logic [15:0] cap_template_id;
    logic [63:0] cap_order_id;
    logic [63:0] cap_price;
    logic [31:0] cap_qty;
    logic        cap_side;
    logic [31:0] cap_security_id;
    integer      cap_count;

    always @(posedge clk) begin
        if (out_valid) begin
            cap_template_id <= out_template_id;
            cap_order_id    <= out_order_id;
            cap_price       <= out_price;
            cap_qty         <= out_qty;
            cap_side        <= out_side;
            cap_security_id <= out_security_id;
            cap_count       <= cap_count + 1;
        end
    end

    // ----------------------------------------------------------------
    // VCD
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("mdp3_chain_dump.vcd");
        $dumpvars(0, mdp3_chain_tb);
    end

    // ----------------------------------------------------------------
    // Scoreboard task — compare captured output vs expected
    // ----------------------------------------------------------------
    integer fail_count;

    task check_output;
        input [15:0]  exp_template;
        input [63:0]  exp_order_id;
        input [63:0]  exp_price;
        input [31:0]  exp_qty;
        input         exp_side;
        input [31:0]  exp_sec_id;

        // Wait up to 200 cycles for out_valid to pulse
        integer timeout;
        timeout = 0;
        while (!out_valid && timeout < 200) begin
            @(posedge clk); #1;
            timeout = timeout + 1;
        end

        if (timeout >= 200) begin
            $display("FAIL: out_valid never asserted (timeout)");
            fail_count = fail_count + 1;
        end else begin
            if (cap_template_id !== exp_template)
                begin $display("FAIL template_id: got %0d exp %0d", cap_template_id, exp_template); fail_count=fail_count+1; end
            if (cap_order_id !== exp_order_id)
                begin $display("FAIL order_id: got %h exp %h", cap_order_id, exp_order_id); fail_count=fail_count+1; end
            if (cap_price !== exp_price)
                begin $display("FAIL price: got %0d exp %0d", cap_price, exp_price); fail_count=fail_count+1; end
            if (cap_qty !== exp_qty)
                begin $display("FAIL qty: got %0d exp %0d", cap_qty, exp_qty); fail_count=fail_count+1; end
            if (cap_side !== exp_side)
                begin $display("FAIL side: got %0d exp %0d", cap_side, exp_side); fail_count=fail_count+1; end
            if (cap_security_id !== exp_sec_id)
                begin $display("FAIL security_id: got %0d exp %0d", cap_security_id, exp_sec_id); fail_count=fail_count+1; end
            if (fail_count == 0)
                $display("PASS: template=%0d order_id=%h price=%0d qty=%0d side=%0d sec=%0d",
                    cap_template_id, cap_order_id, cap_price, cap_qty, cap_side, cap_security_id);
        end
    endtask

    // ----------------------------------------------------------------
    // Main test sequence
    // ----------------------------------------------------------------
    initial begin
        fail_count  = 0;
        cap_count   = 0;
        s_axis_tdata  = '0;
        s_axis_tkeep  = '0;
        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        pkt_buf       = '0;
        pkt_len       = 0;

        rst_n = 0;
        repeat(4) @(posedge clk); #1;
        rst_n = 1;
        repeat(2) @(posedge clk); #1;

        // ============================================================
        // TEST 1: Valid Add Order — back-to-back beats
        // Expected: out_valid pulses, all fields decoded correctly
        // ============================================================
        $display("\n=== TEST 1: Valid Add Order (back-to-back beats) ===");
        build_add_order_pkt(
            32'd1001,             // seq_num
            64'hDEAD_0000_0000_0001, // order_id
            64'd987500,           // price (98.75 in fixed-point)
            32'd100,              // qty
            32'd12345,            // security_id
            1'b0                  // side: bid
        );
        axis_send_packet(0);      // 0 idle cycles = back-to-back
        check_output(TMPL_NEW_ORDER, 64'hDEAD_0000_0000_0001,
                     64'd987500, 32'd100, 1'b0, 32'd12345);

        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 2: Valid Add Order — with idle gaps between beats
        // Verifies framer handles non-continuous tvalid correctly
        // ============================================================
        $display("\n=== TEST 2: Add Order with idle gaps ===");
        build_add_order_pkt(
            32'd1002,
            64'hBEEF_0000_0000_0002,
            64'd1005000,          // 100.50
            32'd250,
            32'd12345,
            1'b1                  // side: ask
        );
        axis_send_packet(2);      // 2 idle cycles between each beat
        check_output(TMPL_NEW_ORDER, 64'hBEEF_0000_0000_0002,
                     64'd1005000, 32'd250, 1'b1, 32'd12345);

        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 3: Wrong UDP port — packet must be DROPPED
        // out_valid must NOT assert
        // ============================================================
        $display("\n=== TEST 3: Wrong UDP port (should be dropped) ===");
        build_add_order_pkt(32'd1003, 64'hAAAA, 64'd100, 32'd1, 32'd1, 1'b0);
        // Patch the destination port to something wrong
        pkt_write_byte(36, 8'hDE);
        pkt_write_byte(37, 8'hAD);
        axis_send_packet(0);

        // Give the pipeline time to process, then check no output
        repeat(30) @(posedge clk); #1;
        if (out_valid)
            begin $display("FAIL t3: out_valid asserted on wrong-port packet"); fail_count=fail_count+1; end
        else
            $display("PASS: wrong-port packet correctly dropped");

        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 4: Two back-to-back packets — pipeline not stalled
        // ============================================================
        $display("\n=== TEST 4: Two consecutive packets ===");
        // Packet A
        build_add_order_pkt(32'd1004, 64'hCAFE_0000_0001, 64'd500000, 32'd50, 32'd99, 1'b0);
        axis_send_packet(0);
        // Packet B immediately after
        build_add_order_pkt(32'd1005, 64'hCAFE_0000_0002, 64'd500100, 32'd75, 32'd99, 1'b1);
        axis_send_packet(0);

        check_output(TMPL_NEW_ORDER, 64'hCAFE_0000_0001, 64'd500000, 32'd50, 1'b0, 32'd99);
        check_output(TMPL_NEW_ORDER, 64'hCAFE_0000_0002, 64'd500100, 32'd75, 1'b1, 32'd99);

        // ============================================================
        // Summary
        // ============================================================
        repeat(4) @(posedge clk);
        $display("\n========================================");
        $display("Total packets decoded : %0d", cap_count);
        if (fail_count == 0)
            $display("ALL TESTS PASSED");
        else
            $display("%0d TEST(S) FAILED", fail_count);
        $display("========================================\n");
        $finish;
    end

endmodule