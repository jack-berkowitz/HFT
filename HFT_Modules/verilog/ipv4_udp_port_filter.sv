`include "sys_defs.svh"
module ipv4_udp_port_filter #(
    parameter [15:0] FILTER_PORT = `MDP3_PORT
)(
    input  logic                 clk,
    input  logic                 rst_n,

    // Slave AXI-Stream input
    input  AXI_TDATA             s_axis_tdata,
    input  AXI_TKEEP             s_axis_tkeep,
    input  logic                 s_axis_tvalid,
    input  logic                 s_axis_tlast,
    output logic                 s_axis_tready,

    // Master AXI-Stream output
    output AXI_TDATA            m_axis_tdata,
    output AXI_TKEEP            m_axis_tkeep,
    output logic                m_axis_tvalid,
    output logic                m_axis_tlast,
    input  logic                m_axis_tready
);
    // ----------------------------------------------------------------
    // Constants — byte positions within the 64-bit beat
    // Beat 2 = bytes 8-15  → EtherType at bytes 12-13 → beat-local offset 4-5
    // Beat 3 = bytes 16-23 → IP proto  at byte  23    → beat-local offset 7
    // Beat 5 = bytes 32-39 → UDP dport at bytes 36-37 → beat-local offset 4-5
    // ----------------------------------------------------------------
    localparam BEAT_ETHERTYPE = 3'd1;   // beat index where EtherType lives (0-indexed)
    localparam BEAT_IP_PROTO  = 3'd2;   // same beat as EtherType (byte 23 is beat 2 bytes 16-23)
    localparam BEAT_UDP_DPORT = 3'd4;   // beat index where UDP dst port lives

    // ----------------------------------------------------------------
    // FSM states
    // ----------------------------------------------------------------
    typedef enum logic [1:0] {
        IDLE     = 2'd0,   // waiting for first beat of a new packet
        INSPECT  = 2'd1,   // receiving beats, checking header fields
        PASS     = 2'd2,   // all checks passed, forward remaining beats
        DROP     = 2'd3    // a check failed, suppress remaining beats
    } state_t;

    state_t state, state_next;

    // ----------------------------------------------------------------
    // Internal signals
    // ----------------------------------------------------------------
    logic [2:0]  beat_count;        // which beat we are on (0-indexed)
    logic [2:0]  beat_count_next;

    // Latched header fields
    logic [15:0] ethertype;         // latched from beat 1
    logic [7:0]  ip_proto;          // latched from beat 2
    logic [15:0] udp_dport;         // latched from beat 4

    // Check results
    logic ethertype_ok;
    logic ip_proto_ok;
    logic udp_dport_ok;

    logic beat_transferred;         // true when a beat actually moves (tvalid & tready)

    // ----------------------------------------------------------------
    // Combinational checks against latched values
    // ----------------------------------------------------------------
    assign ethertype_ok = (ethertype == 16'h0800);
    assign ip_proto_ok  = (ip_proto  ==  8'h11  );
    assign udp_dport_ok = (udp_dport == FILTER_PORT);

    // A beat transfers only when both sides are ready
    assign beat_transferred = s_axis_tvalid & s_axis_tready;

    // ----------------------------------------------------------------
    // Pass-through datapath
    // Data/keep/last are always wired through — tvalid is gated by state
    // ----------------------------------------------------------------
    assign m_axis_tdata = s_axis_tdata;
    assign m_axis_tkeep = s_axis_tkeep;
    assign m_axis_tlast = s_axis_tlast;

    // Only forward tvalid downstream when in PASS state
    assign m_axis_tvalid = s_axis_tvalid & (state == PASS);

    // Absorb backpressure from downstream during PASS, always ready during DROP/INSPECT
    assign s_axis_tready = (state == PASS) ? m_axis_tready : 1'b1;

    // ----------------------------------------------------------------
    // Beat counter
    // ----------------------------------------------------------------
    always_comb begin
        beat_count_next = beat_count;
        if (beat_transferred) begin
            if (s_axis_tlast)
                beat_count_next = '0;           // reset on end of packet
            else
                beat_count_next = beat_count + 3'd1;
        end
    end

    // ----------------------------------------------------------------
    // Header field extraction
    // Called combinationally on the relevant beat
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            ethertype <= '0;
            ip_proto  <= '0;
            udp_dport <= '0;
        end else if (beat_transferred) begin
            case (beat_count)
                3'd1: ethertype <= {s_axis_tdata[47:40], s_axis_tdata[39:32]}; // bytes 13:12
                3'd2: ip_proto  <=  s_axis_tdata[63:56];                        // byte 23
                3'd4: udp_dport <= {s_axis_tdata[47:40], s_axis_tdata[39:32]}; // bytes 37:36
                default: ;
            endcase
        end
    end

    // ----------------------------------------------------------------
    // FSM next-state logic
    // ----------------------------------------------------------------
    always_comb begin
        state_next = state;
        case (state)
            IDLE: begin
                if (s_axis_tvalid)
                    state_next = INSPECT;
            end

            INSPECT: begin
                if (beat_transferred) begin
                    // Once we have seen the UDP dst port beat we can decide
                    if (beat_count == BEAT_UDP_DPORT) begin
                        if (ethertype_ok && ip_proto_ok && udp_dport_ok)
                            state_next = PASS;
                        else
                            state_next = DROP;
                    end
                end
            end

            PASS: begin
                // Stay in PASS until end of packet
                if (beat_transferred && s_axis_tlast)
                    state_next = IDLE;
            end

            DROP: begin
                // Stay in DROP until end of packet, then go back to IDLE
                if (beat_transferred && s_axis_tlast)
                    state_next = IDLE;
            end
        endcase
    end

    // ----------------------------------------------------------------
    // FSM state register + beat counter register
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            state      <= IDLE;
            beat_count <= '0;
        end else begin
            state      <= state_next;
            beat_count <= beat_count_next;
        end
    end

endmodule