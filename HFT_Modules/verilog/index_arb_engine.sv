`include "sys_defs.svh"

module index_arb_engine #(
    parameter N_COMPONENTS = 500,
    parameter IDX_SYM      = `IDX_SYMBOL
)(
    input  logic                       clk,
    input  logic                       rst_n,

    input  tob_out_t                   in_tob,

    input  logic                       wt_wr_en,
    input  logic [`SYM_IDX_W-1:0]     wt_wr_addr,
    input  logic [`WEIGHT_W-1:0]      wt_wr_data,

    input  logic [`ACCUM_W-1:0]       threshold,

    output trade_signal_t              out_trade,

    output logic [`ACCUM_W-1:0]       out_index
);

    localparam FRAC = `WEIGHT_FRAC_BITS;
    localparam AW   = `ACCUM_W;
    localparam WW   = `WEIGHT_W;
    localparam SYMW = `SYM_IDX_W;

    localparam MUL_STAGES = 3;
    localparam SHIFT      = 11;

    logic [WW-1:0]  weight_tbl [N_COMPONENTS];

    logic [31:0]    old_mid    [N_COMPONENTS];

    logic signed [AW-1:0] index_accum;

    logic [31:0]          actual_mid;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            for (int i = 0; i < N_COMPONENTS; i++)
                weight_tbl[i] <= '0;
        end else if (wt_wr_en && wt_wr_addr < SYMW'(N_COMPONENTS)) begin
            weight_tbl[wt_wr_addr] <= wt_wr_data;
        end
    end

    logic               s0_valid;
    logic               s0_is_index;
    logic               s0_is_component;
    logic [SYMW-1:0]    s0_sym;
    logic [31:0]        s0_new_mid;
    logic signed [32:0] s0_delta;
    logic [WW-1:0]      s0_weight;
    logic               s0_two_sided;

    always_comb begin
        s0_sym        = in_tob.symbol_idx;
        s0_new_mid    = (in_tob.best_bid.price + in_tob.best_ask.price) >> 1;
        s0_two_sided  = (in_tob.best_bid.qty != 32'd0) &&
                        (in_tob.best_ask.qty != 32'd0);
        s0_is_index   = (in_tob.symbol_idx == IDX_SYM[SYMW-1:0]);
        s0_is_component = in_tob.valid && !s0_is_index &&
                          s0_two_sided &&
                          (in_tob.symbol_idx < SYMW'(N_COMPONENTS));
        s0_delta      = $signed({1'b0, s0_new_mid}) -
                        $signed({1'b0, old_mid[s0_sym]});
        s0_weight     = weight_tbl[s0_sym];
        s0_valid      = in_tob.valid;
    end

    logic               s1_valid;
    logic               s1_is_index;
    logic               s1_is_component;
    logic [31:0]        s1_new_mid;

    logic [AW-1:0]      s1_mcand;
    logic [AW-1:0]      s1_mplier;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            s1_valid        <= 1'b0;
            s1_is_index     <= 1'b0;
            s1_is_component <= 1'b0;
            s1_new_mid      <= '0;
            s1_mcand        <= '0;
            s1_mplier       <= '0;
        end else begin
            s1_valid        <= s0_valid;
            s1_is_index     <= s0_is_index && in_tob.valid && s0_two_sided;
            s1_is_component <= s0_is_component;
            s1_new_mid      <= s0_new_mid;
            s1_mcand        <= {{(AW-33){s0_delta[32]}}, s0_delta};
            s1_mplier       <= {{(AW-33){1'b0}}, 1'b0, s0_weight};
        end
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            for (int i = 0; i < N_COMPONENTS; i++)
                old_mid[i] <= 32'd0;
        end else if (s0_is_component) begin
            old_mid[s0_sym] <= s0_new_mid;
        end
    end

    logic [AW-1:0] pp1;
    assign pp1 = s1_mplier[SHIFT-1:0] * s1_mcand;

    logic               mp1_valid;
    logic               mp1_is_index;
    logic               mp1_is_component;
    logic [31:0]        mp1_new_mid;
    logic [AW-1:0]      mp1_sum;
    logic [AW-1:0]      mp1_mcand;
    logic [AW-1:0]      mp1_mplier;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            mp1_valid        <= 1'b0;
            mp1_is_index     <= 1'b0;
            mp1_is_component <= 1'b0;
            mp1_new_mid      <= '0;
            mp1_sum          <= '0;
            mp1_mcand        <= '0;
            mp1_mplier       <= '0;
        end else begin
            mp1_valid        <= s1_valid;
            mp1_is_index     <= s1_is_index;
            mp1_is_component <= s1_is_component;
            mp1_new_mid      <= s1_new_mid;
            mp1_sum          <= pp1;
            mp1_mcand        <= {s1_mcand[AW-1-SHIFT:0], {SHIFT{1'b0}}};
            mp1_mplier       <= {{SHIFT{1'b0}}, s1_mplier[AW-1:SHIFT]};
        end
    end

    logic [AW-1:0] pp2;
    assign pp2 = mp1_mplier[SHIFT-1:0] * mp1_mcand;

    logic               mp2_valid;
    logic               mp2_is_index;
    logic               mp2_is_component;
    logic [31:0]        mp2_new_mid;
    logic [AW-1:0]      mp2_sum;
    logic [AW-1:0]      mp2_mcand;
    logic [AW-1:0]      mp2_mplier;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            mp2_valid        <= 1'b0;
            mp2_is_index     <= 1'b0;
            mp2_is_component <= 1'b0;
            mp2_new_mid      <= '0;
            mp2_sum          <= '0;
            mp2_mcand        <= '0;
            mp2_mplier       <= '0;
        end else begin
            mp2_valid        <= mp1_valid;
            mp2_is_index     <= mp1_is_index;
            mp2_is_component <= mp1_is_component;
            mp2_new_mid      <= mp1_new_mid;
            mp2_sum          <= mp1_sum + pp2;
            mp2_mcand        <= {mp1_mcand[AW-1-SHIFT:0], {SHIFT{1'b0}}};
            mp2_mplier       <= {{SHIFT{1'b0}}, mp1_mplier[AW-1:SHIFT]};
        end
    end

    logic [AW-1:0] pp3;
    assign pp3 = mp2_mplier[SHIFT-1:0] * mp2_mcand;

    logic               mp3_valid;
    logic               mp3_is_index;
    logic               mp3_is_component;
    logic [31:0]        mp3_new_mid;
    logic [AW-1:0]      mp3_product;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            mp3_valid        <= 1'b0;
            mp3_is_index     <= 1'b0;
            mp3_is_component <= 1'b0;
            mp3_new_mid      <= '0;
            mp3_product      <= '0;
        end else begin
            mp3_valid        <= mp2_valid;
            mp3_is_index     <= mp2_is_index;
            mp3_is_component <= mp2_is_component;
            mp3_new_mid      <= mp2_new_mid;
            mp3_product      <= mp2_sum + pp3;
        end
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            index_accum <= '0;
            actual_mid  <= '0;
        end else begin
            if (mp3_is_component)
                index_accum <= index_accum + $signed(mp3_product);

            if (mp3_is_index)
                actual_mid <= mp3_new_mid;
        end
    end

    logic signed [AW-1:0] next_accum;
    logic        [31:0]   next_actual;
    logic signed [AW-1:0] actual_q;
    logic signed [AW-1:0] spread;
    logic        [AW-1:0] abs_spread;
    logic                  trade_fire;
    logic                  trade_dir;

    always_comb begin
        next_accum  = mp3_is_component ? (index_accum + $signed(mp3_product))
                                       : index_accum;
        next_actual = mp3_is_index     ? mp3_new_mid : actual_mid;

        actual_q   = AW'(next_actual) <<< FRAC;
        spread     = next_accum - actual_q;
        abs_spread = spread[AW-1] ? AW'(-spread) : AW'(spread);
        trade_fire = (abs_spread > threshold) && (next_actual != 32'd0);
        trade_dir  = ~spread[AW-1];
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            out_trade <= '0;
        end else begin
            out_trade.valid          <= mp3_valid && trade_fire;
            out_trade.direction      <= trade_dir;
            out_trade.spread         <= spread;
            out_trade.computed_index <= next_accum;
            out_trade.actual_price   <= next_actual;
        end
    end

    assign out_index = index_accum;

endmodule
