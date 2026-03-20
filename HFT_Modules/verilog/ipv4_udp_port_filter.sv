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
    // Beat 1 = bytes  8-15 → EtherType at bytes 12-13 → beat-local offset 4-5
    // Beat 2 = bytes 16-23 → IP proto  at byte  23    → beat-local offset 7
    // Beat 4 = bytes 32-39 → UDP dport at bytes 36-37 → beat-local offset 4-5
    // All beat indices are 0-indexed (beat 0 is consumed in IDLE)
    // ----------------------------------------------------------------
    localparam BEAT_ETHERTYPE = 3'd1;
    localparam BEAT_IP_PROTO  = 3'd2;
    localparam BEAT_UDP_DPORT = 3'd4;

    // ----------------------------------------------------------------
    // FSM states
    // ----------------------------------------------------------------
    typedef enum logic [1:0] {
        IDLE     = 2'd0,
        INSPECT  = 2'd1,
        PASS     = 2'd2,
        DROP     = 2'd3
    } state_t;

    state_t state, state_next;

    // ----------------------------------------------------------------
    // Internal signals
    // ----------------------------------------------------------------
    logic [2:0]  beat_count;
    logic [2:0]  beat_count_next;

    // Registered header fields (latched as each header beat arrives)
    logic [15:0] ethertype;
    logic [7:0]  ip_proto;
    logic [15:0] udp_dport;

    // Check results (driven from combinational wires below)
    logic ethertype_ok;
    logic ip_proto_ok;
    logic udp_dport_ok;

    logic beat_transferred;
    assign beat_transferred = s_axis_tvalid & s_axis_tready;

    // ----------------------------------------------------------------
    // Pass-through datapath
    // ----------------------------------------------------------------
    assign m_axis_tdata  = s_axis_tdata;
    assign m_axis_tkeep  = s_axis_tkeep;
    assign m_axis_tlast  = s_axis_tlast;
    assign m_axis_tvalid = s_axis_tvalid & (state == PASS);
    assign s_axis_tready = (state == PASS) ? m_axis_tready : 1'b1;

    logic [15:0] ethertype_now;
    logic [7:0]  ip_proto_now;
    logic [15:0] udp_dport_now;

    always_comb begin
        // Default: use the previously registered values
        ethertype_now = ethertype;
        ip_proto_now  = ip_proto;
        udp_dport_now = udp_dport;
        // Override with live beat data when we are on the relevant beat
        if (beat_transferred) begin
            case (beat_count)
                3'd1: ethertype_now = {s_axis_tdata[39:32], s_axis_tdata[47:40]};
                3'd2: ip_proto_now  =  s_axis_tdata[63:56];   
                3'd4: udp_dport_now = {s_axis_tdata[39:32], s_axis_tdata[47:40]};
                default: ;
            endcase
        end
    end

    assign ethertype_ok = (ethertype_now == 16'h0800);
    assign ip_proto_ok  = (ip_proto_now  ==  8'h11  );
    assign udp_dport_ok = (udp_dport_now == FILTER_PORT);

    // ----------------------------------------------------------------
    // Beat counter
    // ----------------------------------------------------------------
    always_comb begin
        beat_count_next = beat_count;
        if (beat_transferred) begin
            if (s_axis_tlast)
                beat_count_next = '0;
            else
                beat_count_next = beat_count + 3'd1;
        end
    end

    // ----------------------------------------------------------------
    // Header field latch (registered copy — still useful for any logic
    // that needs the value in cycles after the header beat)
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            ethertype <= '0;
            ip_proto  <= '0;
            udp_dport <= '0;
        end else if (beat_transferred) begin
            case (beat_count)
                3'd1: ethertype <= {s_axis_tdata[39:32], s_axis_tdata[47:40]};
                3'd2: ip_proto  <=  s_axis_tdata[63:56];
                3'd4: udp_dport <= {s_axis_tdata[39:32], s_axis_tdata[47:40]};
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
                    // Early DROP paths: fail as soon as a bad field is seen
                    if (beat_count == BEAT_ETHERTYPE && !ethertype_ok)
                        state_next = DROP;
                    else if (beat_count == BEAT_IP_PROTO && !ip_proto_ok)
                        state_next = DROP;
                    // All three checks use *_now so udp_dport is live on this beat
                    else if (beat_count == BEAT_UDP_DPORT) begin
                        if (ethertype_ok && ip_proto_ok && udp_dport_ok)
                            state_next = PASS;
                        else
                            state_next = DROP;
                    end
                end
            end

            PASS: begin
                if (beat_transferred && s_axis_tlast)
                    state_next = IDLE;
            end

            DROP: begin
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