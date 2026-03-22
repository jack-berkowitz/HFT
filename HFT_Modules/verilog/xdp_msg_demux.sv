`include "sys_defs.svh"
// ============================================================
// xdp_msg_demux.sv
//
// Accepts the realigned AXI-Stream from xdp_packet_framer
// (byte 0 = MsgSize LSB) and extracts order fields for
// message types 100–104 (Add, Modify, Delete, Exec, Replace).
//
// Output: single pillar_msg_t struct.  out_msg.valid pulses
// for exactly one cycle per decoded message.
//
// Design: zero-buffer, beat-by-beat extraction.
// Fields are latched directly from each incoming beat as it
// arrives, using the msg_type (known from beat 0) to decide
// which byte lanes map to which output fields.
//
// Beat layout (64-bit LE bus, after framing):
//   Beat 0 [bytes  0- 7]: MsgSize(2) MsgType(2) SourceTimeNS(4)
//   Beat 1 [bytes  8-15]: SymbolIndex(4) SymbolSeqNum(4)
//   Beat 2 [bytes 16-23]: OrderID(8)
//   Beat 3 [bytes 24-31]: type-specific
//   Beat 4 [bytes 32-39]: type-specific
//   Beat 5 [bytes 40-41]: type-specific (Exec/Replace tail)
//
// Beat 3 field map:
//   Add/Mod:  Price(4) Volume(4)
//   Delete:   NumParitySplits(1) — message ends here
//   Exec:     TradeID(4) Price(4)
//   Replace:  NewOrderID(8)
//
// Beat 4 field map:
//   Add:      Side(1) FirmID(5) NumParitySplits(1)
//   Modify:   PositionChange(1) PrevPPS(1) NewPPS(1)
//   Exec:     Volume(4) PrintableFlag(1) NumParitySplits(1) DBExecID(4)
//   Replace:  Price(4) Volume(4)
// ============================================================
module xdp_msg_demux (
    input  logic        clk,
    input  logic        rst_n,

    // Slave AXI-Stream (from framer — aligned XDP message bytes)
    input  AXI_TDATA    s_axis_tdata,
    input  AXI_TKEEP    s_axis_tkeep,
    input  logic        s_axis_tvalid,
    input  logic        s_axis_tlast,
    output logic        s_axis_tready,

    // Decoded output — struct with valid pulse
    output pillar_msg_t out_msg
);

    // ----------------------------------------------------------------
    // FSM
    // ----------------------------------------------------------------
    typedef enum logic [1:0] {
        S_IDLE    = 2'd0,
        S_COLLECT = 2'd1,
        S_OUTPUT  = 2'd2
    } state_t;

    state_t state_r, state_next;

    // ----------------------------------------------------------------
    // Beat counter
    // ----------------------------------------------------------------
    logic [2:0] beat_cnt_r;
    logic       beat_transferred;

    assign beat_transferred = s_axis_tvalid & s_axis_tready;

    // Always accept data in IDLE and COLLECT; stall during OUTPUT pulse
    assign s_axis_tready = (state_r != S_OUTPUT);

    // ----------------------------------------------------------------
    // Internal registered fields (build up across beats)
    // ----------------------------------------------------------------
    logic [15:0] msg_type_r;
    logic [31:0] symbol_index_r;
    logic [31:0] symbol_seq_num_r;
    logic [63:0] order_id_r;
    logic [63:0] new_order_id_r;
    logic [31:0] price_r;
    logic [31:0] qty_r;
    logic [31:0] trade_id_r;
    logic [7:0]  side_r;
    logic [7:0]  printable_r;

    // ----------------------------------------------------------------
    // Output struct wiring
    // ----------------------------------------------------------------
    always_comb begin
        out_msg.valid          = (state_r == S_OUTPUT);
        out_msg.msg_type       = msg_type_r;
        out_msg.symbol_index   = symbol_index_r;
        out_msg.symbol_seq_num = symbol_seq_num_r;
        out_msg.order_id       = order_id_r;
        out_msg.new_order_id   = new_order_id_r;
        out_msg.price          = price_r;
        out_msg.qty            = qty_r;
        out_msg.trade_id       = trade_id_r;
        out_msg.side           = side_r;
        out_msg.printable      = printable_r;
    end

    // ----------------------------------------------------------------
    // FSM next-state
    // ----------------------------------------------------------------
    always_comb begin
        state_next = state_r;
        case (state_r)
            S_IDLE: begin
                if (s_axis_tvalid)
                    state_next = S_COLLECT;
            end
            S_COLLECT: begin
                if (beat_transferred && s_axis_tlast)
                    state_next = S_OUTPUT;
            end
            S_OUTPUT: begin
                state_next = S_IDLE;
            end
        endcase
    end

    // ----------------------------------------------------------------
    // Registered logic — FSM state + beat-by-beat field extraction
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            state_r          <= S_IDLE;
            beat_cnt_r       <= '0;
            msg_type_r       <= '0;
            symbol_index_r   <= '0;
            symbol_seq_num_r <= '0;
            order_id_r       <= '0;
            new_order_id_r   <= '0;
            price_r          <= '0;
            qty_r            <= '0;
            trade_id_r       <= '0;
            side_r           <= '0;
            printable_r      <= '0;
        end else begin
            state_r <= state_next;

            case (state_r)
                S_IDLE: begin
                    beat_cnt_r <= '0;
                    if (s_axis_tvalid) begin
                        // Beat 0 arrives — extract msg header
                        beat_cnt_r       <= 3'd1;
                        msg_type_r       <= s_axis_tdata[31:16];   // bytes 2-3

                        // Clear type-specific fields for clean output
                        new_order_id_r   <= '0;
                        price_r          <= '0;
                        qty_r            <= '0;
                        trade_id_r       <= '0;
                        side_r           <= '0;
                        printable_r      <= '0;
                    end
                end

                S_COLLECT: begin
                    if (beat_transferred) begin
                        beat_cnt_r <= beat_cnt_r + 3'd1;

                        case (beat_cnt_r)
                            // ---- Beat 1: SymbolIndex + SymbolSeqNum ----
                            3'd1: begin
                                symbol_index_r   <= s_axis_tdata[31:0];   // bytes 8-11
                                symbol_seq_num_r <= s_axis_tdata[63:32];  // bytes 12-15
                            end

                            // ---- Beat 2: OrderID (full 64 bits) ----
                            3'd2: begin
                                order_id_r <= s_axis_tdata[63:0];         // bytes 16-23
                            end

                            // ---- Beat 3: type-specific (bytes 24-31) ----
                            3'd3: begin
                                case (msg_type_r)
                                    `XDP_MSG_ADD_ORDER,
                                    `XDP_MSG_MOD_ORDER: begin
                                        price_r <= s_axis_tdata[31:0];    // bytes 24-27
                                        qty_r   <= s_axis_tdata[63:32];   // bytes 28-31
                                    end
                                    `XDP_MSG_EXEC_ORDER: begin
                                        trade_id_r <= s_axis_tdata[31:0]; // bytes 24-27
                                        price_r    <= s_axis_tdata[63:32];// bytes 28-31
                                    end
                                    `XDP_MSG_REPLACE: begin
                                        new_order_id_r <= s_axis_tdata[63:0]; // bytes 24-31
                                    end
                                    default: ; // Delete: byte 24 = NumParitySplits, ignored
                                endcase
                            end

                            // ---- Beat 4: type-specific (bytes 32-39) ----
                            3'd4: begin
                                case (msg_type_r)
                                    `XDP_MSG_ADD_ORDER: begin
                                        side_r <= s_axis_tdata[7:0];      // byte 32
                                    end
                                    `XDP_MSG_EXEC_ORDER: begin
                                        qty_r       <= s_axis_tdata[31:0];   // bytes 32-35
                                        printable_r <= s_axis_tdata[39:32];  // byte 36
                                    end
                                    `XDP_MSG_REPLACE: begin
                                        price_r <= s_axis_tdata[31:0];    // bytes 32-35
                                        qty_r   <= s_axis_tdata[63:32];   // bytes 36-39
                                    end
                                    default: ; // Mod: PositionChange — not needed for book
                                endcase
                            end

                            default: ; // Beats 5+ — tail bytes, nothing we need
                        endcase
                    end
                end

                S_OUTPUT: begin
                    // Valid pulse lasts one cycle, then return to idle
                    beat_cnt_r <= '0;
                end
            endcase
        end
    end

endmodule