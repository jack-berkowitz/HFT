`timescale 1ns/1ps
`include "sys_defs.svh"
// ============================================================
// xdp_chain_tb.sv
//
// Full-chain testbench:
//   ipv4_udp_port_filter → xdp_packet_framer → xdp_msg_demux
//
// Output: pillar_msg_t struct (sys_defs.svh)
//
// Pretty-prints:
//   - INPUT:  what was injected into the pipeline
//   - OUTPUT: what the demux decoded (pillar_msg_t fields)
//   - RESULT: per-field pass/fail, then overall PASS/FAIL
// ============================================================
module xdp_chain_tb;

    localparam DATA_W    = 64;
    localparam KEEP_W    = DATA_W / 8;
    localparam MAX_PKT_B = 256;
    localparam PKT_BITS  = MAX_PKT_B * 8;
    localparam [15:0] XDP_PORT = 16'd11101;

    // ----------------------------------------------------------------
    // DUT signals
    // ----------------------------------------------------------------
    logic              clk, rst_n;
    logic [DATA_W-1:0] s_axis_tdata;
    logic [KEEP_W-1:0] s_axis_tkeep;
    logic              s_axis_tvalid, s_axis_tlast, s_axis_tready;
    pillar_msg_t       decoded;

    // Internal wires
    logic [63:0] filt2frm_tdata;  logic [7:0] filt2frm_tkeep;
    logic        filt2frm_tvalid, filt2frm_tlast, filt2frm_tready;
    logic [63:0] frm2dmx_tdata;   logic [7:0] frm2dmx_tkeep;
    logic        frm2dmx_tvalid, frm2dmx_tlast, frm2dmx_tready;

    // ----------------------------------------------------------------
    // DUT chain
    // ----------------------------------------------------------------
    ipv4_udp_port_filter #(.FILTER_PORT(XDP_PORT)) u_filter (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(s_axis_tdata),   .s_axis_tkeep(s_axis_tkeep),
        .s_axis_tvalid(s_axis_tvalid), .s_axis_tlast(s_axis_tlast),
        .s_axis_tready(s_axis_tready),
        .m_axis_tdata(filt2frm_tdata),   .m_axis_tkeep(filt2frm_tkeep),
        .m_axis_tvalid(filt2frm_tvalid), .m_axis_tlast(filt2frm_tlast),
        .m_axis_tready(filt2frm_tready)
    );

    xdp_packet_framer #(.HEADER_BYTES(`XDP_CHAIN_FRAMER_HDR)) u_framer (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(filt2frm_tdata),   .s_axis_tkeep(filt2frm_tkeep),
        .s_axis_tvalid(filt2frm_tvalid), .s_axis_tlast(filt2frm_tlast),
        .s_axis_tready(filt2frm_tready),
        .m_axis_tdata(frm2dmx_tdata),   .m_axis_tkeep(frm2dmx_tkeep),
        .m_axis_tvalid(frm2dmx_tvalid), .m_axis_tlast(frm2dmx_tlast),
        .m_axis_tready(frm2dmx_tready)
    );

    xdp_msg_demux u_demux (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(frm2dmx_tdata),   .s_axis_tkeep(frm2dmx_tkeep),
        .s_axis_tvalid(frm2dmx_tvalid), .s_axis_tlast(frm2dmx_tlast),
        .s_axis_tready(frm2dmx_tready),
        .out_msg(decoded)
    );

    // ----------------------------------------------------------------
    // Clock
    // ----------------------------------------------------------------
    initial clk = 0;
    always #2 clk = ~clk;

    // ----------------------------------------------------------------
    // Pipeline debug monitors (set debug_en=1 to enable)
    // ----------------------------------------------------------------
    logic debug_en;
    initial debug_en = 0;

    always @(posedge clk) if (debug_en && filt2frm_tvalid && filt2frm_tready)
        $display("  [FILT->FRM] %0tps L=%b data=%h keep=%b",
                 $time, filt2frm_tlast, filt2frm_tdata, filt2frm_tkeep);
    always @(posedge clk) if (debug_en && frm2dmx_tvalid && frm2dmx_tready)
        $display("  [FRM->DMX]  %0tps L=%b data=%h keep=%b",
                 $time, frm2dmx_tlast, frm2dmx_tdata, frm2dmx_tkeep);
    always @(posedge clk) if (debug_en && (filt2frm_tvalid || frm2dmx_tvalid))
        $display("  [FRM STATE] %0tps st=%0d bcnt=%0d plast=%b",
                 $time, u_framer.state_r, u_framer.beat_cnt_r, u_framer.prev_last_r);
    always @(posedge clk) if (debug_en && (frm2dmx_tvalid || decoded.valid))
        $display("  [DMX STATE] %0tps st=%0d bcnt=%0d type=%0d",
                 $time, u_demux.state_r, u_demux.beat_cnt_r, u_demux.msg_type_r);

    // ================================================================
    //  PRETTY PRINT HELPERS
    // ================================================================

    function string msg_type_name(input [15:0] t);
        case (t)
            16'd100: return "ADD ORDER";
            16'd101: return "MODIFY ORDER";
            16'd102: return "DELETE ORDER";
            16'd103: return "ORDER EXECUTION";
            16'd104: return "REPLACE ORDER";
            default: return "UNKNOWN";
        endcase
    endfunction

    function string side_str(input [7:0] s);
        case (s)
            "B":     return "BUY";
            "S":     return "SELL";
            default: return "---";
        endcase
    endfunction

    // Print what we injected
    task print_input(
        input string     test_name,
        input [15:0]     msg_type,
        input [31:0]     sym_idx,
        input [63:0]     order_id,
        input [31:0]     price,
        input [31:0]     qty,
        input [7:0]      side,
        input [63:0]     new_oid,
        input [31:0]     trade_id
    );
        $display("");
        $display("  ┌─────────────────────────────────────────────────────────────┐");
        $display("  │ %s", test_name);
        $display("  ├─────────────────────────────────────────────────────────────┤");
        $display("  │ INPUT (injected into AXI-Stream)                           │");
        $display("  │   msg_type      : %0d (%s)", msg_type, msg_type_name(msg_type));
        $display("  │   symbol_index  : %0d", sym_idx);
        $display("  │   order_id      : 0x%016h", order_id);
        if (msg_type == 16'd104)
        $display("  │   new_order_id  : 0x%016h", new_oid);
        if (msg_type == 16'd103)
        $display("  │   trade_id      : %0d", trade_id);
        if (msg_type != 16'd102) begin
        $display("  │   price         : %0d", price);
        $display("  │   qty           : %0d", qty);
        end
        if (msg_type == 16'd100)
        $display("  │   side          : %s", side_str(side));
        $display("  ├─────────────────────────────────────────────────────────────┤");
    endtask

    // Print what the demux decoded
    task print_output();
        $display("  │ OUTPUT (pillar_msg_t from demux)                           │");
        $display("  │   .valid         : %0d", decoded.valid);
        $display("  │   .msg_type      : %0d (%s)", decoded.msg_type, msg_type_name(decoded.msg_type));
        $display("  │   .symbol_index  : %0d", decoded.symbol_index);
        $display("  │   .symbol_seq_num: %0d", decoded.symbol_seq_num);
        $display("  │   .order_id      : 0x%016h", decoded.order_id);
        if (decoded.msg_type == 16'd104)
        $display("  │   .new_order_id  : 0x%016h", decoded.new_order_id);
        if (decoded.msg_type == 16'd103) begin
        $display("  │   .trade_id      : %0d", decoded.trade_id);
        $display("  │   .printable     : %0d", decoded.printable);
        end
        if (decoded.msg_type != 16'd102) begin
        $display("  │   .price         : %0d", decoded.price);
        $display("  │   .qty           : %0d", decoded.qty);
        end
        if (decoded.msg_type == 16'd100)
        $display("  │   .side          : 0x%02h (%s)", decoded.side, side_str(decoded.side));
        $display("  ├─────────────────────────────────────────────────────────────┤");
    endtask

    task print_result(input integer errs);
        if (errs == 0)
        $display("  │ RESULT: PASS                                               │");
        else
        $display("  │ RESULT: FAIL (%0d field mismatches)                        │", errs);
        $display("  └─────────────────────────────────────────────────────────────┘");
    endtask

    task print_drop_result(input logic was_dropped);
        $display("  │ OUTPUT: %s                                     │",
                 was_dropped ? "correctly dropped (no valid pulse)" : "ERROR: valid pulsed!");
        $display("  │ RESULT: %s                                               │",
                 was_dropped ? "PASS" : "FAIL");
        $display("  └─────────────────────────────────────────────────────────────┘");
    endtask

    // ----------------------------------------------------------------
    // Packet buffer helpers
    // ----------------------------------------------------------------
    logic [PKT_BITS-1:0] pkt_buf;
    integer              pkt_len;

    task pkt_write_byte(input integer idx, input logic [7:0] val);
        pkt_buf[idx*8 +: 8] = val;
    endtask
    task pkt_write_u16_be(input integer idx, input logic [15:0] val);
        pkt_buf[idx*8 +: 8] = val[15:8]; pkt_buf[(idx+1)*8 +: 8] = val[7:0];
    endtask
    task pkt_write_u16_le(input integer idx, input logic [15:0] val);
        pkt_buf[idx*8 +: 8] = val[7:0]; pkt_buf[(idx+1)*8 +: 8] = val[15:8];
    endtask
    task pkt_write_u32_le(input integer idx, input logic [31:0] val);
        pkt_buf[idx*8 +: 8] = val[7:0];     pkt_buf[(idx+1)*8 +: 8] = val[15:8];
        pkt_buf[(idx+2)*8 +: 8] = val[23:16]; pkt_buf[(idx+3)*8 +: 8] = val[31:24];
    endtask
    task pkt_write_u64_le(input integer idx, input logic [63:0] val);
        for (int kk = 0; kk < 8; kk++) pkt_buf[(idx+kk)*8 +: 8] = val[kk*8 +: 8];
    endtask

    // ----------------------------------------------------------------
    // Network + XDP packet header builders
    // ----------------------------------------------------------------
    task build_net_hdr(input integer udp_payload_len);
        integer ip_total_len, udp_total_len;
        ip_total_len  = 20 + 8 + udp_payload_len;
        udp_total_len =      8 + udp_payload_len;
        pkt_write_byte(0,8'h01);  pkt_write_byte(1,8'h00);  pkt_write_byte(2,8'h5e);
        pkt_write_byte(3,8'h00);  pkt_write_byte(4,8'h00);  pkt_write_byte(5,8'h01);
        pkt_write_byte(6,8'hde);  pkt_write_byte(7,8'had);  pkt_write_byte(8,8'hbe);
        pkt_write_byte(9,8'hef);  pkt_write_byte(10,8'hca); pkt_write_byte(11,8'hfe);
        pkt_write_byte(12,8'h08); pkt_write_byte(13,8'h00);
        pkt_write_byte(14,8'h45); pkt_write_byte(15,8'h00);
        pkt_write_byte(16,ip_total_len[15:8]); pkt_write_byte(17,ip_total_len[7:0]);
        pkt_write_byte(18,8'h00); pkt_write_byte(19,8'h00);
        pkt_write_byte(20,8'h00); pkt_write_byte(21,8'h00);
        pkt_write_byte(22,8'h40); pkt_write_byte(23,8'h11);
        pkt_write_byte(24,8'h00); pkt_write_byte(25,8'h00);
        pkt_write_byte(26,8'hef); pkt_write_byte(27,8'h01);
        pkt_write_byte(28,8'h01); pkt_write_byte(29,8'h01);
        pkt_write_byte(30,8'hef); pkt_write_byte(31,8'h01);
        pkt_write_byte(32,8'h01); pkt_write_byte(33,8'h01);
        pkt_write_byte(34,8'hCA); pkt_write_byte(35,8'hFE);
        pkt_write_u16_be(36, XDP_PORT);
        pkt_write_byte(38,udp_total_len[15:8]); pkt_write_byte(39,udp_total_len[7:0]);
        pkt_write_byte(40,8'h00); pkt_write_byte(41,8'h00);
    endtask

    task build_xdp_pkt_hdr(input [15:0] pkt_sz, input [7:0] n_msgs, input [31:0] seq);
        pkt_write_u16_le(42, pkt_sz); pkt_write_byte(44, 8'd11); pkt_write_byte(45, n_msgs);
        pkt_write_u32_le(46, seq); pkt_write_u32_le(50, 32'h5F3759DF); pkt_write_u32_le(54, 32'h00001234);
    endtask

    task build_msg_common(input [15:0] sz, input [15:0] typ,
                          input [31:0] sym, input [31:0] ssn, input [63:0] oid);
        pkt_write_u16_le(58, sz); pkt_write_u16_le(60, typ); pkt_write_u32_le(62, 32'hAAAA_BBBB);
        pkt_write_u32_le(66, sym); pkt_write_u32_le(70, ssn); pkt_write_u64_le(74, oid);
    endtask

    // ----------------------------------------------------------------
    // Per-type packet builders
    // ----------------------------------------------------------------
    task build_add_order(input [31:0] seq, input [63:0] oid, input [31:0] price,
                         input [31:0] qty, input [31:0] sym, input [7:0] side);
        localparam SZ = 39;
        pkt_buf = '0; build_net_hdr(16+SZ); build_xdp_pkt_hdr(16+SZ, 1, seq);
        build_msg_common(SZ, 16'd100, sym, 32'd1, oid);
        pkt_write_u32_le(82, price); pkt_write_u32_le(86, qty); pkt_write_byte(90, side);
        pkt_write_byte(91,"A"); pkt_write_byte(92,"B"); pkt_write_byte(93,"C");
        pkt_write_byte(94,"D"); pkt_write_byte(95,"E"); pkt_write_byte(96, 0);
        pkt_len = 42+16+SZ;
    endtask

    task build_mod_order(input [31:0] seq, input [63:0] oid, input [31:0] price,
                         input [31:0] qty, input [31:0] sym);
        localparam SZ = 35;
        pkt_buf = '0; build_net_hdr(16+SZ); build_xdp_pkt_hdr(16+SZ, 1, seq);
        build_msg_common(SZ, 16'd101, sym, 32'd2, oid);
        pkt_write_u32_le(82, price); pkt_write_u32_le(86, qty);
        pkt_write_byte(90,0); pkt_write_byte(91,0); pkt_write_byte(92,0);
        pkt_len = 42+16+SZ;
    endtask

    task build_del_order(input [31:0] seq, input [63:0] oid, input [31:0] sym);
        localparam SZ = 25;
        pkt_buf = '0; build_net_hdr(16+SZ); build_xdp_pkt_hdr(16+SZ, 1, seq);
        build_msg_common(SZ, 16'd102, sym, 32'd3, oid);
        pkt_write_byte(82, 0);
        pkt_len = 42+16+SZ;
    endtask

    task build_exec_order(input [31:0] seq, input [63:0] oid, input [31:0] tid,
                          input [31:0] price, input [31:0] qty, input [31:0] sym);
        localparam SZ = 42;
        pkt_buf = '0; build_net_hdr(16+SZ); build_xdp_pkt_hdr(16+SZ, 1, seq);
        build_msg_common(SZ, 16'd103, sym, 32'd4, oid);
        pkt_write_u32_le(82, tid); pkt_write_u32_le(86, price);
        pkt_write_u32_le(90, qty); pkt_write_byte(94, 8'd1); pkt_write_byte(95, 0);
        pkt_write_u32_le(96, 0);
        pkt_len = 42+16+SZ;
    endtask

    task build_replace_order(input [31:0] seq, input [63:0] oid, input [63:0] new_oid,
                             input [31:0] price, input [31:0] qty, input [31:0] sym);
        localparam SZ = 42;
        pkt_buf = '0; build_net_hdr(16+SZ); build_xdp_pkt_hdr(16+SZ, 1, seq);
        build_msg_common(SZ, 16'd104, sym, 32'd5, oid);
        pkt_write_u64_le(82, new_oid); pkt_write_u32_le(90, price);
        pkt_write_u32_le(94, qty); pkt_write_byte(98, 0); pkt_write_byte(99, 0);
        pkt_len = 42+16+SZ;
    endtask

    // ----------------------------------------------------------------
    // AXI-Stream Master BFM
    // ----------------------------------------------------------------
    task axis_send(input integer idle_cycles);
        integer bt, bk, bi, bthis, beats_total, remainder;
        logic [63:0] bd; logic [7:0] bkp;
        beats_total = (pkt_len + KEEP_W - 1) / KEEP_W;
        remainder   = pkt_len % KEEP_W;
        for (bt = 0; bt < beats_total; bt++) begin
            if (idle_cycles > 0 && bt > 0) begin
                s_axis_tvalid = 0; repeat(idle_cycles) @(posedge clk); #1;
            end
            bd = '0; bkp = '0; bi = bt * KEEP_W;
            bthis = (bt == beats_total-1 && remainder != 0) ? remainder : KEEP_W;
            for (bk = 0; bk < bthis; bk++) begin
                bd[bk*8 +: 8] = pkt_buf[(bi+bk)*8 +: 8]; bkp[bk] = 1'b1;
            end
            s_axis_tdata = bd; s_axis_tkeep = bkp;
            s_axis_tvalid = 1; s_axis_tlast = (bt == beats_total-1);
            @(posedge clk); while (!s_axis_tready) @(posedge clk); #1;
        end
        s_axis_tvalid = 0; s_axis_tlast = 0; s_axis_tkeep = '0; s_axis_tdata = '0;
    endtask

    // ----------------------------------------------------------------
    // Wait for decoded.valid
    // ----------------------------------------------------------------
    task automatic wait_decoded(output logic timed_out);
        integer t; timed_out = 0; t = 0;
        while (!decoded.valid && t < 400) begin @(posedge clk); #1; t++; end
        if (t >= 400) timed_out = 1;
        else @(posedge clk); #1;
    endtask

    // ----------------------------------------------------------------
    // Scoreboard — returns number of field errors
    // ----------------------------------------------------------------
    integer fail_count, msg_count, field_errs;

    task check_u16(input string nm, input [15:0] got, exp);
        if (got !== exp) begin $display("  │   ** MISMATCH %s: got %0d, expected %0d", nm, got, exp); field_errs++; fail_count++; end
    endtask
    task check_u32(input string nm, input [31:0] got, exp);
        if (got !== exp) begin $display("  │   ** MISMATCH %s: got %0d (0x%08h), expected %0d", nm, got, got, exp); field_errs++; fail_count++; end
    endtask
    task check_u64(input string nm, input [63:0] got, exp);
        if (got !== exp) begin $display("  │   ** MISMATCH %s: got 0x%016h, expected 0x%016h", nm, got, exp); field_errs++; fail_count++; end
    endtask
    task check_u8(input string nm, input [7:0] got, exp);
        if (got !== exp) begin $display("  │   ** MISMATCH %s: got 0x%02h, expected 0x%02h", nm, got, exp); field_errs++; fail_count++; end
    endtask

    // ----------------------------------------------------------------
    // VCD
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("xdp_chain_dump.vcd");
        $dumpvars(0, xdp_chain_tb);
    end

    // ----------------------------------------------------------------
    // Main test sequence
    // ----------------------------------------------------------------
    initial begin
        logic timed_out;

        fail_count = 0; msg_count = 0;
        s_axis_tdata = '0; s_axis_tkeep = '0;
        s_axis_tvalid = 0; s_axis_tlast = 0;
        pkt_buf = '0; pkt_len = 0;

        $display("");
        $display("  ╔═════════════════════════════════════════════════════════════╗");
        $display("  ║  NYSE XDP/Pillar Pipeline Integration Test                 ║");
        $display("  ║  filter -> framer -> demux                                 ║");
        $display("  ╚═════════════════════════════════════════════════════════════╝");

        rst_n = 0; repeat(4) @(posedge clk); #1;
        rst_n = 1; repeat(2) @(posedge clk); #1;

        // ============================================================
        // TEST 1: Add Order (100)
        // ============================================================
        field_errs = 0;
        build_add_order(32'd1001, 64'hDEAD_0000_0000_0001,
                        32'd98750, 32'd100, 32'd12345, "B");
        print_input("TEST 1: Add Order", 16'd100, 32'd12345,
                    64'hDEAD_0000_0000_0001, 32'd98750, 32'd100, "B", '0, '0);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ OUTPUT: TIMEOUT — demux never pulsed valid"); fail_count++; end
        else begin
            msg_count++; print_output();
            check_u16("msg_type",   decoded.msg_type,     16'd100);
            check_u32("symbol_idx", decoded.symbol_index, 32'd12345);
            check_u64("order_id",   decoded.order_id,     64'hDEAD_0000_0000_0001);
            check_u32("price",      decoded.price,        32'd98750);
            check_u32("qty",        decoded.qty,          32'd100);
            check_u8 ("side",       decoded.side,         "B");
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 2: Modify Order (101)
        // ============================================================
        field_errs = 0;
        build_mod_order(32'd1002, 64'hBEEF_0000_0000_0002,
                        32'd100500, 32'd250, 32'd12345);
        print_input("TEST 2: Modify Order", 16'd101, 32'd12345,
                    64'hBEEF_0000_0000_0002, 32'd100500, 32'd250, '0, '0, '0);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ OUTPUT: TIMEOUT"); fail_count++; end
        else begin
            msg_count++; print_output();
            check_u16("msg_type",   decoded.msg_type,     16'd101);
            check_u32("symbol_idx", decoded.symbol_index, 32'd12345);
            check_u64("order_id",   decoded.order_id,     64'hBEEF_0000_0000_0002);
            check_u32("price",      decoded.price,        32'd100500);
            check_u32("qty",        decoded.qty,          32'd250);
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 3: Delete Order (102)
        // ============================================================
        field_errs = 0;
        build_del_order(32'd1003, 64'hCAFE_DEAD_BEEF_0003, 32'd42);
        print_input("TEST 3: Delete Order", 16'd102, 32'd42,
                    64'hCAFE_DEAD_BEEF_0003, '0, '0, '0, '0, '0);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ OUTPUT: TIMEOUT"); fail_count++; end
        else begin
            msg_count++; print_output();
            check_u16("msg_type",   decoded.msg_type,     16'd102);
            check_u32("symbol_idx", decoded.symbol_index, 32'd42);
            check_u64("order_id",   decoded.order_id,     64'hCAFE_DEAD_BEEF_0003);
            check_u32("price",      decoded.price,        32'd0);
            check_u32("qty",        decoded.qty,          32'd0);
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 4: Order Execution (103)
        // ============================================================
        field_errs = 0;
        build_exec_order(32'd1004, 64'h1111_2222_3333_4444,
                         32'd77777, 32'd55000, 32'd50, 32'd9999);
        print_input("TEST 4: Order Execution", 16'd103, 32'd9999,
                    64'h1111_2222_3333_4444, 32'd55000, 32'd50, '0, '0, 32'd77777);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ OUTPUT: TIMEOUT"); fail_count++; end
        else begin
            msg_count++; print_output();
            check_u16("msg_type",   decoded.msg_type,     16'd103);
            check_u32("symbol_idx", decoded.symbol_index, 32'd9999);
            check_u64("order_id",   decoded.order_id,     64'h1111_2222_3333_4444);
            check_u32("trade_id",   decoded.trade_id,     32'd77777);
            check_u32("price",      decoded.price,        32'd55000);
            check_u32("qty",        decoded.qty,          32'd50);
            check_u8 ("printable",  decoded.printable,    8'd1);
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 5: Replace Order (104)
        // ============================================================
        field_errs = 0;
        build_replace_order(32'd1005, 64'hAAAA_BBBB_CCCC_DDDD,
                            64'hEEEE_FFFF_0000_1111,
                            32'd60000, 32'd200, 32'd5555);
        print_input("TEST 5: Replace Order", 16'd104, 32'd5555,
                    64'hAAAA_BBBB_CCCC_DDDD, 32'd60000, 32'd200, '0,
                    64'hEEEE_FFFF_0000_1111, '0);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ OUTPUT: TIMEOUT"); fail_count++; end
        else begin
            msg_count++; print_output();
            check_u16("msg_type",    decoded.msg_type,      16'd104);
            check_u32("symbol_idx",  decoded.symbol_index,  32'd5555);
            check_u64("order_id",    decoded.order_id,      64'hAAAA_BBBB_CCCC_DDDD);
            check_u64("new_order_id",decoded.new_order_id,  64'hEEEE_FFFF_0000_1111);
            check_u32("price",       decoded.price,         32'd60000);
            check_u32("qty",         decoded.qty,           32'd200);
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 6: Wrong UDP port — dropped
        // ============================================================
        build_add_order(32'd1006, 64'hFFFF, 32'd1, 32'd1, 32'd1, "B");
        pkt_write_byte(36, 8'hDE); pkt_write_byte(37, 8'hAD);
        $display("");
        $display("  ┌─────────────────────────────────────────────────────────────┐");
        $display("  │ TEST 6: Wrong UDP port - expect DROP                       │");
        $display("  │ INPUT: Add Order to port 0xDEAD (not %0d)           │", XDP_PORT);
        $display("  ├─────────────────────────────────────────────────────────────┤");
        axis_send(0);
        repeat(40) @(posedge clk); #1;
        if (decoded.valid) fail_count++;
        print_drop_result(!decoded.valid);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 7: Add Order with idle gaps
        // ============================================================
        field_errs = 0;
        build_add_order(32'd1007, 64'h7777_8888_9999_AAAA,
                        32'd123456, 32'd75, 32'd321, "S");
        print_input("TEST 7: Add Order (idle gaps between beats)", 16'd100, 32'd321,
                    64'h7777_8888_9999_AAAA, 32'd123456, 32'd75, "S", '0, '0);
        axis_send(2); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ OUTPUT: TIMEOUT"); fail_count++; end
        else begin
            msg_count++; print_output();
            check_u16("msg_type", decoded.msg_type, 16'd100);
            check_u64("order_id", decoded.order_id, 64'h7777_8888_9999_AAAA);
            check_u32("price",    decoded.price,    32'd123456);
            check_u32("qty",      decoded.qty,      32'd75);
            check_u8 ("side",     decoded.side,     "S");
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 8: Back-to-back Add → Delete
        // ============================================================
        $display("");
        $display("  ┌─────────────────────────────────────────────────────────────┐");
        $display("  │ TEST 8: Back-to-back Add -> Delete (pipeline throughput)    │");
        $display("  ├─────────────────────────────────────────────────────────────┤");
        field_errs = 0;

        build_add_order(32'd2001, 64'hA0A0_B0B0_C0C0_D0D0, 32'd50000, 32'd10, 32'd100, "B");
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ pkt A (Add): TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ pkt A decoded: type=%0d oid=0x%016h price=%0d qty=%0d",
                     decoded.msg_type, decoded.order_id, decoded.price, decoded.qty);
            check_u16("A_type", decoded.msg_type, 16'd100);
            check_u64("A_oid",  decoded.order_id, 64'hA0A0_B0B0_C0C0_D0D0);
        end

        build_del_order(32'd2002, 64'hA0A0_B0B0_C0C0_D0D0, 32'd100);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ pkt B (Del): TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ pkt B decoded: type=%0d oid=0x%016h",
                     decoded.msg_type, decoded.order_id);
            check_u16("B_type", decoded.msg_type, 16'd102);
            check_u64("B_oid",  decoded.order_id, 64'hA0A0_B0B0_C0C0_D0D0);
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 9: Full lifecycle — Add → Modify → Exec → Delete
        // ============================================================
        $display("");
        $display("  ┌─────────────────────────────────────────────────────────────┐");
        $display("  │ TEST 9: Full order lifecycle                               │");
        $display("  │   Add -> Modify -> Partial Exec -> Delete                  │");
        $display("  │   OrderID: 0xFACE_0000_0000_0001   Symbol: 777             │");
        $display("  ├─────────────────────────────────────────────────────────────┤");
        field_errs = 0;

        // Add: 500 shares @ 25000, Buy
        build_add_order(32'd3001, 64'hFACE_0000_0000_0001, 32'd25000, 32'd500, 32'd777, "B");
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ 1. Add:  TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ 1. Add:     price=%0d  qty=%0d  side=%s",
                     decoded.price, decoded.qty, side_str(decoded.side));
            check_u16("add_type", decoded.msg_type, 16'd100);
            check_u32("add_price", decoded.price, 32'd25000);
            check_u32("add_qty", decoded.qty, 32'd500);
        end

        // Modify: reprice to 25100, reduce to 400
        build_mod_order(32'd3002, 64'hFACE_0000_0000_0001, 32'd25100, 32'd400, 32'd777);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ 2. Mod:  TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ 2. Modify: price=%0d  qty=%0d  (was 25000/500)",
                     decoded.price, decoded.qty);
            check_u16("mod_type", decoded.msg_type, 16'd101);
            check_u32("mod_price", decoded.price, 32'd25100);
            check_u32("mod_qty", decoded.qty, 32'd400);
        end

        // Exec: fill 100 of 400 @ 25100
        build_exec_order(32'd3003, 64'hFACE_0000_0000_0001,
                         32'd88888, 32'd25100, 32'd100, 32'd777);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ 3. Exec: TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ 3. Exec:   trade=%0d  price=%0d  qty=%0d  (300 remaining)",
                     decoded.trade_id, decoded.price, decoded.qty);
            check_u16("exec_type", decoded.msg_type, 16'd103);
            check_u32("exec_tid", decoded.trade_id, 32'd88888);
            check_u32("exec_price", decoded.price, 32'd25100);
            check_u32("exec_qty", decoded.qty, 32'd100);
        end

        // Delete remaining
        build_del_order(32'd3004, 64'hFACE_0000_0000_0001, 32'd777);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ 4. Del:  TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ 4. Delete: oid=0x%016h  (order removed)", decoded.order_id);
            check_u16("del_type", decoded.msg_type, 16'd102);
        end
        print_result(field_errs);
        repeat(4) @(posedge clk); #1;

        // ============================================================
        // TEST 10: Add → Replace → Delete
        // ============================================================
        $display("");
        $display("  ┌─────────────────────────────────────────────────────────────┐");
        $display("  │ TEST 10: Cancel-Replace flow                               │");
        $display("  │   Add -> Replace (new OID + price) -> Delete new           │");
        $display("  ├─────────────────────────────────────────────────────────────┤");
        field_errs = 0;

        build_add_order(32'd4001, 64'hAAAA_0000_0000_0001,
                        32'd30000, 32'd1000, 32'd555, "S");
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ 1. Add:     TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ 1. Add:     oid=0x%016h  price=%0d  qty=%0d  side=%s",
                     decoded.order_id, decoded.price, decoded.qty, side_str(decoded.side));
            check_u16("type", decoded.msg_type, 16'd100);
        end

        build_replace_order(32'd4002, 64'hAAAA_0000_0000_0001,
                            64'hBBBB_0000_0000_0002,
                            32'd30500, 32'd800, 32'd555);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ 2. Replace: TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ 2. Replace: old=0x%016h -> new=0x%016h",
                     decoded.order_id, decoded.new_order_id);
            $display("  │             price=%0d  qty=%0d  (was 30000/1000)",
                     decoded.price, decoded.qty);
            check_u16("type",    decoded.msg_type,     16'd104);
            check_u64("old_oid", decoded.order_id,     64'hAAAA_0000_0000_0001);
            check_u64("new_oid", decoded.new_order_id, 64'hBBBB_0000_0000_0002);
            check_u32("price",   decoded.price,        32'd30500);
            check_u32("qty",     decoded.qty,          32'd800);
        end

        build_del_order(32'd4003, 64'hBBBB_0000_0000_0002, 32'd555);
        axis_send(0); wait_decoded(timed_out);
        if (timed_out) begin $display("  │ 3. Delete:  TIMEOUT"); fail_count++; end
        else begin
            msg_count++;
            $display("  │ 3. Delete:  oid=0x%016h  (replacement removed)",
                     decoded.order_id);
            check_u16("type", decoded.msg_type, 16'd102);
            check_u64("oid",  decoded.order_id, 64'hBBBB_0000_0000_0002);
        end
        print_result(field_errs);

        // ============================================================
        // Summary
        // ============================================================
        repeat(4) @(posedge clk);
        $display("");
        $display("  ╔═════════════════════════════════════════════════════════════╗");
        $display("  ║  SUMMARY                                                   ║");
        $display("  ║  Messages decoded : %-3d                                     ║", msg_count);
        $display("  ║  Field errors     : %-3d                                     ║", fail_count);
        if (fail_count == 0)
        $display("  ║  @@@ Passed                                                ║");
        else
        $display("  ║  @@@ Failed                                                ║");
        $display("  ╚═════════════════════════════════════════════════════════════╝");
        $display("");
        $finish;
    end

endmodule