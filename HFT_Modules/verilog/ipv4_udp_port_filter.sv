`include "sys_defs.svh"

module ipv4_udp_port_filter #(
    parameter [15:0] FILTER_PORT = `XDP_PORT
)(
    input  logic                 clk,
    input  logic                 rst_n,

    input  AXI_TDATA             s_axis_tdata,
    input  AXI_TKEEP             s_axis_tkeep,
    input  logic                 s_axis_tvalid,
    input  logic                 s_axis_tlast,
    output logic                 s_axis_tready,

    output AXI_TDATA             m_axis_tdata,
    output AXI_TKEEP             m_axis_tkeep,
    output logic                 m_axis_tvalid,
    output logic                 m_axis_tlast,
    input  logic                 m_axis_tready
);

    localparam BEAT_ETHERTYPE = 3'd1;
    localparam BEAT_IP_PROTO  = 3'd2;
    localparam BEAT_UDP_DPORT = 3'd4;
    localparam BEAT_DECISION  = 3'd5;

    typedef enum logic [1:0] {
        IDLE    = 2'd0,
        INSPECT = 2'd1,
        PASS    = 2'd2,
        DROP    = 2'd3
    } state_t;

    state_t state, state_next;

    logic [2:0]  beat_count, beat_count_next;

    logic [15:0] ethertype;
    logic [7:0]  ip_proto;
    logic [15:0] udp_dport;

    logic ethertype_ok;
    logic ip_proto_ok;
    logic udp_dport_ok;

    logic beat_transferred;

    assign ethertype_ok = (ethertype == 16'h0800);
    assign ip_proto_ok  = (ip_proto  ==  8'h11);
    assign udp_dport_ok = (udp_dport == FILTER_PORT);

    assign beat_transferred = s_axis_tvalid & s_axis_tready;

    assign m_axis_tdata = s_axis_tdata;
    assign m_axis_tkeep = s_axis_tkeep;
    assign m_axis_tlast = s_axis_tlast;

    assign m_axis_tvalid = s_axis_tvalid & (state == PASS);

    assign s_axis_tready = (state == PASS) ? m_axis_tready : 1'b1;

    always_comb begin
        beat_count_next = beat_count;
        if (beat_transferred) begin
            if (s_axis_tlast)
                beat_count_next = '0;
            else
                beat_count_next = beat_count + 3'd1;
        end
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            ethertype <= '0;
            ip_proto  <= '0;
            udp_dport <= '0;
        end else if (beat_transferred) begin
            case (beat_count)
                BEAT_ETHERTYPE: ethertype <= {s_axis_tdata[39:32], s_axis_tdata[47:40]};
                BEAT_IP_PROTO:  ip_proto  <=  s_axis_tdata[63:56];
                BEAT_UDP_DPORT: udp_dport <= {s_axis_tdata[39:32], s_axis_tdata[47:40]};
                default: ;
            endcase
        end
    end

    always_comb begin
        state_next = state;
        case (state)
            IDLE: begin
                if (s_axis_tvalid)
                    state_next = INSPECT;
            end

            INSPECT: begin
                if (beat_transferred) begin
                    if (s_axis_tlast)
                        state_next = IDLE;
                    else if (beat_count == BEAT_DECISION) begin
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
