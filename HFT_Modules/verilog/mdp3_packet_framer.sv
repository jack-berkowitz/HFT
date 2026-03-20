// ============================================================
// mdp3_packet_framer.sv
//
// Strips the 46-byte network header (Eth 14 + IPv4 20 + UDP 8
// + MDP3 seq 4) from an AXI-Stream packet and realigns the
// remaining SBE payload so byte 0 starts at tdata[7:0].
//
// On a 64-bit (8-byte) bus, 46 bytes = 5 full beats + 6 leftover
// header bytes in beat 5.  The upper 2 bytes of beat 5 are the
// first payload bytes.
//
//   SKIP_BEATS = 5     beats consumed silently (beats 0-4)
//   OFFSET     = 6     header bytes in the boundary beat (beat 5)
//   REMAIN     = 2     payload bytes in the boundary beat
//
// Realignment barrel shift (every output beat):
//   output = { cur_tdata[47:0],  prev_tdata[63:48] }
//              6 bytes (upper)    2 bytes (lower)
//
// Latency: 1 cycle after the boundary beat.
// ============================================================
`include "sys_defs.svh"

module mdp3_packet_framer #(
    parameter HEADER_BYTES = 46
)(
    input  logic        clk,
    input  logic        rst_n,

    // Slave AXI-Stream input (from ipv4_udp_port_filter)
    input  AXI_TDATA    s_axis_tdata,
    input  AXI_TKEEP    s_axis_tkeep,
    input  logic        s_axis_tvalid,
    input  logic        s_axis_tlast,
    output logic        s_axis_tready,

    // Master AXI-Stream output (clean SBE payload, lane-aligned)
    output AXI_TDATA    m_axis_tdata,
    output AXI_TKEEP    m_axis_tkeep,
    output logic        m_axis_tvalid,
    output logic        m_axis_tlast,
    input  logic        m_axis_tready
);

    // ----------------------------------------------------------------
    // Compile-time constants
    // ----------------------------------------------------------------
    localparam BYTE_W      = `DATA_W / 8;               // 8
    localparam SKIP_BEATS  = HEADER_BYTES / BYTE_W;     // 5
    localparam OFFSET      = HEADER_BYTES % BYTE_W;     // 6
    localparam REMAIN      = BYTE_W - OFFSET;            // 2
    localparam CNT_W       = $clog2(SKIP_BEATS + 1);    // 3

    // ----------------------------------------------------------------
    // FSM states
    // ----------------------------------------------------------------
    typedef enum logic [1:0] {
        S_SKIP,         // consuming and discarding header beats
        S_PAYLOAD,      // realigning and forwarding payload beats
        S_EXTRA         // flushing residual bytes after input tlast
    } state_t;

    state_t             state_r, state_nxt;
    logic [CNT_W-1:0]  beat_cnt_r, beat_cnt_nxt;

    // Previous-beat residual register
    AXI_TDATA           prev_data_r;
    AXI_TKEEP           prev_keep_r;

    // ----------------------------------------------------------------
    // Handshake helpers
    // ----------------------------------------------------------------
    logic xfer_in;
    logic xfer_out;

    assign xfer_in  = s_axis_tvalid & s_axis_tready;
    assign xfer_out = m_axis_tvalid & m_axis_tready;

    // ----------------------------------------------------------------
    // Popcount of tkeep — valid bytes in current input beat
    // ----------------------------------------------------------------
    logic [3:0] in_valid_bytes;

    always_comb begin
        in_valid_bytes = '0;
        for (int i = 0; i < BYTE_W; i++)
            in_valid_bytes = in_valid_bytes + {3'b0, s_axis_tkeep[i]};
    end

    // Residual: valid bytes above the OFFSET boundary in the
    // current beat (need a flush beat after tlast if > 0)
    logic [3:0] residual_cnt;
    assign residual_cnt = (in_valid_bytes > 4'(OFFSET))
                        ? (in_valid_bytes - 4'(OFFSET))
                        : 4'd0;

    // ----------------------------------------------------------------
    // Barrel-shift combiner
    //
    //   combined = { cur[47:0],  prev[63:48] }
    //               6 bytes      2 bytes
    //
    // prev's top 2 bytes (residual from the prior beat) land at
    // byte lanes [1:0]; current beat's bottom 6 bytes fill [7:2].
    // ----------------------------------------------------------------
    AXI_TDATA combined_data;
    AXI_TKEEP combined_keep;

    assign combined_data = { s_axis_tdata[OFFSET*8-1 : 0],
                             prev_data_r [`DATA_W-1  : OFFSET*8] };
    assign combined_keep = { s_axis_tkeep[OFFSET-1   : 0],
                             prev_keep_r [BYTE_W-1   : OFFSET]   };

    // ----------------------------------------------------------------
    // Extra-beat data — shift residual down to byte lane 0
    // ----------------------------------------------------------------
    AXI_TDATA extra_data;
    AXI_TKEEP extra_keep;

    assign extra_data = prev_data_r >> (OFFSET * 8);
    assign extra_keep = prev_keep_r >> OFFSET;

    // ----------------------------------------------------------------
    // Output mux + backpressure
    // ----------------------------------------------------------------
    always_comb begin
        m_axis_tdata  = '0;
        m_axis_tkeep  = '0;
        m_axis_tvalid = 1'b0;
        m_axis_tlast  = 1'b0;
        s_axis_tready = 1'b0;

        case (state_r)
            S_SKIP: begin
                s_axis_tready = 1'b1;
            end

            S_PAYLOAD: begin
                m_axis_tdata  = combined_data;
                m_axis_tkeep  = combined_keep;
                m_axis_tvalid = s_axis_tvalid;
                s_axis_tready = m_axis_tready;

                if (s_axis_tlast && residual_cnt == 4'd0)
                    m_axis_tlast = 1'b1;
            end

            S_EXTRA: begin
                m_axis_tdata  = extra_data;
                m_axis_tkeep  = extra_keep;
                m_axis_tvalid = 1'b1;
                m_axis_tlast  = 1'b1;
                s_axis_tready = 1'b0;
            end

            default: begin
                s_axis_tready = 1'b0;
            end
        endcase
    end

    // ----------------------------------------------------------------
    // Next-state logic
    // ----------------------------------------------------------------
    always_comb begin
        state_nxt    = state_r;
        beat_cnt_nxt = beat_cnt_r;

        case (state_r)
            S_SKIP: begin
                if (xfer_in) begin
                    if (s_axis_tlast &&
                        beat_cnt_r != CNT_W'(SKIP_BEATS)) begin
                        // Runt packet — tlast before boundary beat
                        beat_cnt_nxt = '0;

                    end else if (beat_cnt_r == CNT_W'(SKIP_BEATS)) begin
                        // Boundary beat reached
                        beat_cnt_nxt = '0;
                        state_nxt    = s_axis_tlast ? S_EXTRA : S_PAYLOAD;

                    end else begin
                        beat_cnt_nxt = beat_cnt_r + CNT_W'(1);
                    end
                end
            end

            S_PAYLOAD: begin
                if (xfer_in && s_axis_tlast) begin
                    state_nxt = (residual_cnt > 4'd0) ? S_EXTRA : S_SKIP;
                end
            end

            S_EXTRA: begin
                if (xfer_out)
                    state_nxt = S_SKIP;
            end

            default: state_nxt = S_SKIP;
        endcase
    end

    // ----------------------------------------------------------------
    // Sequential: state, beat counter, residual register
    // ----------------------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state_r     <= S_SKIP;
            beat_cnt_r  <= '0;
            prev_data_r <= '0;
            prev_keep_r <= '0;
        end else begin
            state_r    <= state_nxt;
            beat_cnt_r <= beat_cnt_nxt;

            // Latch boundary beat (S_SKIP -> first residual)
            if (state_r == S_SKIP && xfer_in &&
                beat_cnt_r == CNT_W'(SKIP_BEATS)) begin
                prev_data_r <= s_axis_tdata;
                prev_keep_r <= s_axis_tkeep;

            // Latch every payload beat for next-cycle combination
            end else if (state_r == S_PAYLOAD && xfer_in) begin
                prev_data_r <= s_axis_tdata;
                prev_keep_r <= s_axis_tkeep;
            end
        end
    end

endmodule