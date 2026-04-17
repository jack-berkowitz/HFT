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

    localparam SYM_W  = $clog2(N_SYMBOLS);
    localparam DATA_W = N_LEVELS * 64;

    // dp_ram ports ------------------------------------------------------
    logic [SYM_W-1:0]   bid_a_addr;
    logic [DATA_W-1:0]  bid_a_rdata;
    logic [SYM_W-1:0]   bid_b_addr;
    logic [DATA_W-1:0]  bid_b_wdata;
    logic                bid_b_wen;
    logic [DATA_W-1:0]  bid_b_rdata;

    logic [SYM_W-1:0]   ask_a_addr;
    logic [DATA_W-1:0]  ask_a_rdata;
    logic [SYM_W-1:0]   ask_b_addr;
    logic [DATA_W-1:0]  ask_b_wdata;
    logic                ask_b_wen;
    logic [DATA_W-1:0]  ask_b_rdata;

    dp_ram #(
        .ADDR_W (SYM_W),
        .DATA_W (DATA_W),
        .DEPTH  (N_SYMBOLS)
    ) bid_ram (
        .clk     (clk),
        .rst_n   (rst_n),
        .a_addr  (bid_a_addr),
        .a_rdata (bid_a_rdata),
        .b_addr  (bid_b_addr),
        .b_wdata (bid_b_wdata),
        .b_wen   (bid_b_wen),
        .b_rdata (bid_b_rdata)
    );

    dp_ram #(
        .ADDR_W (SYM_W),
        .DATA_W (DATA_W),
        .DEPTH  (N_SYMBOLS)
    ) ask_ram (
        .clk     (clk),
        .rst_n   (rst_n),
        .a_addr  (ask_a_addr),
        .a_rdata (ask_a_rdata),
        .b_addr  (ask_b_addr),
        .b_wdata (ask_b_wdata),
        .b_wen   (ask_b_wen),
        .b_rdata (ask_b_rdata)
    );

    // 4-stage FSM:
    //   S_CLEAR   - zero out all RAM entries on reset
    //   S_IDLE    - wait for valid update, issue RAM read
    //   S_REMOVE  - RAM data ready; compute after-remove; latch into regs
    //   S_ADD     - compute after-add from registered remove; write RAM;
    //               register output (cuts combinational depth in half)
    typedef enum logic [1:0] {
        S_CLEAR,
        S_IDLE,
        S_REMOVE,
        S_ADD
    } state_t;

    state_t            state_r;
    logic [SYM_W-1:0] clear_addr_r;

    // Decode --------------------------------------------------------------
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

    // Stage-1 pipeline registers (IDLE -> REMOVE) -----------------------
    logic [SYM_W-1:0] sym_r;
    logic              sym_valid_r;
    logic              do_add_r;
    logic [31:0]       add_price_r, add_qty_r;
    logic              is_bid_r;

    // Stage-2 pipeline registers (REMOVE -> ADD) ------------------------
    // Hold the removed book for the active side and the unchanged-side
    // snapshot read from RAM.
    logic [DATA_W-1:0] active_rdata;
    assign active_rdata = is_bid_r ? bid_a_rdata : ask_a_rdata;

    logic [31:0] cur_price [N_LEVELS];
    logic [31:0] cur_qty   [N_LEVELS];

    always_comb begin
        for (int i = 0; i < N_LEVELS; i++) begin
            cur_price[i] = active_rdata[i*64+32 +: 32];
            cur_qty[i]   = active_rdata[i*64 +: 32];
        end
    end

    // Remove one share/level combinationally (consumed in S_REMOVE).
    logic [31:0] after_rm_price [N_LEVELS];
    logic [31:0] after_rm_qty   [N_LEVELS];
    logic         rm_found;

    logic        do_remove_r;
    logic [31:0] rm_price_r, rm_qty_r;

    always_comb begin
        rm_found = 1'b0;
        for (int i = 0; i < N_LEVELS; i++) begin
            after_rm_price[i] = cur_price[i];
            after_rm_qty[i]   = cur_qty[i];
        end

        if (do_remove_r) begin
            for (int i = 0; i < N_LEVELS; i++) begin
                if (!rm_found &&
                    cur_qty[i] != 32'd0 &&
                    cur_price[i] == rm_price_r) begin

                    rm_found = 1'b1;

                    if (cur_qty[i] <= rm_qty_r) begin
                        for (int j = i; j < N_LEVELS - 1; j++) begin
                            after_rm_price[j] = cur_price[j + 1];
                            after_rm_qty[j]   = cur_qty[j + 1];
                        end
                        after_rm_price[N_LEVELS - 1] = 32'd0;
                        after_rm_qty[N_LEVELS - 1]   = 32'd0;
                    end else begin
                        after_rm_price[i] = cur_price[i];
                        after_rm_qty[i]   = cur_qty[i] - rm_qty_r;
                    end
                end
            end
        end
    end

    // Registers carrying remove result and the unchanged-side snapshot
    // forward into S_ADD.
    logic [31:0]       after_rm_price_r [N_LEVELS];
    logic [31:0]       after_rm_qty_r   [N_LEVELS];
    logic [DATA_W-1:0] other_side_r;

    // Combinational add using registered remove result -----------------
    logic [31:0]              after_add_price [N_LEVELS];
    logic [31:0]              after_add_qty   [N_LEVELS];
    logic                     add_match_found;
    logic                     add_inserted;
    logic [N_LEVELS-1:0]      better_than;

    always_comb begin
        for (int i = 0; i < N_LEVELS; i++) begin
            if (after_rm_qty_r[i] == 32'd0)
                better_than[i] = 1'b1;
            else if (is_bid_r)
                better_than[i] = (add_price_r > after_rm_price_r[i]);
            else
                better_than[i] = (add_price_r < after_rm_price_r[i]);
        end
    end

    always_comb begin
        add_match_found = 1'b0;
        add_inserted    = 1'b0;

        for (int i = 0; i < N_LEVELS; i++) begin
            after_add_price[i] = after_rm_price_r[i];
            after_add_qty[i]   = after_rm_qty_r[i];
        end

        if (do_add_r && add_qty_r != 32'd0) begin

            for (int i = 0; i < N_LEVELS; i++) begin
                if (!add_match_found &&
                    after_rm_qty_r[i] != 32'd0 &&
                    after_rm_price_r[i] == add_price_r) begin

                    add_match_found = 1'b1;
                    after_add_price[i] = after_rm_price_r[i];
                    after_add_qty[i]   = after_rm_qty_r[i] + add_qty_r;
                end
            end

            if (!add_match_found) begin
                for (int i = 0; i < N_LEVELS; i++) begin
                    if (!add_inserted && better_than[i]) begin
                        add_inserted = 1'b1;

                        for (int j = N_LEVELS - 1; j > i; j--) begin
                            after_add_price[j] = after_rm_price_r[j - 1];
                            after_add_qty[j]   = after_rm_qty_r[j - 1];
                        end

                        after_add_price[i] = add_price_r;
                        after_add_qty[i]   = add_qty_r;
                    end
                end
            end
        end
    end

    logic [DATA_W-1:0] new_data;
    always_comb begin
        new_data = '0;
        for (int i = 0; i < N_LEVELS; i++) begin
            new_data[i*64+32 +: 32] = after_add_price[i];
            new_data[i*64 +: 32]    = after_add_qty[i];
        end
    end

    // RAM port drivers --------------------------------------------------
    always_comb begin
        bid_a_addr  = '0;
        ask_a_addr  = '0;
        bid_b_addr  = '0;
        ask_b_addr  = '0;
        bid_b_wdata = '0;
        ask_b_wdata = '0;
        bid_b_wen   = 1'b0;
        ask_b_wen   = 1'b0;

        case (state_r)
            S_CLEAR: begin
                bid_b_addr  = clear_addr_r;
                ask_b_addr  = clear_addr_r;
                bid_b_wdata = '0;
                ask_b_wdata = '0;
                bid_b_wen   = 1'b1;
                ask_b_wen   = 1'b1;
            end

            S_IDLE: begin
                bid_a_addr = sym;
                ask_a_addr = sym;
            end

            // Hold read address on sym_r so a_rdata stays stable through
            // both remove and add phases.
            S_REMOVE: begin
                bid_a_addr = sym_r;
                ask_a_addr = sym_r;
            end

            S_ADD: begin
                bid_a_addr = sym_r;
                ask_a_addr = sym_r;
                if (sym_valid_r) begin
                    if (is_bid_r) begin
                        bid_b_addr  = sym_r;
                        bid_b_wdata = new_data;
                        bid_b_wen   = 1'b1;
                    end else begin
                        ask_b_addr  = sym_r;
                        ask_b_wdata = new_data;
                        ask_b_wen   = 1'b1;
                    end
                end
            end

            default: ;
        endcase
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            state_r      <= S_CLEAR;
            clear_addr_r <= '0;
            sym_r        <= '0;
            sym_valid_r  <= 1'b0;
            do_remove_r  <= 1'b0;
            do_add_r     <= 1'b0;
            rm_price_r   <= '0;
            rm_qty_r     <= '0;
            add_price_r  <= '0;
            add_qty_r    <= '0;
            is_bid_r     <= 1'b0;
            other_side_r <= '0;
            out_tob      <= '0;
            for (int i = 0; i < N_LEVELS; i++) begin
                after_rm_price_r[i] <= 32'd0;
                after_rm_qty_r[i]   <= 32'd0;
            end
        end else begin
            case (state_r)
                S_CLEAR: begin
                    if (clear_addr_r == SYM_W'(N_SYMBOLS - 1))
                        state_r <= S_IDLE;
                    else
                        clear_addr_r <= clear_addr_r + 1'b1;
                end

                S_IDLE: begin
                    out_tob.valid <= 1'b0;

                    if (sym_valid) begin
                        sym_r       <= sym;
                        sym_valid_r <= 1'b1;
                        do_remove_r <= do_remove;
                        do_add_r    <= do_add;
                        rm_price_r  <= rm_price;
                        rm_qty_r    <= rm_qty;
                        add_price_r <= add_price;
                        add_qty_r   <= add_qty;
                        is_bid_r    <= is_bid;
                        state_r     <= S_REMOVE;
                    end
                end

                // Latch the after-remove result and the unchanged side's
                // current book for use in S_ADD.
                S_REMOVE: begin
                    for (int i = 0; i < N_LEVELS; i++) begin
                        after_rm_price_r[i] <= after_rm_price[i];
                        after_rm_qty_r[i]   <= after_rm_qty[i];
                    end
                    other_side_r <= is_bid_r ? ask_a_rdata : bid_a_rdata;
                    state_r      <= S_ADD;
                end

                S_ADD: begin
                    out_tob.valid      <= sym_valid_r;
                    out_tob.symbol_idx <= sym_r;

                    if (sym_valid_r) begin
                        if (is_bid_r) begin
                            out_tob.best_bid.price <= after_add_price[0];
                            out_tob.best_bid.qty   <= after_add_qty[0];
                            out_tob.best_ask.price <= other_side_r[32 +: 32];
                            out_tob.best_ask.qty   <= other_side_r[0 +: 32];
                        end else begin
                            out_tob.best_bid.price <= other_side_r[32 +: 32];
                            out_tob.best_bid.qty   <= other_side_r[0 +: 32];
                            out_tob.best_ask.price <= after_add_price[0];
                            out_tob.best_ask.qty   <= after_add_qty[0];
                        end
                    end

                    state_r <= S_IDLE;
                end

                default: state_r <= S_IDLE;
            endcase
        end
    end

endmodule
