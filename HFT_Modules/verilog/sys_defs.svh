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

`define XDP_PORT 16'd11101

`define XDP_HDR_BYTES 58
`define XDP_HDR_BEATS 7
`define XDP_CHAIN_FRAMER_HDR 10

`define XDP_PKT_HDR_SZ 16
`define XDP_MSG_HDR_SZ  4

// XDP Message Types — raw 16-bit wire values
`define XDP_MSG_ADD_ORDER    16'd100
`define XDP_MSG_MOD_ORDER    16'd101
`define XDP_MSG_DEL_ORDER    16'd102
`define XDP_MSG_EXEC_ORDER   16'd103
`define XDP_MSG_REPLACE      16'd104

// Encoded 3-bit message types (after demux)
`define MSG_ADD    3'd0
`define MSG_MOD    3'd1
`define MSG_DEL    3'd2
`define MSG_EXEC   3'd3
`define MSG_REPL   3'd4

// XDP message sizes (bytes)
`define XDP_ADD_ORDER_SZ     39
`define XDP_MOD_ORDER_SZ     35
`define XDP_DEL_ORDER_SZ     25
`define XDP_EXEC_ORDER_SZ    42
`define XDP_REPLACE_SZ       42

// ----------------------------------------------------------------
// Side encoding:  0 = Buy (bid),  1 = Sell (ask)
// ----------------------------------------------------------------
`define SIDE_BUY   1'b0
`define SIDE_SELL  1'b1

// ----------------------------------------------------------------
// Top-of-book parameters
// ----------------------------------------------------------------
`define TOB_LEVELS  3       // price levels tracked per side per symbol
`define TOB_SYMBOLS 501     // S&P 500 components (0-499) + index tracker (500)
`define SYM_IDX_W   9       // ceil(log2(501)) = 9 bits

// The symbol index assigned to the actual index instrument (SPY/ES)
`define IDX_SYMBOL  9'd500

// ----------------------------------------------------------------
// Fixed-point arithmetic for index calculation
//   Weights are Q12.20:  12 integer bits, 20 fractional bits
//   All weights sum to 1.0  (= 1 << 20 = 1_048_576)
//   Accumulator is Q44.20:  64 bits total
// ----------------------------------------------------------------
`define WEIGHT_FRAC_BITS  20
`define WEIGHT_W          32
`define ACCUM_W           64

// ----------------------------------------------------------------
// Pillar decoded message struct (output of xdp_msg_demux)
// ----------------------------------------------------------------
typedef struct packed {
    logic        valid;
    logic [2:0]  msg_type;
    logic [31:0] symbol_index;
    logic [31:0] symbol_seq_num;

    logic [63:0] order_id;
    logic [63:0] new_order_id;

    logic [31:0] price;
    logic [31:0] qty;
    logic [31:0] trade_id;

    logic        side;        // 0=Buy, 1=Sell
    logic [7:0]  printable;
} pillar_msg_t;

// ----------------------------------------------------------------
// Price level — one entry in the sorted TOB array
//   qty == 0  means the slot is empty
// ----------------------------------------------------------------
typedef struct packed {
    logic [31:0] price;
    logic [31:0] qty;
} price_level_t;

// ----------------------------------------------------------------
// Top-of-book output snapshot
// ----------------------------------------------------------------
typedef struct packed {
    logic                     valid;
    logic [`SYM_IDX_W-1:0]   symbol_idx;
    price_level_t             best_bid;
    price_level_t             best_ask;
} tob_out_t;

// ----------------------------------------------------------------
// Index arbitrage trade signal
// ----------------------------------------------------------------
typedef struct packed {
    logic                    valid;      // pulses when spread crosses threshold
    logic                    direction;  // 0=BUY index (computed < actual), 1=SELL index
    logic signed [`ACCUM_W-1:0] spread; // signed spread in Q44.20 fixed-point
    logic [`ACCUM_W-1:0]    computed_index;  // our fair-value estimate
    logic [31:0]             actual_price;    // tracked index instrument mid-price
} trade_signal_t;

// ----------------------------------------------------------------
// Output of the order hash lookup stage
// Carries the decoded message alongside the hash-table result
// so the book engine has both wire fields and old-order context.
// ----------------------------------------------------------------
typedef struct packed {
    logic        valid;
    pillar_msg_t pillar_msg;

    // result of hash-table lookup on order_id
    logic [31:0] symbol_idx;
    logic [31:0] old_price;
    logic [31:0] old_qty;
    logic        old_side;

    logic [2:0]  msg_type;
} order_lookup_out_t;

// ----------------------------------------------------------------
// Order hash table entry (stored in BRAM)
// ----------------------------------------------------------------
typedef struct packed {
    logic [63:0] order_id;
    logic [31:0] price;
    logic [31:0] qty;
    logic        side;
} hash_entry_t;

// ----------------------------------------------------------------
// Timescale
// ----------------------------------------------------------------
`timescale 1ns/100ps

`ifndef SYNTH
    //`define DEBUG
`endif

`endif // __SYS_DEFS_SVH__
