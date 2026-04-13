`include "sys_defs.svh"

module xdp_packet_framer #(
    parameter HEADER_BYTES = `XDP_HDR_BYTES
)(
    input  logic        clk,
    input  logic        rst_n,

    input  AXI_TDATA    s_axis_tdata,
    input  AXI_TKEEP    s_axis_tkeep,
    input  logic        s_axis_tvalid,
    input  logic        s_axis_tlast,
    output logic        s_axis_tready,

    output AXI_TDATA    m_axis_tdata,
    output AXI_TKEEP    m_axis_tkeep,
    output logic        m_axis_tvalid,
    output logic        m_axis_tlast,
    input  logic        m_axis_tready
);

    localparam KEEP_W      = `KEEP_W;
    localparam SKIP_BEATS  = HEADER_BYTES / KEEP_W;
    localparam OFFSET      = HEADER_BYTES % KEEP_W;
    localparam REMAIN      = KEEP_W - OFFSET;
    localparam CNT_W       = $clog2(SKIP_BEATS + 2);

    typedef enum logic [1:0] {
        S_SKIP    = 2'd0,
        S_FILL    = 2'd1,
        S_PAYLOAD = 2'd2,
        S_IDLE    = 2'd3
    } state_t;

    state_t state_r, state_next;

    logic [CNT_W-1:0]  beat_cnt_r;
    AXI_TDATA          prev_data_r;
    AXI_TKEEP          prev_keep_r;
    logic              prev_last_r;

    AXI_TDATA combined_data;
    AXI_TKEEP combined_keep;

    assign combined_data = { s_axis_tdata[OFFSET*8-1:0],
                             prev_data_r [63:OFFSET*8]    };

    assign combined_keep = { s_axis_tkeep[OFFSET-1:0],
                             prev_keep_r [KEEP_W-1:OFFSET] };

    always_comb begin
        m_axis_tdata  = '0;
        m_axis_tkeep  = '0;
        m_axis_tvalid = 1'b0;
        m_axis_tlast  = 1'b0;
        s_axis_tready = 1'b1;

        case (state_r)
            S_SKIP: begin
                s_axis_tready = 1'b1;
            end

            S_FILL: begin
                s_axis_tready = 1'b0;
            end

            S_PAYLOAD: begin
                if (prev_last_r) begin
                    m_axis_tdata  = prev_data_r >> (OFFSET * 8);
                    m_axis_tkeep  = prev_keep_r >> OFFSET;
                    m_axis_tvalid = |m_axis_tkeep;
                    m_axis_tlast  = 1'b1;
                    s_axis_tready = 1'b0;
                end else begin
                    m_axis_tdata  = combined_data;
                    m_axis_tkeep  = combined_keep;
                    m_axis_tvalid = s_axis_tvalid;
                    s_axis_tready = m_axis_tready;

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
                        state_next = S_FILL;
                    else if (s_axis_tlast)
                        state_next = S_IDLE;
                end
            end

            S_FILL: begin
                state_next = S_PAYLOAD;
            end

            S_PAYLOAD: begin
                if (prev_last_r) begin
                    if (|prev_keep_r[KEEP_W-1:OFFSET])
                        if (m_axis_tvalid && m_axis_tready)
                            state_next = S_IDLE;
                        else
                            state_next = S_PAYLOAD;
                    else
                        state_next = S_IDLE;
                end else if (s_axis_tvalid && s_axis_tready && s_axis_tlast) begin
                    if (|s_axis_tkeep[KEEP_W-1:OFFSET])
                        state_next = S_PAYLOAD;
                    else
                        state_next = S_IDLE;
                end
            end
        endcase
    end

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
                    beat_cnt_r <= '0;
                end

                S_PAYLOAD: begin
                    if (prev_last_r) begin
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
