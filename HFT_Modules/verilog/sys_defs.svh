/////////////////////////////////////////////////////////////////////////
//                                                                     //
//   Modulename :  sys_defs.svh                                        //
//                                                                     //
//  Description :  Macros and data structures for the HFT pipeline.    //
//                 Updated for NYSE XDP/Pillar protocol.               //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

`ifndef __SYS_DEFS_SVH__
`define __SYS_DEFS_SVH__

// ----------------------------------------------------------------
// AXI-Stream bus definitions (64-bit / 10G Corundum config)
// ----------------------------------------------------------------
`define DATA_W 64
typedef logic [`DATA_W-1:0]  AXI_TDATA;
`define KEEP_W (`DATA_W/8)
typedef logic [`KEEP_W-1:0]  AXI_TKEEP;

// ----------------------------------------------------------------
// NYSE XDP Protocol Constants
// ----------------------------------------------------------------

// Default XDP multicast port (varies by channel — override per-instance)
`define XDP_PORT 16'd11101

// Network + XDP Packet Header overhead (bytes before first XDP message)
//   Ethernet 14 + IPv4 20 + UDP 8 + XDP Packet Header 16 = 58
`define XDP_HDR_BYTES 58

// Number of full beats to skip before boundary beat (58 / 8 = 7 full + 2 rem)
`define XDP_HDR_BEATS 7

// When the framer sits behind the port filter, the filter consumes
// beats 0–5 (48 bytes) during INSPECT.  The framer only needs to
// strip the remaining header bytes: 58 - 48 = 10.
`define XDP_CHAIN_FRAMER_HDR 10

// XDP Packet Header size (bytes)
`define XDP_PKT_HDR_SZ 16

// XDP Message Header size (bytes): MsgSize(2) + MsgType(2)
`define XDP_MSG_HDR_SZ  4

// XDP Message Types — NYSE Integrated Feed (order book relevant)
`define XDP_MSG_ADD_ORDER    16'd100
`define XDP_MSG_MOD_ORDER    16'd101
`define XDP_MSG_DEL_ORDER    16'd102
`define XDP_MSG_EXEC_ORDER   16'd103
`define XDP_MSG_REPLACE      16'd104

// XDP message sizes (bytes, including 4-byte msg header)
`define XDP_ADD_ORDER_SZ     39
`define XDP_MOD_ORDER_SZ     35
`define XDP_DEL_ORDER_SZ     25
`define XDP_EXEC_ORDER_SZ    42
`define XDP_REPLACE_SZ       42

// ----------------------------------------------------------------
// Pillar decoded message struct
//
// Single superset struct carrying every field that any of the five
// order message types (100-104) can produce.  Unused fields are
// zeroed for a given msg_type.
//
//   valid          — pulses high for exactly one cycle per message
//   msg_type       — 100/101/102/103/104
//   symbol_index   — SymbolIndex from Symbol Index Mapping (all types)
//   symbol_seq_num — per-symbol sequence number (all types)
//   order_id       — 8-byte matching-engine order ID (all types)
//   new_order_id   — replacement order ID (104 Replace only)
//   price          — integer price, use PriceScaleCode to interpret
//                    (100 Add, 101 Mod, 103 Exec, 104 Replace)
//   qty            — share volume
//                    (100 Add, 101 Mod, 103 Exec, 104 Replace)
//   trade_id       — execution ID (103 Exec only)
//   side           — ASCII 'B'=Buy / 'S'=Sell (100 Add only)
//   printable      — SIP print flag (103 Exec only)
// ----------------------------------------------------------------
typedef struct packed {
    logic        valid;
    logic [15:0] msg_type;
    logic [31:0] symbol_index;
    logic [31:0] symbol_seq_num;

    logic [63:0] order_id;
    logic [63:0] new_order_id;

    logic [31:0] price;
    logic [31:0] qty;
    logic [31:0] trade_id;

    logic [7:0]  side;        // 'B' / 'S' (Add only, else 0)
    logic [7:0]  printable;   // 0 or 1 (Exec only, else 0)
} pillar_msg_t;

typedef struct packed {
    logic [31:0] bidprice; //BEST BID PRICE
    logic [31:0] bidquant;//BEST BID Q
    logic [31:0] askprice;//BEST ASK PRICE
    logic [31:0] askquant;//BEST ASK Q
} orderbook_t;

typedef struct packed {
    logic        req_valid;
    pillar_msg_t pillar_msg;

    // result of hash-table lookup on order_id
    logic        found;
    logic        old_valid;
    logic [31:0] old_price;
    logic [31:0] old_qty;
    logic [7:0]  old_side;
} order_lookup_out_t;

// ----------------------------------------------------------------
// Timescale
// ----------------------------------------------------------------
`timescale 1ns/100ps

`ifndef SYNTH
    //`define DEBUG
`endif

`endif // __SYS_DEFS_SVH__
