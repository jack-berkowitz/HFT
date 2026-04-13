`ifndef __SYS_DEFS_SVH__
`define __SYS_DEFS_SVH__

`define DATA_W 64
typedef logic [`DATA_W-1:0]  AXI_TDATA;
`define KEEP_W (`DATA_W/8)
typedef logic [`KEEP_W-1:0]  AXI_TKEEP;

`define XDP_PORT 16'd11101

`define XDP_HDR_BYTES 58
`define XDP_HDR_BEATS 7
`define XDP_CHAIN_FRAMER_HDR 10

`define XDP_PKT_HDR_SZ 16
`define XDP_MSG_HDR_SZ  4

`define XDP_MSG_ADD_ORDER    16'd100
`define XDP_MSG_MOD_ORDER    16'd101
`define XDP_MSG_DEL_ORDER    16'd102
`define XDP_MSG_EXEC_ORDER   16'd103
`define XDP_MSG_REPLACE      16'd104

`define MSG_ADD    3'd0
`define MSG_MOD    3'd1
`define MSG_DEL    3'd2
`define MSG_EXEC   3'd3
`define MSG_REPL   3'd4

`define XDP_ADD_ORDER_SZ     39
`define XDP_MOD_ORDER_SZ     35
`define XDP_DEL_ORDER_SZ     25
`define XDP_EXEC_ORDER_SZ    42
`define XDP_REPLACE_SZ       42

`define SIDE_BUY   1'b0
`define SIDE_SELL  1'b1

`define TOB_LEVELS  3
`define TOB_SYMBOLS 500
`define SYM_IDX_W   9

`define IDX_SYMBOL  9'd500

`define WEIGHT_FRAC_BITS  20
`define WEIGHT_W          32
`define ACCUM_W           64

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

    logic        side;
    logic [7:0]  printable;
} pillar_msg_t;

typedef struct packed {
    logic [31:0] price;
    logic [31:0] qty;
} price_level_t;

typedef struct packed {
    logic                     valid;
    logic [`SYM_IDX_W-1:0]   symbol_idx;
    price_level_t             best_bid;
    price_level_t             best_ask;
} tob_out_t;

typedef struct packed {
    logic                    valid;
    logic                    direction;
    logic signed [`ACCUM_W-1:0] spread;
    logic [`ACCUM_W-1:0]    computed_index;
    logic [31:0]             actual_price;
} trade_signal_t;

typedef struct packed {
    logic        valid;
    pillar_msg_t pillar_msg;

    logic [31:0] symbol_idx;
    logic [31:0] old_price;
    logic [31:0] old_qty;
    logic        old_side;

    logic [2:0]  msg_type;
} order_lookup_out_t;

typedef struct packed {
    logic [63:0] order_id;
    logic [31:0] price;
    logic [31:0] qty;
    logic        side;
} hash_entry_t;

`timescale 1ns/100ps

`ifndef SYNTH
`endif

`endif
