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
// Encoding at beat 0:
//   Wire 16'd100 → msg_type 3'd0 (MSG_ADD)
//   Wire 16'd101 → msg_type 3'd1 (MSG_MOD)
//   Wire 16'd102 → msg_type 3'd2 (MSG_DEL)
//   Wire 16'd103 → msg_type 3'd3 (MSG_EXEC)
//   Wire 16'd104 → msg_type 3'd4 (MSG_REPL)
//
// Side encoding at beat 4 (Add only):
//   Wire ASCII 'S' (0x53) → side = 1 (SIDE_SELL)
//   Anything else          → side = 0 (SIDE_BUY)
//
// Beat layout (64-bit LE bus, after framing):
//   Beat 0 [bytes  0- 7]: MsgSize(2) MsgType(2) SourceTimeNS(4)
//   Beat 1 [bytes  8-15]: SymbolIndex(4) SymbolSeqNum(4)
//   Beat 2 [bytes 16-23]: OrderID(8)
//   Beat 3 [bytes 24-31]: type-specific
//   Beat 4 [bytes 32-39]: type-specific
//   Beat 5 [bytes 40-41]: type-specific (Exec/Replace tail)
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
    logic [2:0]  msg_type_r;
    logic [31:0] symbol_index_r;
    logic [31:0] symbol_seq_num_r;
    logic [63:0] order_id_r;
    logic [63:0] new_order_id_r;
    logic [31:0] price_r;
    logic [31:0] qty_r;
    logic [31:0] trade_id_r;
    logic        side_r;
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
                        // Beat 0 arrives — encode 16-bit wire msg_type to 3-bit
                        beat_cnt_r <= 3'd1;
                        case (s_axis_tdata[31:16])
                            `XDP_MSG_ADD_ORDER:  msg_type_r <= `MSG_ADD;
                            `XDP_MSG_MOD_ORDER:  msg_type_r <= `MSG_MOD;
                            `XDP_MSG_DEL_ORDER:  msg_type_r <= `MSG_DEL;
                            `XDP_MSG_EXEC_ORDER: msg_type_r <= `MSG_EXEC;
                            `XDP_MSG_REPLACE:    msg_type_r <= `MSG_REPL;
                            default:             msg_type_r <= '0;
                        endcase

                        // Clear type-specific fields for clean output
                        new_order_id_r   <= '0;
                        price_r          <= '0;
                        qty_r            <= '0;
                        trade_id_r       <= '0;
                        side_r           <= `SIDE_BUY;
                        printable_r      <= '0;
                    end
                end

                S_COLLECT: begin
                    if (beat_transferred) begin
                        beat_cnt_r <= beat_cnt_r + 3'd1;

                        case (beat_cnt_r)
                            // ---- Beat 1: SymbolIndex + SymbolSeqNum ----
                            3'd1: begin
                                symbol_index_r   <= s_axis_tdata[31:0];
                                symbol_seq_num_r <= s_axis_tdata[63:32];
                            end

                            // ---- Beat 2: OrderID (full 64 bits) ----
                            3'd2: begin
                                order_id_r <= s_axis_tdata[63:0];
                            end

                            // ---- Beat 3: type-specific (bytes 24-31) ----
                            3'd3: begin
                                case (msg_type_r)
                                    `MSG_ADD,
                                    `MSG_MOD: begin
                                        price_r <= s_axis_tdata[31:0];
                                        qty_r   <= s_axis_tdata[63:32];
                                    end
                                    `MSG_EXEC: begin
                                        trade_id_r <= s_axis_tdata[31:0];
                                        price_r    <= s_axis_tdata[63:32];
                                    end
                                    `MSG_REPL: begin
                                        new_order_id_r <= s_axis_tdata[63:0];
                                    end
                                    default: ; // Delete: NumParitySplits, ignored
                                endcase
                            end

                            // ---- Beat 4: type-specific (bytes 32-39) ----
                            3'd4: begin
                                case (msg_type_r)
                                    `MSG_ADD: begin
                                        side_r <= (s_axis_tdata[7:0] == "S") ?
                                                  `SIDE_SELL : `SIDE_BUY;
                                    end
                                    `MSG_EXEC: begin
                                        qty_r       <= s_axis_tdata[31:0];
                                        printable_r <= s_axis_tdata[39:32];
                                    end
                                    `MSG_REPL: begin
                                        price_r <= s_axis_tdata[31:0];
                                        qty_r   <= s_axis_tdata[63:32];
                                    end
                                    default: ; // Mod: PositionChange — not needed
                                endcase
                            end

                            default: ;
                        endcase
                    end
                end

                S_OUTPUT: begin
                    beat_cnt_r <= '0;
                end
            endcase
        end
    end

endmodule