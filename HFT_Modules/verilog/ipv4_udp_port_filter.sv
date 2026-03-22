`include "sys_defs.svh"
// ============================================================
// ipv4_udp_port_filter.sv
//
// AXI-Stream inline filter for 64-bit (10G Corundum) bus.
// Inspects EtherType, IP protocol, and UDP destination port.
// Only packets matching all three checks have m_axis_tvalid
// asserted; all others are silently consumed.
//
// Updated for NYSE XDP: default FILTER_PORT = XDP multicast port.
//
// Beat-level field positions (64-bit bus, 0-indexed beats):
//   Beat 1 — EtherType  at bytes 12-13 → tdata[47:32]
//   Beat 2 — IP proto   at byte  23    → tdata[63:56]
//   Beat 4 — UDP dport  at bytes 36-37 → tdata[47:32]
//
// FSM: IDLE → INSPECT (beats 0..5) → PASS or DROP (until tlast)
//
// Note: udp_dport is latched on beat 4 via NBA (<=). The FSM
// decision is deferred to beat 5 so the registered value has
// settled before the combinational check reads it.
// ============================================================
module ipv4_udp_port_filter #(
    parameter [15:0] FILTER_PORT = `XDP_PORT
)(
    input  logic                 clk,
    input  logic                 rst_n,

    // Slave AXI-Stream input (from MAC)
    input  AXI_TDATA             s_axis_tdata,
    input  AXI_TKEEP             s_axis_tkeep,
    input  logic                 s_axis_tvalid,
    input  logic                 s_axis_tlast,
    output logic                 s_axis_tready,

    // Master AXI-Stream output (to framer)
    output AXI_TDATA             m_axis_tdata,
    output AXI_TKEEP             m_axis_tkeep,
    output logic                 m_axis_tvalid,
    output logic                 m_axis_tlast,
    input  logic                 m_axis_tready
);

    // ----------------------------------------------------------------
    // Constants — beat indices where header fields live
    // ----------------------------------------------------------------
    localparam BEAT_ETHERTYPE = 3'd1;
    localparam BEAT_IP_PROTO  = 3'd2;
    localparam BEAT_UDP_DPORT = 3'd4;   // extraction beat
    localparam BEAT_DECISION  = 3'd5;   // check one beat after last latch (NBA settled)

    // ----------------------------------------------------------------
    // FSM states
    // ----------------------------------------------------------------
    typedef enum logic [1:0] {
        IDLE    = 2'd0,
        INSPECT = 2'd1,
        PASS    = 2'd2,
        DROP    = 2'd3
    } state_t;

    state_t state, state_next;

    // ----------------------------------------------------------------
    // Internal signals
    // ----------------------------------------------------------------
    logic [2:0]  beat_count, beat_count_next;

    // Latched header fields
    logic [15:0] ethertype;
    logic [7:0]  ip_proto;
    logic [15:0] udp_dport;

    // Check results
    logic ethertype_ok;
    logic ip_proto_ok;
    logic udp_dport_ok;

    logic beat_transferred;

    // ----------------------------------------------------------------
    // Combinational checks
    // ----------------------------------------------------------------
    assign ethertype_ok = (ethertype == 16'h0800);   // IPv4
    assign ip_proto_ok  = (ip_proto  ==  8'h11);     // UDP
    assign udp_dport_ok = (udp_dport == FILTER_PORT);

    assign beat_transferred = s_axis_tvalid & s_axis_tready;

    // ----------------------------------------------------------------
    // Pass-through datapath — data always wired, tvalid gated
    // ----------------------------------------------------------------
    assign m_axis_tdata = s_axis_tdata;
    assign m_axis_tkeep = s_axis_tkeep;
    assign m_axis_tlast = s_axis_tlast;

    // Forward tvalid only in PASS state
    assign m_axis_tvalid = s_axis_tvalid & (state == PASS);

    // Backpressure: absorb from downstream in PASS, always ready otherwise
    assign s_axis_tready = (state == PASS) ? m_axis_tready : 1'b1;

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
    // Header field extraction — latch on the relevant beat
    //
    // Byte ordering on 64-bit LE AXI-Stream bus:
    //   tdata[7:0]   = byte at beat_offset+0
    //   tdata[15:8]  = byte at beat_offset+1
    //   ...
    //   tdata[63:56] = byte at beat_offset+7
    //
    // EtherType is bytes 12-13 (big-endian on wire):
    //   byte 12 = tdata[39:32], byte 13 = tdata[47:40]
    //   ethertype = {byte12, byte13} = {tdata[39:32], tdata[47:40]}
    //
    // IP proto is byte 23:
    //   byte 23 = tdata[63:56]
    //
    // UDP dport is bytes 36-37 (big-endian on wire):
    //   byte 36 = tdata[39:32], byte 37 = tdata[47:40]  (beat 4 starts at byte 32)
    //   Offsets within beat 4: byte 36 is offset 4, byte 37 is offset 5
    //   udp_dport = {byte36, byte37} = {tdata[39:32], tdata[47:40]}
    // ----------------------------------------------------------------
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
                    // Tiny packet that ends before decision? Go back to IDLE.
                    if (s_axis_tlast)
                        state_next = IDLE;
                    // All three fields latched by beat 4; check on beat 5
                    // so the NBA write of udp_dport has settled.
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

    // ----------------------------------------------------------------
    // FSM + beat counter registers
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