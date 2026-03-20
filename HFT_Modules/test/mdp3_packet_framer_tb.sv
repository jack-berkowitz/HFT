`timescale 1ns/1ps
// ============================================================
// mdp3_packet_framer_tb.sv
//
// Testbench for the MDP3 packet framer.
// Verifies header stripping and byte-lane realignment for
// HEADER_BYTES = 46 (OFFSET=6, REMAIN=2).
//
// TIMING NOTE:
//   The BFM drives inputs at posedge + #1 (models Tcq of the
//   upstream register).  The capture task samples outputs at
//   @(posedge clk) — the active region — which is BEFORE NBA
//   updates.  This matches how a real downstream flip-flop
//   would sample combinational AXI-Stream outputs.
//
//   Sampling at posedge + #1 would create a race between the
//   DUT's NBA update of prev_data_r and the BFM's #1 update
//   of s_axis_tdata — both modify the combinational barrel-
//   shift output at the same simulation time.
//
// Tests:
//   1. Single normal packet (79 bytes) — back-to-back beats
//   2. Single packet with idle gaps between beats
//   3. Runt packet (tlast before header consumed) — must be dropped
//   4. Minimum-payload packet (payload fits in boundary residual)
//   5. Two back-to-back packets — pipeline re-entry
//   6. Exact-beat-boundary payload (no extra flush beat needed)
//   7. Downstream backpressure
//   8. Large payload (128 bytes)
// ============================================================
`include "sys_defs.svh"

module mdp3_packet_framer_tb;

    // ----------------------------------------------------------------
    // Parameters
    // ----------------------------------------------------------------
    localparam DATA_W       = `DATA_W;
    localparam BYTE_W       = DATA_W / 8;
    localparam HEADER_BYTES = 46;
    localparam OFFSET       = HEADER_BYTES % BYTE_W;   // 6
    localparam REMAIN       = BYTE_W - OFFSET;          // 2
    localparam MAX_PKT_B    = 256;
    localparam PKT_BITS     = MAX_PKT_B * 8;

    // ----------------------------------------------------------------
    // Clock / Reset
    // ----------------------------------------------------------------
    logic clk, rst_n;
    initial clk = 0;
    always #2 clk = ~clk;   // 250 MHz

    // ----------------------------------------------------------------
    // DUT signals
    // ----------------------------------------------------------------
    AXI_TDATA   s_axis_tdata;
    AXI_TKEEP   s_axis_tkeep;
    logic       s_axis_tvalid;
    logic       s_axis_tlast;
    logic       s_axis_tready;

    AXI_TDATA   m_axis_tdata;
    AXI_TKEEP   m_axis_tkeep;
    logic       m_axis_tvalid;
    logic       m_axis_tlast;
    logic       m_axis_tready;

    mdp3_packet_framer #(
        .HEADER_BYTES (HEADER_BYTES)
    ) DUT (
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
    // Packet buffer — flat bit vector
    // ----------------------------------------------------------------
    logic [PKT_BITS-1:0] pkt_buf;
    integer              pkt_len;

    task automatic pkt_write_byte(input integer idx, input logic [7:0] val);
        pkt_buf[idx*8 +: 8] = val;
    endtask

    task automatic pkt_write_u16_le(input integer idx, input logic [15:0] val);
        pkt_buf[ idx   *8 +: 8] = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
    endtask

    task automatic pkt_write_u32_le(input integer idx, input logic [31:0] val);
        pkt_buf[ idx   *8 +: 8] = val[7:0];
        pkt_buf[(idx+1)*8 +: 8] = val[15:8];
        pkt_buf[(idx+2)*8 +: 8] = val[23:16];
        pkt_buf[(idx+3)*8 +: 8] = val[31:24];
    endtask

    task automatic pkt_write_u64_le(input integer idx, input logic [63:0] val);
        integer k;
        for (k = 0; k < 8; k = k + 1)
            pkt_buf[(idx+k)*8 +: 8] = val[k*8 +: 8];
    endtask

    // ----------------------------------------------------------------
    // Build a fake 46-byte network header (content doesn't matter,
    // the framer only counts beats — it doesn't inspect fields).
    // Fill with recognisable pattern so we can detect leakage.
    // ----------------------------------------------------------------
    task automatic build_header;
        integer i;
        for (i = 0; i < HEADER_BYTES; i = i + 1)
            pkt_write_byte(i, i[7:0]);  // 0x00, 0x01, ... 0x2D
    endtask

    // ----------------------------------------------------------------
    // Build a full test packet:
    //   [0:45]   header (filled with index pattern)
    //   [46+]    payload (filled with payload_tag + offset)
    // ----------------------------------------------------------------
    task automatic build_packet(
        input integer payload_len,
        input logic [7:0] payload_tag     // first payload byte
    );
        integer i;
        pkt_buf = '0;
        build_header();
        for (i = 0; i < payload_len; i = i + 1)
            pkt_write_byte(HEADER_BYTES + i, payload_tag + i[7:0]);
        pkt_len = HEADER_BYTES + payload_len;
    endtask

    // ----------------------------------------------------------------
    // AXI-Stream Master BFM — send pkt_buf as 64-bit beats
    //
    // Drives data via blocking assignments before @(posedge clk),
    // then advances at posedge + #1 to model upstream Tcq.
    // ----------------------------------------------------------------
    task automatic axis_send_packet(input integer idle_cycles);
        integer beats_total, bytes_this_beat, byte_idx;
        integer beat, k, remainder;
        logic [DATA_W-1:0] beat_data;
        logic [BYTE_W-1:0] beat_keep;

        beats_total = (pkt_len + BYTE_W - 1) / BYTE_W;
        remainder   = pkt_len % BYTE_W;

        for (beat = 0; beat < beats_total; beat = beat + 1) begin
            // Optional idle gap
            if (idle_cycles > 0 && beat > 0) begin
                s_axis_tvalid = 1'b0;
                repeat (idle_cycles) @(posedge clk);
                #1;
            end

            beat_data = '0;
            beat_keep = '0;
            byte_idx  = beat * BYTE_W;

            bytes_this_beat = ((beat == beats_total - 1) && remainder != 0)
                            ? remainder : BYTE_W;

            for (k = 0; k < bytes_this_beat; k = k + 1) begin
                beat_data[k*8 +: 8] = pkt_buf[(byte_idx + k)*8 +: 8];
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

    // ----------------------------------------------------------------
    // Output capture
    //
    // Samples at @(posedge clk) — the active region, BEFORE NBA.
    // This is where a real downstream register would sample the
    // combinational AXI-Stream outputs.  prev_data_r still holds
    // its OLD value, and s_axis_tdata is the value driven by the
    // BFM before this posedge, so combined_data is correct.
    // ----------------------------------------------------------------
    localparam MAX_OUT_BYTES = 256;
    logic [7:0] out_bytes [0:MAX_OUT_BYTES-1];
    integer     out_byte_cnt;
    logic       out_pkt_done;

    task automatic capture_output(input integer timeout_cycles);
        integer cyc, i;
        out_byte_cnt = 0;
        out_pkt_done = 0;

        for (cyc = 0; cyc < timeout_cycles && !out_pkt_done; cyc = cyc + 1) begin
            @(posedge clk);
            if (m_axis_tvalid && m_axis_tready) begin
                for (i = 0; i < BYTE_W; i = i + 1) begin
                    if (m_axis_tkeep[i]) begin
                        out_bytes[out_byte_cnt] = m_axis_tdata[i*8 +: 8];
                        out_byte_cnt = out_byte_cnt + 1;
                    end
                end
                if (m_axis_tlast)
                    out_pkt_done = 1;
            end
        end
    endtask

    // ----------------------------------------------------------------
    // Checker: compare captured output against expected payload
    // ----------------------------------------------------------------
    integer fail_count;

    task automatic check_payload(
        input integer       exp_len,
        input logic [7:0]   exp_tag     // expected first byte
    );
        integer i;
        logic [7:0] exp_byte;

        if (out_byte_cnt !== exp_len) begin
            $display("  FAIL: payload length mismatch: got %0d, expected %0d",
                     out_byte_cnt, exp_len);
            fail_count = fail_count + 1;
        end

        for (i = 0; i < exp_len && i < out_byte_cnt; i = i + 1) begin
            exp_byte = exp_tag + i[7:0];
            if (out_bytes[i] !== exp_byte) begin
                $display("  FAIL: byte[%0d] = 0x%02h, expected 0x%02h",
                         i, out_bytes[i], exp_byte);
                fail_count = fail_count + 1;
            end
        end
    endtask

    // Check that no output was produced (for drop tests)
    task automatic check_no_output(input integer wait_cycles);
        integer cyc;
        for (cyc = 0; cyc < wait_cycles; cyc = cyc + 1) begin
            @(posedge clk);
            if (m_axis_tvalid && m_axis_tready) begin
                $display("  FAIL: unexpected output beat received");
                fail_count = fail_count + 1;
                return;
            end
        end
    endtask

    // ----------------------------------------------------------------
    // VCD dump
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("mdp3_packet_framer_tb.vcd");
        $dumpvars(0, mdp3_packet_framer_tb);
    end

    // ----------------------------------------------------------------
    // Main test sequence
    // ----------------------------------------------------------------
    integer test_num;

    initial begin
        fail_count = 0;
        test_num   = 0;

        s_axis_tdata  = '0;
        s_axis_tkeep  = '0;
        s_axis_tvalid = 1'b0;
        s_axis_tlast  = 1'b0;
        m_axis_tready = 1'b1;    // downstream always ready by default

        rst_n = 0;
        repeat (4) @(posedge clk);
        #1;
        rst_n = 1;
        repeat (2) @(posedge clk);
        #1;

        // ============================================================
        // TEST 1: Normal 33-byte payload, back-to-back beats
        // ============================================================
        test_num = 1;
        fail_count = 0;
        $display("\n=== TEST %0d: Normal packet (33B payload, back-to-back) ===", test_num);
        build_packet(33, 8'hA0);
        fork
            axis_send_packet(0);
            capture_output(100);
        join
        check_payload(33, 8'hA0);
        if (out_pkt_done)
            $display("  tlast received: YES");
        else begin
            $display("  FAIL: tlast never received");
            fail_count = fail_count + 1;
        end
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // TEST 2: Normal packet with idle gaps between beats
        // ============================================================
        test_num = 2;
        fail_count = 0;
        $display("\n=== TEST %0d: Normal packet (33B payload, idle gaps) ===", test_num);
        build_packet(33, 8'hB0);
        fork
            axis_send_packet(2);
            capture_output(200);
        join
        check_payload(33, 8'hB0);
        if (out_pkt_done)
            $display("  tlast received: YES");
        else begin
            $display("  FAIL: tlast never received");
            fail_count = fail_count + 1;
        end
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // TEST 3: Runt packet — tlast on beat 3 (only 32 bytes)
        // ============================================================
        test_num = 3;
        fail_count = 0;
        $display("\n=== TEST %0d: Runt packet (32 bytes, expect drop) ===", test_num);
        pkt_buf = '0;
        begin
            integer i;
            for (i = 0; i < 32; i = i + 1)
                pkt_write_byte(i, 8'hDD);
        end
        pkt_len = 32;
        axis_send_packet(0);
        check_no_output(30);
        $display("  No spurious output: GOOD");
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // TEST 4: Minimum-payload packet — 2 bytes in boundary
        // ============================================================
        test_num = 4;
        fail_count = 0;
        $display("\n=== TEST %0d: Minimum payload (2 bytes in boundary) ===", test_num);
        build_packet(2, 8'hCC);
        fork
            axis_send_packet(0);
            capture_output(100);
        join
        check_payload(2, 8'hCC);
        if (out_pkt_done)
            $display("  tlast received: YES");
        else begin
            $display("  FAIL: tlast never received");
            fail_count = fail_count + 1;
        end
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // TEST 5: Two back-to-back packets
        // ============================================================
        test_num = 5;
        fail_count = 0;
        $display("\n=== TEST %0d: Two consecutive packets ===", test_num);

        // --- Packet A: 16-byte payload ---
        build_packet(16, 8'h10);
        fork
            axis_send_packet(0);
            capture_output(100);
        join
        check_payload(16, 8'h10);
        if (!out_pkt_done) begin
            $display("  FAIL: pkt A tlast missing");
            fail_count = fail_count + 1;
        end

        repeat (2) @(posedge clk); #1;

        // --- Packet B: 24-byte payload ---
        begin
            build_packet(24, 8'h20);
            fork
                axis_send_packet(0);
                capture_output(100);
            join
            check_payload(24, 8'h20);
            if (!out_pkt_done) begin
                $display("  FAIL: pkt B tlast missing");
                fail_count = fail_count + 1;
            end
        end
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // TEST 6: Payload that avoids the extra flush beat
        // ============================================================
        test_num = 6;
        fail_count = 0;
        $display("\n=== TEST %0d: No-extra-beat payload (6 bytes) ===", test_num);
        build_packet(6, 8'hE0);
        fork
            axis_send_packet(0);
            capture_output(100);
        join
        check_payload(6, 8'hE0);
        if (out_pkt_done)
            $display("  tlast received: YES");
        else begin
            $display("  FAIL: tlast never received");
            fail_count = fail_count + 1;
        end
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // TEST 7: Downstream backpressure
        // ============================================================
        test_num = 7;
        fail_count = 0;
        $display("\n=== TEST %0d: Downstream backpressure ===", test_num);
        build_packet(33, 8'hF0);
        fork
            axis_send_packet(0);
            capture_output(300);
            // Pulse backpressure: deassert tready for 5 cycles
            begin
                repeat (20) @(posedge clk); #1;
                m_axis_tready = 1'b0;
                repeat (5) @(posedge clk); #1;
                m_axis_tready = 1'b1;
            end
        join
        check_payload(33, 8'hF0);
        if (out_pkt_done)
            $display("  tlast received: YES");
        else begin
            $display("  FAIL: tlast never received");
            fail_count = fail_count + 1;
        end
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        repeat (4) @(posedge clk); #1;

        // ============================================================
        // TEST 8: Large payload (128 bytes)
        // ============================================================
        test_num = 8;
        fail_count = 0;
        $display("\n=== TEST %0d: Large payload (128 bytes) ===", test_num);
        build_packet(128, 8'h00);
        fork
            axis_send_packet(0);
            capture_output(200);
        join
        check_payload(128, 8'h00);
        if (out_pkt_done)
            $display("  tlast received: YES");
        else begin
            $display("  FAIL: tlast never received");
            fail_count = fail_count + 1;
        end
        $display("  %s", (fail_count == 0) ? "@@@ Passed" : "@@@ Failed");

        // ============================================================
        // Summary
        // ============================================================
        repeat (4) @(posedge clk);
        $display("\n========================================");
        $display("  All tests complete.");
        $display("========================================\n");
        $finish;
    end

endmodule