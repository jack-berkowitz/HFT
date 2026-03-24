`include "sys_defs.svh"
// ============================================================
// order_gen_tx.sv
//
// Order generation and AXI-Stream TX path.
//
// When the index arbitrage engine fires a trade signal, this
// module applies risk checks, constructs a UDP order-entry
// packet, and serializes it as AXI-Stream beats for the
// Corundum TX MAC.
//
// Packet format (74 bytes total):
//   Ethernet  14 B
//   IPv4      20 B  (header checksum computed at elaboration)
//   UDP        8 B  (checksum = 0; offload to NIC)
//   Order     32 B  (msg_type, side, symbol, price, qty,
//                     client_oid, spread snapshot)
//
// 74 bytes / 8 B per beat = 10 beats (last beat: 2 valid bytes)
//
// Price logic:
//   BUY  → price = actual_price + PRICE_OFFSET  (lift the ask)
//   SELL → price = actual_price - PRICE_OFFSET  (hit the bid)
//
// Risk controls:
//   - Cooldown timer between consecutive orders
//   - Signed net-position limit
//   - Global enable gate
//
// Order entry payload (32 bytes, little-endian fields):
//   [0:1]   msg_type     = 0x0001 (NEW_ORDER)
//   [2]     side         = 'B' (0x42) or 'S' (0x53)
//   [3]     reserved
//   [4:7]   symbol_id
//   [8:11]  price
//   [12:15] qty
//   [16:23] client_oid   (incrementing 64-bit sequence)
//   [24:31] spread_snap  (raw Q44.20 spread for audit trail)
// ============================================================
module order_gen_tx #(
    parameter [47:0] SRC_MAC       = 48'hDE_AD_BE_EF_CA_FE,
    parameter [47:0] DST_MAC       = 48'h01_00_5E_00_00_01,
    parameter [31:0] SRC_IP        = {8'd10, 8'd0, 8'd0, 8'd1},
    parameter [31:0] DST_IP        = {8'd10, 8'd0, 8'd0, 8'd2},
    parameter [15:0] SRC_PORT      = 16'd12345,
    parameter [15:0] DST_PORT      = 16'd54321,
    parameter [31:0] ORDER_QTY     = 32'd100,
    parameter [31:0] COOLDOWN_CYC  = 32'd100,
    parameter [31:0] MAX_POS       = 32'd1000,
    parameter [31:0] PRICE_OFFSET  = 32'd10,
    parameter [31:0] IDX_SYM_ID    = `IDX_SYMBOL
)(
    input  logic              clk,
    input  logic              rst_n,
    input  logic              enable,

    input  trade_signal_t     in_trade,

    // Master AXI-Stream (to NIC TX)
    output AXI_TDATA          m_axis_tdata,
    output AXI_TKEEP          m_axis_tkeep,
    output logic              m_axis_tvalid,
    output logic              m_axis_tlast,
    input  logic              m_axis_tready,

    // Status / monitoring
    output logic [31:0]       order_count,
    output logic signed [31:0] net_position,
    output logic              pkt_sent
);

    // ================================================================
    // Constants
    // ================================================================
    localparam N_BEATS   = 10;
    localparam LAST_BEAT = N_BEATS - 1;
    localparam BEAT_W    = 4;

    localparam [15:0] ORDER_MSG_NEW = 16'h0001;
    localparam [15:0] IP_TOTAL_LEN  = 16'd60;    // 20 + 8 + 32
    localparam [15:0] UDP_LEN       = 16'd40;     //  8 + 32

    // ================================================================
    // IPv4 header checksum — constant since all header fields are
    // parameters.  Computed at elaboration time.
    // ================================================================
    function automatic [15:0] compute_ip_csum(input [31:0] sip, dip);
        logic [31:0] s;
        s = 32'h4500 + {16'd0, IP_TOTAL_LEN} + 32'h0000 +
            32'h4000 + 32'h4011;
        s = s + {16'd0, sip[31:16]} + {16'd0, sip[15:0]};
        s = s + {16'd0, dip[31:16]} + {16'd0, dip[15:0]};
        s = {16'd0, s[15:0]} + {16'd0, s[31:16]};
        s = {16'd0, s[15:0]} + {16'd0, s[31:16]};
        return ~s[15:0];
    endfunction

    localparam [15:0] IP_CSUM = compute_ip_csum(SRC_IP, DST_IP);

    // ================================================================
    // FSM
    // ================================================================
    typedef enum logic [0:0] {
        S_IDLE = 1'b0,
        S_SEND = 1'b1
    } state_t;

    state_t            state_r;
    logic [BEAT_W-1:0] beat_r;

    // ================================================================
    // Latched order fields (captured in S_IDLE, stable during S_SEND)
    // ================================================================
    logic        lat_dir;           // 0=BUY, 1=SELL
    logic [31:0] lat_price;
    logic [63:0] lat_spread;
    logic [63:0] lat_oid;

    // ================================================================
    // Cooldown counter
    // ================================================================
    logic [31:0] cd_cnt;
    wire         cd_ok = (cd_cnt == 32'd0);

    // ================================================================
    // Position-limit check
    // ================================================================
    logic signed [31:0] next_pos;
    logic               pos_ok;

    always_comb begin
        next_pos = in_trade.direction
                   ? (net_position - $signed({1'b0, ORDER_QTY}))
                   : (net_position + $signed({1'b0, ORDER_QTY}));
        pos_ok = (next_pos >= -$signed(MAX_POS)) &&
                 (next_pos <=  $signed(MAX_POS));
    end

    // ================================================================
    // Order-ID sequence generator
    // ================================================================
    logic [63:0] oid_seq;

    // ================================================================
    // Byte-packing helper
    //   b0 → tdata[7:0]   (lowest wire address)
    //   b7 → tdata[63:56]  (highest wire address)
    // ================================================================
    function automatic [63:0] pk(
        input [7:0] b0, b1, b2, b3, b4, b5, b6, b7);
        return {b7, b6, b5, b4, b3, b2, b1, b0};
    endfunction

    // ================================================================
    // Per-beat packet construction (combinational)
    //
    // Byte layout:
    //   Beat 0 [ 0: 7] dst_mac[5:0], src_mac[1:0]
    //   Beat 1 [ 8:15] src_mac[5:2], ethertype, ipv4 ver/tos
    //   Beat 2 [16:23] ipv4 len/id/flags/ttl/proto
    //   Beat 3 [24:31] ipv4 csum, src_ip, dst_ip[1:0]
    //   Beat 4 [32:39] dst_ip[3:2], udp sport/dport/len
    //   Beat 5 [40:47] udp csum, order msg_type/side/rsvd/sym[1:0]
    //   Beat 6 [48:55] sym[3:2], price, qty[1:0]
    //   Beat 7 [56:63] qty[3:2], client_oid[5:0]
    //   Beat 8 [64:71] client_oid[7:6], spread[5:0]
    //   Beat 9 [72:73] spread[7:6]  (2 valid bytes, tlast)
    // ================================================================
    logic [63:0] beat_data;
    logic [7:0]  beat_keep;
    logic        beat_last;

    always_comb begin
        beat_data = '0;
        beat_keep = 8'hFF;
        beat_last = 1'b0;

        case (beat_r)
            4'd0: beat_data = pk(
                DST_MAC[47:40], DST_MAC[39:32], DST_MAC[31:24],
                DST_MAC[23:16], DST_MAC[15:8],  DST_MAC[7:0],
                SRC_MAC[47:40], SRC_MAC[39:32]);

            4'd1: beat_data = pk(
                SRC_MAC[31:24], SRC_MAC[23:16], SRC_MAC[15:8],
                SRC_MAC[7:0],   8'h08, 8'h00,   8'h45, 8'h00);

            4'd2: beat_data = pk(
                IP_TOTAL_LEN[15:8], IP_TOTAL_LEN[7:0],
                8'h00, 8'h00,   8'h40, 8'h00,   8'h40, 8'h11);

            4'd3: beat_data = pk(
                IP_CSUM[15:8],  IP_CSUM[7:0],
                SRC_IP[31:24],  SRC_IP[23:16],
                SRC_IP[15:8],   SRC_IP[7:0],
                DST_IP[31:24],  DST_IP[23:16]);

            4'd4: beat_data = pk(
                DST_IP[15:8],   DST_IP[7:0],
                SRC_PORT[15:8], SRC_PORT[7:0],
                DST_PORT[15:8], DST_PORT[7:0],
                UDP_LEN[15:8],  UDP_LEN[7:0]);

            4'd5: beat_data = pk(
                8'h00, 8'h00,
                ORDER_MSG_NEW[7:0], ORDER_MSG_NEW[15:8],
                lat_dir ? 8'h53 : 8'h42,
                8'h00,
                IDX_SYM_ID[7:0],  IDX_SYM_ID[15:8]);

            4'd6: beat_data = pk(
                IDX_SYM_ID[23:16], IDX_SYM_ID[31:24],
                lat_price[7:0],    lat_price[15:8],
                lat_price[23:16],  lat_price[31:24],
                ORDER_QTY[7:0],    ORDER_QTY[15:8]);

            4'd7: beat_data = pk(
                ORDER_QTY[23:16],  ORDER_QTY[31:24],
                lat_oid[7:0],      lat_oid[15:8],
                lat_oid[23:16],    lat_oid[31:24],
                lat_oid[39:32],    lat_oid[47:40]);

            4'd8: beat_data = pk(
                lat_oid[55:48],    lat_oid[63:56],
                lat_spread[7:0],   lat_spread[15:8],
                lat_spread[23:16], lat_spread[31:24],
                lat_spread[39:32], lat_spread[47:40]);

            4'd9: begin
                beat_data = pk(
                    lat_spread[55:48], lat_spread[63:56],
                    8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00);
                beat_keep = 8'b0000_0011;
                beat_last = 1'b1;
            end

            default: ;
        endcase
    end

    // ================================================================
    // AXI-Stream output
    // ================================================================
    assign m_axis_tdata  = beat_data;
    assign m_axis_tkeep  = (state_r == S_SEND) ? beat_keep : '0;
    assign m_axis_tvalid = (state_r == S_SEND);
    assign m_axis_tlast  = (state_r == S_SEND) && beat_last;

    // ================================================================
    // FSM + sequential logic
    // ================================================================
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            state_r      <= S_IDLE;
            beat_r       <= '0;
            lat_dir      <= 1'b0;
            lat_price    <= '0;
            lat_spread   <= '0;
            lat_oid      <= '0;
            cd_cnt       <= '0;
            oid_seq      <= 64'd1;
            order_count  <= '0;
            net_position <= '0;
            pkt_sent     <= 1'b0;
        end else begin
            pkt_sent <= 1'b0;

            if (cd_cnt != 32'd0)
                cd_cnt <= cd_cnt - 32'd1;

            case (state_r)
                // =====================================================
                S_IDLE: begin
                    if (in_trade.valid && enable && cd_ok && pos_ok) begin
                        lat_dir    <= in_trade.direction;
                        lat_spread <= in_trade.spread;
                        lat_oid    <= oid_seq;

                        if (!in_trade.direction)
                            lat_price <= in_trade.actual_price + PRICE_OFFSET;
                        else if (in_trade.actual_price > PRICE_OFFSET)
                            lat_price <= in_trade.actual_price - PRICE_OFFSET;
                        else
                            lat_price <= 32'd1;

                        oid_seq <= oid_seq + 64'd1;
                        beat_r  <= '0;
                        state_r <= S_SEND;
                    end
                end

                // =====================================================
                S_SEND: begin
                    if (m_axis_tready) begin
                        if (beat_r == BEAT_W'(LAST_BEAT)) begin
                            cd_cnt      <= COOLDOWN_CYC;
                            order_count <= order_count + 32'd1;
                            pkt_sent    <= 1'b1;

                            if (!lat_dir)
                                net_position <= net_position +
                                    $signed({1'b0, ORDER_QTY});
                            else
                                net_position <= net_position -
                                    $signed({1'b0, ORDER_QTY});

                            state_r <= S_IDLE;
                        end else begin
                            beat_r <= beat_r + 1'b1;
                        end
                    end
                end

                default: state_r <= S_IDLE;
            endcase
        end
    end

endmodule
