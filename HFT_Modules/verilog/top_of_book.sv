`include "sys_defs.svh"
// ============================================================
// top_of_book.sv
//
// Maintains a sorted N-level order book per side per symbol.
// Input: order_lookup_out_t from the hash lookup stage.
// Output: tob_out_t with BBO whenever the book changes.
//
// Storage: register file — sized for TOB_SYMBOLS symbols.
//
// Per symbol, per side, N_LEVELS price levels are sorted:
//   Bids: level[0].price >= level[1].price >= level[2].price
//   Asks: level[0].price <= level[1].price <= level[2].price
//   level[0] = best (BBO).  qty == 0 means empty.
//
// Every message decomposes into at most:
//   1. REMOVE: subtract qty from a matching price level.
//              If level empties, shift deeper levels up.
//   2. ADD:    add qty to an existing matching level, or
//              insert a new level at the correct sorted position.
//
// Message type -> operations:
//   ADD   : ADD(wire price, wire qty, wire side)
//   MOD   : REMOVE(old) -> ADD(wire price, wire qty, old side)
//   DEL   : REMOVE(old)
//   EXEC  : REMOVE(old price, exec qty, old side)
//   REPL  : REMOVE(old) -> ADD(wire price, wire qty, old side)
// ============================================================
module top_of_book #(
    parameter N_LEVELS  = `TOB_LEVELS,
    parameter N_SYMBOLS = `TOB_SYMBOLS
)(
    input  logic              clk,
    input  logic              rst_n,

    input  order_lookup_out_t in_update,

    output tob_out_t          out_tob
);

    localparam SYM_W = $clog2(N_SYMBOLS);

    // ----------------------------------------------------------------
    // Book storage — register arrays
    // ----------------------------------------------------------------
    price_level_t bid_book [N_SYMBOLS][N_LEVELS];
    price_level_t ask_book [N_SYMBOLS][N_LEVELS];

    // ----------------------------------------------------------------
    // Input decode — extract fields from order_lookup_out_t
    // ----------------------------------------------------------------
    logic [SYM_W-1:0] sym;
    logic              sym_valid;
    logic [2:0]        msg_type;

    assign sym       = in_update.pillar_msg.symbol_index[SYM_W-1:0];
    assign sym_valid = in_update.valid &&
                       (in_update.pillar_msg.symbol_index < 32'(N_SYMBOLS));
    assign msg_type  = in_update.msg_type;

    // ----------------------------------------------------------------
    // Operation decode
    // ----------------------------------------------------------------
    logic        do_remove, do_add;
    logic [31:0] rm_price, rm_qty;
    logic [31:0] add_price, add_qty;
    logic        op_side;
    logic        is_bid;

    always_comb begin
        do_remove = 1'b0;
        do_add    = 1'b0;
        rm_price  = 32'd0;
        rm_qty    = 32'd0;
        add_price = 32'd0;
        add_qty   = 32'd0;
        op_side   = `SIDE_BUY;

        if (sym_valid) begin
            case (msg_type)
                `MSG_ADD: begin
                    do_add    = 1'b1;
                    add_price = in_update.pillar_msg.price;
                    add_qty   = in_update.pillar_msg.qty;
                    op_side   = in_update.pillar_msg.side;
                end
                `MSG_MOD: begin
                    do_remove = 1'b1;
                    rm_price  = in_update.old_price;
                    rm_qty    = in_update.old_qty;
                    do_add    = 1'b1;
                    add_price = in_update.pillar_msg.price;
                    add_qty   = in_update.pillar_msg.qty;
                    op_side   = in_update.old_side;
                end
                `MSG_DEL: begin
                    do_remove = 1'b1;
                    rm_price  = in_update.old_price;
                    rm_qty    = in_update.old_qty;
                    op_side   = in_update.old_side;
                end
                `MSG_EXEC: begin
                    do_remove = 1'b1;
                    rm_price  = in_update.old_price;
                    rm_qty    = in_update.pillar_msg.qty; // exec qty from wire
                    op_side   = in_update.old_side;
                end
                `MSG_REPL: begin
                    do_remove = 1'b1;
                    rm_price  = in_update.old_price;
                    rm_qty    = in_update.old_qty;
                    do_add    = 1'b1;
                    add_price = in_update.pillar_msg.price;
                    add_qty   = in_update.pillar_msg.qty;
                    op_side   = in_update.old_side;
                end
                default: ;
            endcase
        end
    end

    assign is_bid = (op_side == `SIDE_BUY);

    // ----------------------------------------------------------------
    // Read current levels for the targeted symbol + side
    // ----------------------------------------------------------------
    price_level_t cur [N_LEVELS];

    always_comb begin
        for (int i = 0; i < N_LEVELS; i++)
            cur[i] = is_bid ? bid_book[sym][i] : ask_book[sym][i];
    end

    // ----------------------------------------------------------------
    // Phase 1: REMOVE
    //
    // Priority-scan for the first level matching rm_price.
    // rm_found cascades through the loop to ensure only the first
    // match acts (replaces the non-synthesizable 'break').
    // ----------------------------------------------------------------
    price_level_t after_rm [N_LEVELS];
    logic         rm_found;          // declared at module scope for VCS

    always_comb begin
        rm_found = 1'b0;
        for (int i = 0; i < N_LEVELS; i++)
            after_rm[i] = cur[i];

        if (do_remove) begin
            for (int i = 0; i < N_LEVELS; i++) begin
                if (!rm_found &&
                    cur[i].qty != 32'd0 &&
                    cur[i].price == rm_price) begin

                    rm_found = 1'b1;

                    if (cur[i].qty <= rm_qty) begin
                        // Level emptied — shift deeper levels up
                        for (int j = i; j < N_LEVELS - 1; j++)
                            after_rm[j] = cur[j + 1];
                        after_rm[N_LEVELS - 1].price = 32'd0;
                        after_rm[N_LEVELS - 1].qty   = 32'd0;
                    end else begin
                        // Partial — reduce qty only
                        after_rm[i].price = cur[i].price;
                        after_rm[i].qty   = cur[i].qty - rm_qty;
                    end
                end
            end
        end
    end

    // ----------------------------------------------------------------
    // Phase 2: ADD
    //
    // Step A: priority-scan for an existing level at add_price.
    //         If found, aggregate qty into that level.
    // Step B: if not found, priority-scan for the insertion point
    //         (first level that is empty or has a worse price).
    //         Insert there and shift deeper levels down by one.
    //         If add_price is worse than all N levels, discard.
    //
    // 'better_than[i]' is precomputed to avoid VCS issues with
    // function calls inside for-loop conditions.
    // ----------------------------------------------------------------
    price_level_t             after_add [N_LEVELS];
    logic                     add_match_found;    // module-scope
    logic                     add_inserted;        // module-scope
    logic [N_LEVELS-1:0]      better_than;        // per-level comparator

    // Precompute: is add_price better than each post-remove level?
    always_comb begin
        for (int i = 0; i < N_LEVELS; i++) begin
            if (after_rm[i].qty == 32'd0)
                better_than[i] = 1'b1;           // empty: anything wins
            else if (is_bid)
                better_than[i] = (add_price > after_rm[i].price);
            else
                better_than[i] = (add_price < after_rm[i].price);
        end
    end

    always_comb begin
        add_match_found = 1'b0;
        add_inserted    = 1'b0;

        for (int i = 0; i < N_LEVELS; i++)
            after_add[i] = after_rm[i];

        if (do_add && add_qty != 32'd0) begin

            // Step A: aggregate into existing level
            for (int i = 0; i < N_LEVELS; i++) begin
                if (!add_match_found &&
                    after_rm[i].qty != 32'd0 &&
                    after_rm[i].price == add_price) begin

                    add_match_found = 1'b1;
                    after_add[i].price = after_rm[i].price;
                    after_add[i].qty   = after_rm[i].qty + add_qty;
                end
            end

            // Step B: sorted insertion (only if no aggregate match)
            if (!add_match_found) begin
                for (int i = 0; i < N_LEVELS; i++) begin
                    if (!add_inserted && better_than[i]) begin
                        add_inserted = 1'b1;

                        // Shift levels [i..N-2] down to [i+1..N-1]
                        for (int j = N_LEVELS - 1; j > i; j--)
                            after_add[j] = after_rm[j - 1];

                        after_add[i].price = add_price;
                        after_add[i].qty   = add_qty;
                    end
                end
            end
        end
    end

    // ----------------------------------------------------------------
    // Write back — update register file on clock edge
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            for (int s = 0; s < N_SYMBOLS; s++)
                for (int l = 0; l < N_LEVELS; l++) begin
                    bid_book[s][l].price <= 32'd0;
                    bid_book[s][l].qty   <= 32'd0;
                    ask_book[s][l].price <= 32'd0;
                    ask_book[s][l].qty   <= 32'd0;
                end
        end else if (sym_valid) begin
            if (is_bid) begin
                for (int l = 0; l < N_LEVELS; l++)
                    bid_book[sym][l] <= after_add[l];
            end else begin
                for (int l = 0; l < N_LEVELS; l++)
                    ask_book[sym][l] <= after_add[l];
            end
        end
    end

    // ----------------------------------------------------------------
    // Output — BBO is level[0] of each side.
    // Uses after_add (combinational) for the side being modified,
    // and the registered value for the other side.
    // Pulses valid for one cycle per accepted update.
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            out_tob <= '0;
        end else begin
            out_tob.valid      <= sym_valid;
            out_tob.symbol_idx <= sym;

            if (sym_valid) begin
                if (is_bid) begin
                    out_tob.best_bid.price <= after_add[0].price;
                    out_tob.best_bid.qty   <= after_add[0].qty;
                    out_tob.best_ask.price <= ask_book[sym][0].price;
                    out_tob.best_ask.qty   <= ask_book[sym][0].qty;
                end else begin
                    out_tob.best_bid.price <= bid_book[sym][0].price;
                    out_tob.best_bid.qty   <= bid_book[sym][0].qty;
                    out_tob.best_ask.price <= after_add[0].price;
                    out_tob.best_ask.qty   <= after_add[0].qty;
                end
            end
        end
    end

endmodule