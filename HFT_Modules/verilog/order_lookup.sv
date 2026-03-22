// ============================================================================
// order_lookup.sv
//
// Order hash-table controller for Pillar message types 0-4.
//
// Assumes `include "sys_defs.svh" provides:
//   pillar_msg_t, order_lookup_out_t, hash_entry_t
//   `XDP_MSG_ADD_ORDER_COMP  .. `XDP_MSG_REPLACE_COMP
//
// Instantiates a skewed_hash_table with:
//   Key   = order_id           (64 bits)
//   Value = {price, qty, side} (65 bits packed)
//
// Message handling:
//   Add     (0) : Insert order_id -> {price, qty, side}.  No prior lookup.
//   Modify  (1) : Lookup order_id; overwrite price/qty, preserve side.
//   Delete  (2) : Lookup order_id; remove entry.
//   Exec    (3) : Lookup order_id; subtract executed qty.
//                  qty == 0 -> remove; else update in place.
//   Replace (4) : Lookup old order_id; delete old; insert new_order_id
//                  with new price/qty and old side.
//
// ============================================================================

`include "sys_defs.svh"

module order_lookup #(
    parameter N_WAYS      = 4,
    parameter TBL_ENTRIES = 4096,
    parameter MAX_CHAIN   = 16
) (
    input  logic              clk,
    input  logic              rst_n,

    output logic              ready,

    input  pillar_msg_t       msg_in,

    output order_lookup_out_t out
);

    // =================================================================
    // Hash table dimensioning
    // =================================================================
    localparam KEY_W   = 64;              // order_id
    localparam VALUE_W = 32 + 32 + 1;     // price + qty + side = 65

    // Value bit-field positions
    localparam P_HI  = 64, P_LO = 33;    // price [64:33]
    localparam Q_HI  = 32, Q_LO =  1;    // qty   [32:1]
    localparam S_BIT = 0;                 // side  [0]

    // =================================================================
    // Value packing helper
    // =================================================================
    function automatic [VALUE_W-1:0] pack_val(
        input [31:0] price,
        input [31:0] qty,
        input        side
    );
        return {price, qty, side};
    endfunction

    // =================================================================
    // Hash table instance
    // =================================================================
    logic                  ht_ready;

    logic                  ht_lu_valid;
    logic [KEY_W-1:0]      ht_lu_key;
    logic                  ht_lu_result_valid;
    logic                  ht_lu_hit;
    logic [VALUE_W-1:0]    ht_lu_value;

    logic                  ht_ins_valid;
    logic [KEY_W-1:0]      ht_ins_key;
    logic [VALUE_W-1:0]    ht_ins_value;
    logic                  ht_ins_ready;
    logic                  ht_ins_done;
    logic                  ht_ins_fail;

    logic                  ht_del_valid;
    logic [KEY_W-1:0]      ht_del_key;
    logic                  ht_del_ready;
    logic                  ht_del_done;
    logic                  ht_del_not_found;

    skewed_hash_table #(
        .KEY_W       (KEY_W),
        .VALUE_W     (VALUE_W),
        .N_WAYS      (N_WAYS),
        .TBL_ENTRIES (TBL_ENTRIES),
        .MAX_CHAIN   (MAX_CHAIN)
    ) u_order_table (
        .clk                 (clk),
        .rst_n               (rst_n),
        .ready               (ht_ready),

        .lookup_valid        (ht_lu_valid),
        .lookup_key          (ht_lu_key),
        .lookup_result_valid (ht_lu_result_valid),
        .lookup_hit          (ht_lu_hit),
        .lookup_value        (ht_lu_value),

        .insert_valid        (ht_ins_valid),
        .insert_key          (ht_ins_key),
        .insert_value        (ht_ins_value),
        .insert_ready        (ht_ins_ready),
        .insert_done         (ht_ins_done),
        .insert_fail         (ht_ins_fail),

        .delete_valid        (ht_del_valid),
        .delete_key          (ht_del_key),
        .delete_ready        (ht_del_ready),
        .delete_done         (ht_del_done),
        .delete_not_found    (ht_del_not_found)
    );

    // =================================================================
    // Exec remaining-qty computation (combinational)
    // =================================================================
    logic [31:0] exec_remaining;

    always_comb begin
        if (ht_lu_value[Q_HI:Q_LO] > msg_r.qty)
            exec_remaining = ht_lu_value[Q_HI:Q_LO] - msg_r.qty;
        else
            exec_remaining = 32'd0;
    end

    // =================================================================
    // Controller FSM
    // =================================================================
    typedef enum logic [3:0] {
        ST_IDLE,
        ST_LOOKUP,
        ST_LU_PIPE,
        ST_LU_LATCH,
        ST_INSERT,
        ST_DELETE,
        ST_WAIT_OP,
        ST_EMIT
    } state_t;

    state_t              state_r;

    pillar_msg_t         msg_r;

    logic                lu_hit_r;
    logic [31:0]         lu_price_r;
    logic [31:0]         lu_qty_r;
    logic                lu_side_r;

    logic [KEY_W-1:0]    wr_key_r;
    logic [VALUE_W-1:0]  wr_val_r;

    logic                repl_pending_r;

    // -----------------------------------------------------------------
    // Hash-table signal drive (combinational)
    // -----------------------------------------------------------------
    always_comb begin
        ht_lu_valid  = 1'b0;
        ht_lu_key    = '0;
        ht_ins_valid = 1'b0;
        ht_ins_key   = '0;
        ht_ins_value = '0;
        ht_del_valid = 1'b0;
        ht_del_key   = '0;

        case (state_r)
            ST_LOOKUP: begin
                ht_lu_valid = 1'b1;
                ht_lu_key   = msg_r.order_id;
            end

            ST_INSERT: begin
                ht_ins_valid = ht_ins_ready;
                ht_ins_key   = wr_key_r;
                ht_ins_value = wr_val_r;
            end

            ST_DELETE: begin
                ht_del_valid = ht_del_ready;
                ht_del_key   = wr_key_r;
            end

            default: ;
        endcase
    end

    // -----------------------------------------------------------------
    // State machine (sequential)
    // -----------------------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state_r        <= ST_IDLE;
            msg_r          <= '0;
            lu_hit_r       <= 1'b0;
            lu_price_r     <= '0;
            lu_qty_r       <= '0;
            lu_side_r      <= 1'b0;
            wr_key_r       <= '0;
            wr_val_r       <= '0;
            repl_pending_r <= 1'b0;
            out            <= '0;
        end else begin
            out.valid <= 1'b0;

            case (state_r)
                // =====================================================
                ST_IDLE: begin
                    if (msg_in.valid && ht_ready) begin
                        msg_r          <= msg_in;
                        repl_pending_r <= 1'b0;

                        if (msg_in.msg_type == `XDP_MSG_ADD_ORDER_COMP) begin
                            wr_key_r   <= msg_in.order_id;
                            wr_val_r   <= pack_val(msg_in.price,
                                                   msg_in.qty,
                                                   msg_in.side);
                            lu_hit_r   <= 1'b0;
                            lu_price_r <= '0;
                            lu_qty_r   <= '0;
                            lu_side_r  <= 1'b0;
                            state_r    <= ST_INSERT;
                        end else begin
                            state_r <= ST_LOOKUP;
                        end
                    end
                end

                // =====================================================
                ST_LOOKUP: begin
                    state_r <= ST_LU_PIPE;
                end

                // =====================================================
                ST_LU_PIPE: begin
                    state_r <= ST_LU_LATCH;
                end

                // =====================================================
                ST_LU_LATCH: begin
                    lu_hit_r   <= ht_lu_hit;
                    lu_price_r <= ht_lu_hit ? ht_lu_value[P_HI:P_LO] : '0;
                    lu_qty_r   <= ht_lu_hit ? ht_lu_value[Q_HI:Q_LO] : '0;
                    lu_side_r  <= ht_lu_hit ? ht_lu_value[S_BIT]      : 1'b0;

                    case (msg_r.msg_type)
                        // -----------------------------------------
                        `XDP_MSG_MOD_ORDER_COMP: begin
                            if (ht_lu_hit) begin
                                wr_key_r <= msg_r.order_id;
                                wr_val_r <= pack_val(
                                    msg_r.price,
                                    msg_r.qty,
                                    ht_lu_value[S_BIT]
                                );
                                state_r <= ST_INSERT;
                            end else begin
                                state_r <= ST_EMIT;
                            end
                        end

                        // -----------------------------------------
                        `XDP_MSG_DEL_ORDER_COMP: begin
                            if (ht_lu_hit) begin
                                wr_key_r <= msg_r.order_id;
                                state_r  <= ST_DELETE;
                            end else begin
                                state_r <= ST_EMIT;
                            end
                        end

                        // -----------------------------------------
                        `XDP_MSG_EXEC_ORDER_COMP: begin
                            if (ht_lu_hit) begin
                                if (exec_remaining == 32'd0) begin
                                    wr_key_r <= msg_r.order_id;
                                    state_r  <= ST_DELETE;
                                end else begin
                                    wr_key_r <= msg_r.order_id;
                                    wr_val_r <= pack_val(
                                        ht_lu_value[P_HI:P_LO],
                                        exec_remaining,
                                        ht_lu_value[S_BIT]
                                    );
                                    state_r <= ST_INSERT;
                                end
                            end else begin
                                state_r <= ST_EMIT;
                            end
                        end

                        // -----------------------------------------
                        `XDP_MSG_REPLACE_COMP: begin
                            if (ht_lu_hit) begin
                                wr_key_r       <= msg_r.order_id;
                                repl_pending_r <= 1'b1;
                                state_r        <= ST_DELETE;
                            end else begin
                                wr_key_r <= msg_r.new_order_id;
                                wr_val_r <= pack_val(
                                    msg_r.price,
                                    msg_r.qty,
                                    1'b0
                                );
                                state_r <= ST_INSERT;
                            end
                        end

                        // -----------------------------------------
                        default: state_r <= ST_EMIT;
                    endcase
                end

                // =====================================================
                ST_INSERT: begin
                    if (ht_ins_ready)
                        state_r <= ST_WAIT_OP;
                end

                // =====================================================
                ST_DELETE: begin
                    if (ht_del_ready)
                        state_r <= ST_WAIT_OP;
                end

                // =====================================================
                ST_WAIT_OP: begin
                    if (ht_ins_done || ht_ins_fail ||
                        ht_del_done || ht_del_not_found) begin

                        if (repl_pending_r) begin
                            wr_key_r       <= msg_r.new_order_id;
                            wr_val_r       <= pack_val(
                                msg_r.price,
                                msg_r.qty,
                                lu_side_r
                            );
                            repl_pending_r <= 1'b0;
                            state_r        <= ST_INSERT;
                        end else begin
                            state_r <= ST_EMIT;
                        end
                    end
                end

                // =====================================================
                ST_EMIT: begin
                    out.valid       <= 1'b1;
                    out.pillar_msg  <= msg_r;
                    out.symbol_idx  <= msg_r.symbol_index;
                    out.old_price   <= lu_price_r;
                    out.old_qty     <= lu_qty_r;
                    out.old_side    <= lu_side_r;
                    out.msg_type    <= msg_r.msg_type;
                    state_r         <= ST_IDLE;
                end

                // =====================================================
                default: state_r <= ST_IDLE;
            endcase
        end
    end

    // =================================================================
    assign ready = (state_r == ST_IDLE) && ht_ready;

endmodule