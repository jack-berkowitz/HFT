`include "sys_defs.svh"
// ============================================================
// xdp_packet_framer.sv
//
// Strips the 58-byte network + XDP packet header from an
// AXI-Stream packet and realigns the remaining XDP message
// payload so byte 0 starts at tdata[7:0].
//
// Header breakdown:
//   Ethernet     14 B
//   IPv4         20 B
//   UDP           8 B
//   XDP Pkt Hdr  16 B  (PktSize, DeliveryFlag, NumberMsgs,
//                        SeqNum, SendTime, SendTimeNS)
//   ─────────────────
//   Total        58 B
//
// On a 64-bit (8-byte) bus, 58 bytes = 7 full beats + 2 leftover
// header bytes in beat 7.  The upper 6 bytes of beat 7 are the
// first payload bytes (start of the first XDP message).
//
//   SKIP_BEATS = 7     beats consumed silently (beats 0–6)
//   OFFSET     = 2     header bytes in the boundary beat (beat 7)
//   REMAIN     = 6     payload bytes in the boundary beat
//
// Realignment barrel shift (every output beat):
//   output = { cur_tdata[15:0],  prev_tdata[63:16] }
//              2 bytes (from cur)  6 bytes (from prev)
//
// Latency: 1 cycle after the boundary beat (S_FILL state).
// ============================================================
module xdp_packet_framer #(
    parameter HEADER_BYTES = `XDP_HDR_BYTES   // 58
)(
    input  logic        clk,
    input  logic        rst_n,

    // Slave AXI-Stream (from filter)
    input  AXI_TDATA    s_axis_tdata,
    input  AXI_TKEEP    s_axis_tkeep,
    input  logic        s_axis_tvalid,
    input  logic        s_axis_tlast,
    output logic        s_axis_tready,

    // Master AXI-Stream (to demux — clean XDP message stream)
    output AXI_TDATA    m_axis_tdata,
    output AXI_TKEEP    m_axis_tkeep,
    output logic        m_axis_tvalid,
    output logic        m_axis_tlast,
    input  logic        m_axis_tready
);

    // ----------------------------------------------------------------
    // Derived constants
    // ----------------------------------------------------------------
    localparam KEEP_W      = `KEEP_W;                          // 8
    localparam SKIP_BEATS  = HEADER_BYTES / KEEP_W;            // 7
    localparam OFFSET      = HEADER_BYTES % KEEP_W;            // 2
    localparam REMAIN      = KEEP_W - OFFSET;                  // 6
    localparam CNT_W       = $clog2(SKIP_BEATS + 2);           // beat counter width

    // ----------------------------------------------------------------
    // FSM
    // ----------------------------------------------------------------
    typedef enum logic [1:0] {
        S_SKIP    = 2'd0,   // consuming header beats (0 .. SKIP_BEATS-1)
        S_FILL    = 2'd1,   // 1-cycle hold after boundary beat to let input settle
        S_PAYLOAD = 2'd2,   // realigning and forwarding payload
        S_IDLE    = 2'd3    // between packets
    } state_t;

    state_t state_r, state_next;

    // ----------------------------------------------------------------
    // Registers
    // ----------------------------------------------------------------
    logic [CNT_W-1:0]  beat_cnt_r;
    AXI_TDATA          prev_data_r;     // previous beat's tdata (for barrel shift)
    AXI_TKEEP          prev_keep_r;     // previous beat's tkeep
    logic              prev_last_r;     // previous beat's tlast

    // ----------------------------------------------------------------
    // Combined (barrel-shifted) output
    //
    // The payload is shifted left by OFFSET bytes so that payload
    // byte 0 lands at tdata[7:0]:
    //   combined = { cur[OFFSET*8-1 : 0],  prev[63 : OFFSET*8] }
    //            = { cur[15:0],             prev[63:16]          }
    // ----------------------------------------------------------------
    AXI_TDATA combined_data;
    AXI_TKEEP combined_keep;

    assign combined_data = { s_axis_tdata[OFFSET*8-1:0],
                             prev_data_r [63:OFFSET*8]    };

    // tkeep shift: valid bytes from previous (upper REMAIN bits)
    // merged with valid bytes from current (lower OFFSET bits)
    assign combined_keep = { s_axis_tkeep[OFFSET-1:0],
                             prev_keep_r [KEEP_W-1:OFFSET] };

    // ----------------------------------------------------------------
    // Output mux
    // ----------------------------------------------------------------
    always_comb begin
        m_axis_tdata  = '0;
        m_axis_tkeep  = '0;
        m_axis_tvalid = 1'b0;
        m_axis_tlast  = 1'b0;
        s_axis_tready = 1'b1;   // default: absorb input

        case (state_r)
            S_SKIP: begin
                // Silently consume header beats
                s_axis_tready = 1'b1;
            end

            S_FILL: begin
                // 1-cycle hold — don't consume input, don't produce output
                s_axis_tready = 1'b0;
            end

            S_PAYLOAD: begin
                // Check if previous beat was the last
                if (prev_last_r) begin
                    // Flush residual bytes from prev — no new data needed
                    m_axis_tdata  = prev_data_r >> (OFFSET * 8);
                    m_axis_tkeep  = prev_keep_r >> OFFSET;
                    m_axis_tvalid = |m_axis_tkeep;  // only if there are valid residual bytes
                    m_axis_tlast  = 1'b1;
                    s_axis_tready = 1'b0;           // don't consume next packet's data
                end else begin
                    // Normal: combine previous and current
                    m_axis_tdata  = combined_data;
                    m_axis_tkeep  = combined_keep;
                    m_axis_tvalid = s_axis_tvalid;
                    s_axis_tready = m_axis_tready;

                    // If the current input beat is the last, check whether
                    // a flush cycle is needed. Flush is needed only if
                    // shifting cur's tkeep right by OFFSET leaves valid bytes.
                    // If not, this combined beat IS the last output beat.
                    if (s_axis_tvalid && s_axis_tlast)
                        m_axis_tlast = ~(|s_axis_tkeep[KEEP_W-1:OFFSET]);
                    else
                        m_axis_tlast = 1'b0;
                end
            end

            S_IDLE: begin
                s_axis_tready = 1'b0;
            end
        endcase
    end

    // ----------------------------------------------------------------
    // FSM next-state
    // ----------------------------------------------------------------
    always_comb begin
        state_next = state_r;
        case (state_r)
            S_IDLE: begin
                if (s_axis_tvalid)
                    state_next = S_SKIP;
            end

            S_SKIP: begin
                if (s_axis_tvalid && s_axis_tready) begin
                    if (beat_cnt_r == CNT_W'(SKIP_BEATS))
                        // Boundary beat — always go to S_FILL, even if tlast
                        // (payload bytes exist in the upper lanes of this beat)
                        state_next = S_FILL;
                    else if (s_axis_tlast)
                        // Tiny packet (all header, ends before boundary) — no payload
                        state_next = S_IDLE;
                end
            end

            S_FILL: begin
                // Always transition after exactly one cycle
                state_next = S_PAYLOAD;
            end

            S_PAYLOAD: begin
                if (prev_last_r) begin
                    // Flushing residuals
                    if (|prev_keep_r[KEEP_W-1:OFFSET])
                        // Wait for downstream to accept flush beat
                        if (m_axis_tvalid && m_axis_tready)
                            state_next = S_IDLE;
                        else
                            state_next = S_PAYLOAD;  // stall until accepted
                    else
                        state_next = S_IDLE;
                end else if (s_axis_tvalid && s_axis_tready && s_axis_tlast) begin
                    // Current input beat is the last
                    if (|s_axis_tkeep[KEEP_W-1:OFFSET])
                        state_next = S_PAYLOAD;  // need flush cycle for residuals
                    else
                        state_next = S_IDLE;     // no residuals, tlast set on this beat
                end
            end
        endcase
    end

    // ----------------------------------------------------------------
    // Registers
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            state_r     <= S_IDLE;
            beat_cnt_r  <= '0;
            prev_data_r <= '0;
            prev_keep_r <= '0;
            prev_last_r <= 1'b0;
        end else begin
            state_r <= state_next;

            case (state_r)
                S_IDLE: begin
                    beat_cnt_r  <= '0;
                    prev_last_r <= 1'b0;
                end

                S_SKIP: begin
                    if (s_axis_tvalid && s_axis_tready) begin
                        beat_cnt_r  <= beat_cnt_r + 1'b1;
                        prev_data_r <= s_axis_tdata;
                        prev_keep_r <= s_axis_tkeep;
                        prev_last_r <= s_axis_tlast;
                    end
                end

                S_FILL: begin
                    // prev_data_r already holds boundary beat — don't touch
                    beat_cnt_r <= '0;
                end

                S_PAYLOAD: begin
                    if (prev_last_r) begin
                        // After flush, reset
                        if (state_next == S_IDLE) begin
                            prev_last_r <= 1'b0;
                            beat_cnt_r  <= '0;
                        end
                    end else if (s_axis_tvalid && s_axis_tready) begin
                        prev_data_r <= s_axis_tdata;
                        prev_keep_r <= s_axis_tkeep;
                        prev_last_r <= s_axis_tlast;
                    end
                end
            endcase
        end
    end

endmodule