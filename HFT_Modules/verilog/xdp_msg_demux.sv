`include "sys_defs.svh"

module xdp_msg_demux (
    input  logic        clk,
    input  logic        rst_n,

    input  AXI_TDATA    s_axis_tdata,
    input  AXI_TKEEP    s_axis_tkeep,
    input  logic        s_axis_tvalid,
    input  logic        s_axis_tlast,
    output logic        s_axis_tready,

    output pillar_msg_t out_msg
);

    typedef enum logic [1:0] {
        S_IDLE    = 2'd0,
        S_COLLECT = 2'd1,
        S_OUTPUT  = 2'd2
    } state_t;

    state_t state_r, state_next;

    logic [2:0] beat_cnt_r;
    logic       beat_transferred;

    assign beat_transferred = s_axis_tvalid & s_axis_tready;

    assign s_axis_tready = (state_r != S_OUTPUT);

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
                        beat_cnt_r <= 3'd1;
                        case (s_axis_tdata[31:16])
                            `XDP_MSG_ADD_ORDER:  msg_type_r <= `MSG_ADD;
                            `XDP_MSG_MOD_ORDER:  msg_type_r <= `MSG_MOD;
                            `XDP_MSG_DEL_ORDER:  msg_type_r <= `MSG_DEL;
                            `XDP_MSG_EXEC_ORDER: msg_type_r <= `MSG_EXEC;
                            `XDP_MSG_REPLACE:    msg_type_r <= `MSG_REPL;
                            default:             msg_type_r <= '0;
                        endcase

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
                            3'd1: begin
                                symbol_index_r   <= s_axis_tdata[31:0];
                                symbol_seq_num_r <= s_axis_tdata[63:32];
                            end

                            3'd2: begin
                                order_id_r <= s_axis_tdata[63:0];
                            end

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
                                    default: ;
                                endcase
                            end

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
                                    default: ;
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
