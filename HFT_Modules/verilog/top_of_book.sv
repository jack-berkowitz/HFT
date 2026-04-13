`include "sys_defs.svh"

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

    logic [31:0] bid_price [N_SYMBOLS][N_LEVELS];
    logic [31:0] bid_qty   [N_SYMBOLS][N_LEVELS];
    logic [31:0] ask_price [N_SYMBOLS][N_LEVELS];
    logic [31:0] ask_qty   [N_SYMBOLS][N_LEVELS];

    logic [SYM_W-1:0] sym;
    logic              sym_valid;
    logic [2:0]        msg_type;

    assign sym       = in_update.pillar_msg.symbol_index[SYM_W-1:0];
    assign sym_valid = in_update.valid &&
                       (in_update.pillar_msg.symbol_index < 32'(N_SYMBOLS));
    assign msg_type  = in_update.msg_type;

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
                    rm_qty    = in_update.pillar_msg.qty;
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

    logic [31:0] cur_price [N_LEVELS];
    logic [31:0] cur_qty   [N_LEVELS];

    always_comb begin
        for (int i = 0; i < N_LEVELS; i++) begin
            cur_price[i] = is_bid ? bid_price[sym][i] : ask_price[sym][i];
            cur_qty[i]   = is_bid ? bid_qty[sym][i]   : ask_qty[sym][i];
        end
    end

    logic [31:0] after_rm_price [N_LEVELS];
    logic [31:0] after_rm_qty   [N_LEVELS];
    logic         rm_found;

    always_comb begin
        rm_found = 1'b0;
        for (int i = 0; i < N_LEVELS; i++) begin
            after_rm_price[i] = cur_price[i];
            after_rm_qty[i]   = cur_qty[i];
        end

        if (do_remove) begin
            for (int i = 0; i < N_LEVELS; i++) begin
                if (!rm_found &&
                    cur_qty[i] != 32'd0 &&
                    cur_price[i] == rm_price) begin

                    rm_found = 1'b1;

                    if (cur_qty[i] <= rm_qty) begin
                        for (int j = i; j < N_LEVELS - 1; j++) begin
                            after_rm_price[j] = cur_price[j + 1];
                            after_rm_qty[j]   = cur_qty[j + 1];
                        end
                        after_rm_price[N_LEVELS - 1] = 32'd0;
                        after_rm_qty[N_LEVELS - 1]   = 32'd0;
                    end else begin
                        after_rm_price[i] = cur_price[i];
                        after_rm_qty[i]   = cur_qty[i] - rm_qty;
                    end
                end
            end
        end
    end

    logic [31:0]              after_add_price [N_LEVELS];
    logic [31:0]              after_add_qty   [N_LEVELS];
    logic                     add_match_found;
    logic                     add_inserted;
    logic [N_LEVELS-1:0]      better_than;

    always_comb begin
        for (int i = 0; i < N_LEVELS; i++) begin
            if (after_rm_qty[i] == 32'd0)
                better_than[i] = 1'b1;
            else if (is_bid)
                better_than[i] = (add_price > after_rm_price[i]);
            else
                better_than[i] = (add_price < after_rm_price[i]);
        end
    end

    always_comb begin
        add_match_found = 1'b0;
        add_inserted    = 1'b0;

        for (int i = 0; i < N_LEVELS; i++) begin
            after_add_price[i] = after_rm_price[i];
            after_add_qty[i]   = after_rm_qty[i];
        end

        if (do_add && add_qty != 32'd0) begin

            for (int i = 0; i < N_LEVELS; i++) begin
                if (!add_match_found &&
                    after_rm_qty[i] != 32'd0 &&
                    after_rm_price[i] == add_price) begin

                    add_match_found = 1'b1;
                    after_add_price[i] = after_rm_price[i];
                    after_add_qty[i]   = after_rm_qty[i] + add_qty;
                end
            end

            if (!add_match_found) begin
                for (int i = 0; i < N_LEVELS; i++) begin
                    if (!add_inserted && better_than[i]) begin
                        add_inserted = 1'b1;

                        for (int j = N_LEVELS - 1; j > i; j--) begin
                            after_add_price[j] = after_rm_price[j - 1];
                            after_add_qty[j]   = after_rm_qty[j - 1];
                        end

                        after_add_price[i] = add_price;
                        after_add_qty[i]   = add_qty;
                    end
                end
            end
        end
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            for (int s = 0; s < N_SYMBOLS; s++)
                for (int l = 0; l < N_LEVELS; l++) begin
                    bid_price[s][l] <= 32'd0;
                    bid_qty[s][l]   <= 32'd0;
                    ask_price[s][l] <= 32'd0;
                    ask_qty[s][l]   <= 32'd0;
                end
        end else if (sym_valid) begin
            if (is_bid) begin
                for (int l = 0; l < N_LEVELS; l++) begin
                    bid_price[sym][l] <= after_add_price[l];
                    bid_qty[sym][l]   <= after_add_qty[l];
                end
            end else begin
                for (int l = 0; l < N_LEVELS; l++) begin
                    ask_price[sym][l] <= after_add_price[l];
                    ask_qty[sym][l]   <= after_add_qty[l];
                end
            end
        end
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            out_tob <= '0;
        end else begin
            out_tob.valid      <= sym_valid;
            out_tob.symbol_idx <= sym;

            if (sym_valid) begin
                if (is_bid) begin
                    out_tob.best_bid.price <= after_add_price[0];
                    out_tob.best_bid.qty   <= after_add_qty[0];
                    out_tob.best_ask.price <= ask_price[sym][0];
                    out_tob.best_ask.qty   <= ask_qty[sym][0];
                end else begin
                    out_tob.best_bid.price <= bid_price[sym][0];
                    out_tob.best_bid.qty   <= bid_qty[sym][0];
                    out_tob.best_ask.price <= after_add_price[0];
                    out_tob.best_ask.qty   <= after_add_qty[0];
                end
            end
        end
    end

endmodule
