`include "sys_defs.svh"
// ============================================================
// index_arb_engine.sv
//
// Computes a market-cap-weighted "fair value" of the S&P 500
// index from the 500 component stocks' live BBO mid-prices,
// compares it to the actual traded index price (SPY/ES at
// symbol IDX_SYMBOL = 500), and generates a trade signal
// when the spread exceeds a configurable threshold.
//
// --- How it works ---
//
// The index value is:
//   index = SUM over i in [0,499]  of  (mid_price_i * weight_i)
//
// where weight_i is each stock's market-cap weight in Q12.20
// fixed-point (all weights sum to 1.0 = 1<<20).
//
// Rather than recomputing the full sum on every tick, we do it
// INCREMENTALLY:
//   delta_i   = new_mid_i - old_mid_i
//   index    += delta_i * weight_i
//
// This is O(1) per update — one subtract, one multiply, one add.
//
// The spread (basis) is:
//   spread = computed_index - (actual_index_mid << FRAC_BITS)
//
// A trade signal fires when |spread| > threshold.
//   spread > 0  → computed > actual → SELL the index (overpriced)
//   spread < 0  → computed < actual → BUY  the index (underpriced)
//
// --- Interfaces ---
//
// in_tob:      tob_out_t from top_of_book (one per cycle max)
// wt_wr_*:     weight load port (write once at startup per symbol)
// threshold:   spread threshold in Q44.20 (runtime-configurable)
// out_trade:   trade_signal_t output
// out_index:   current computed index value (for monitoring)
//
// --- Timing ---
//
// Pipelined: 5-cycle latency from tob input to trade output.
//   Stage 0 (DECODE):  compute mid, read old_mid + weight, delta
//   Stage 1 (MUL_P1):  partial product — mplier[10:0] × mcand
//   Stage 2 (MUL_P2):  partial product — mplier[21:11] × mcand
//   Stage 3 (MUL_P3):  partial product — mplier[32:22] × mcand
//   Stage 4 (ACC):     accumulate, spread compare, trade output
//
// The 33-bit × 33-bit multiply is decomposed into three 11-bit ×
// 64-bit partial-product stages using shift-and-add.  Each stage
// processes SHIFT=11 bits of the multiplier (weight), shifts the
// multiplicand (delta) left by 11, and accumulates the running
// partial product sum.  This keeps each stage's combinational
// multiply small (11×64) and ASIC-friendly for Synopsys DC
// at 250 MHz without relying on retiming.
//
// The low 64 bits of the unsigned partial-product sum equal the
// correct signed Q44.20 result because 2's-complement truncation
// is modular: sign-extending delta to 64 bits and zero-extending
// weight to 64 bits, then keeping the low 64 bits of the unsigned
// product, gives the same bit pattern as a signed multiply.
// ============================================================
module index_arb_engine #(
    parameter N_COMPONENTS = 500,        // number of index components
    parameter IDX_SYM      = `IDX_SYMBOL // symbol slot for actual index
)(
    input  logic                       clk,
    input  logic                       rst_n,

    // BBO updates from top_of_book
    input  tob_out_t                   in_tob,

    // Weight load port (written once at startup by config logic)
    input  logic                       wt_wr_en,
    input  logic [`SYM_IDX_W-1:0]     wt_wr_addr,
    input  logic [`WEIGHT_W-1:0]      wt_wr_data,

    // Spread threshold (unsigned, in Q44.20 fixed-point)
    input  logic [`ACCUM_W-1:0]       threshold,

    output logic                       ready,

    // Trade signal output
    output trade_signal_t              out_trade,

    // Index value monitor port (active every cycle)
    output logic [`ACCUM_W-1:0]       out_index
);

    localparam FRAC = `WEIGHT_FRAC_BITS;   // 20
    localparam AW   = `ACCUM_W;            // 64
    localparam WW   = `WEIGHT_W;           // 32
    localparam SYMW = `SYM_IDX_W;          // 9

    // Partial-product multiply parameters
    // delta is signed 33-bit, weight is unsigned 32-bit (33-bit with leading 0)
    // 33 multiplier bits / 3 stages = 11 bits per stage
    localparam MUL_STAGES = 3;
    localparam SHIFT      = 11;

    // ----------------------------------------------------------------
    // Storage
    // ----------------------------------------------------------------

    // Per-component weight table (Q12.20, written at startup)
    logic [WW-1:0]  weight_tbl [N_COMPONENTS];

    // Per-component last-known mid-price (32-bit integer, cents)
    logic [31:0]    old_mid    [N_COMPONENTS];

    // Running weighted index accumulator (Q44.20 signed)
    logic signed [AW-1:0] index_accum;

    // Tracked actual index instrument mid-price
    logic [31:0]          actual_mid;

`ifdef SYNTH
    logic [SYMW-1:0]      clear_mid_r;
`endif

    // ----------------------------------------------------------------
    // Weight load port
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
`ifndef SYNTH
        if (!rst_n) begin
            for (int i = 0; i < N_COMPONENTS; i++)
                weight_tbl[i] <= '0;
        end else if (wt_wr_en && wt_wr_addr < SYMW'(N_COMPONENTS)) begin
            weight_tbl[wt_wr_addr] <= wt_wr_data;
        end
`else
        if (wt_wr_en && wt_wr_addr < SYMW'(N_COMPONENTS)) begin
            weight_tbl[wt_wr_addr] <= wt_wr_data;
        end
`endif
    end

    // ----------------------------------------------------------------
    // Stage 0 (DECODE): compute mid-price, delta, read weight
    //
    // Mid-price = (bid + ask) / 2
    // Only update if both bid and ask are nonzero (two-sided market).
    // If one side is empty, skip the update — old_mid holds the
    // last valid price and the index stays consistent.
    // ----------------------------------------------------------------
    logic               s0_valid;
    logic               s0_is_index;
    logic               s0_is_component;
    logic               s0_in_component_range;
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
        s0_in_component_range = (in_tob.symbol_idx < SYMW'(N_COMPONENTS));
        s0_is_component = ready && in_tob.valid && !s0_is_index &&
                          s0_two_sided && s0_in_component_range;
        s0_delta      = '0;
        s0_weight     = '0;
        if (s0_in_component_range) begin
            s0_delta  = $signed({1'b0, s0_new_mid}) -
                        $signed({1'b0, old_mid[s0_sym]});
            s0_weight = weight_tbl[s0_sym];
        end
        s0_valid      = ready && in_tob.valid;
    end

    // Pipeline register: Stage 0 → Stage 1
    logic               s1_valid;
    logic               s1_is_index;
    logic               s1_is_component;
    logic [31:0]        s1_new_mid;

    // Multiply operands (sign-extended / zero-extended to AW bits)
    // These become the initial mcand and mplier for the partial-product chain.
    logic [AW-1:0]      s1_mcand;     // sign-extended delta (multiplicand)
    logic [AW-1:0]      s1_mplier;    // zero-extended weight (multiplier)

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
            // Sign-extend 33-bit signed delta to 64 bits
            s1_mcand        <= {{(AW-33){s0_delta[32]}}, s0_delta};
            // Zero-extend {1'b0, weight} = 33-bit unsigned to 64 bits
            s1_mplier       <= {{(AW-33){1'b0}}, 1'b0, s0_weight};
        end
    end

    // ----------------------------------------------------------------
    // Stages 1-3: 3-stage partial-product multiply pipeline
    //
    // Decomposes the 33×33 multiply into three 11×64 partial products
    // using shift-and-add:
    //
    //   Stage 1: pp = mplier[10:0]  × mcand;  sum  = 0 + pp
    //   Stage 2: pp = mplier[10:0]  × mcand;  sum += pp
    //            (mplier has been shifted right by 11, mcand left by 11)
    //   Stage 3: pp = mplier[10:0]  × mcand;  sum += pp
    //            (shifted again — now covers mplier bits [32:22])
    //
    // After 3 stages, all 33 multiplier bits have been consumed.
    // The remaining mplier bits [63:33] were zero (from zero-extension)
    // so no further stages are needed.
    //
    // Each stage's combinational path is only an 11-bit × 64-bit
    // multiply plus a 64-bit add — easily meets 250 MHz on ASIC.
    // ----------------------------------------------------------------

    // --- Stage 1 (MUL_P1): first partial product -------------------

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
            mp1_sum          <= pp1;                                            // prev_sum = 0
            mp1_mcand        <= {s1_mcand[AW-1-SHIFT:0], {SHIFT{1'b0}}};       // shift left
            mp1_mplier       <= {{SHIFT{1'b0}}, s1_mplier[AW-1:SHIFT]};        // shift right
        end
    end

    // --- Stage 2 (MUL_P2): second partial product ------------------

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
            mp2_sum          <= mp1_sum + pp2;                                  // accumulate
            mp2_mcand        <= {mp1_mcand[AW-1-SHIFT:0], {SHIFT{1'b0}}};      // shift left
            mp2_mplier       <= {{SHIFT{1'b0}}, mp1_mplier[AW-1:SHIFT]};       // shift right
        end
    end

    // --- Stage 3 (MUL_P3): third partial product (final) -----------

    logic [AW-1:0] pp3;
    assign pp3 = mp2_mplier[SHIFT-1:0] * mp2_mcand;

    logic               mp3_valid;
    logic               mp3_is_index;
    logic               mp3_is_component;
    logic [31:0]        mp3_new_mid;
    logic [AW-1:0]      mp3_product;     // final multiply result

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
            mp3_product      <= mp2_sum + pp3;                                  // final sum
            // No need to pipe mcand/mplier further — all 33 bits consumed
        end
    end

    // ----------------------------------------------------------------
    // Stage 4 (ACC): accumulate, spread compare, trade output
    //
    // mp3_product is the correct signed Q44.20 weighted delta
    // (low 64 bits of unsigned partial-product chain = signed result
    // due to 2's-complement modular arithmetic).
    //
    // No forwarding hazard: consecutive TOB updates entering S0 on
    // cycles N and N+1 reach S4 on cycles N+4 and N+5.  The
    // accumulate from N+4 writes index_accum on posedge N+4, so
    // cycle N+5's S4 reads the already-updated value.
    // ----------------------------------------------------------------

    // Accumulate index value and track actual index price
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            index_accum <= '0;
            actual_mid  <= '0;
        end else if (!ready) begin
            index_accum <= '0;
            actual_mid  <= '0;
        end else begin
            if (mp3_is_component)
                index_accum <= index_accum + $signed(mp3_product);

            if (mp3_is_index)
                actual_mid <= mp3_new_mid;
        end
    end

    // ----------------------------------------------------------------
    // Startup clear / ready sequencing
    //
    // Synthesis clears old_mid one entry per cycle after reset so the
    // reset path does not fan out across the entire table.
    // ----------------------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin
`ifndef SYNTH
        if (!rst_n) begin
            ready <= 1'b0;
            for (int i = 0; i < N_COMPONENTS; i++)
                old_mid[i] <= 32'd0;
        end else begin
            ready <= 1'b1;
            if (s0_is_component)
                old_mid[s0_sym] <= s0_new_mid;
        end
`else
        if (!rst_n) begin
            ready       <= 1'b0;
            clear_mid_r <= '0;
        end else if (!ready) begin
            old_mid[clear_mid_r] <= 32'd0;

            if (clear_mid_r == SYMW'(N_COMPONENTS - 1)) begin
                ready <= 1'b1;
            end else begin
                clear_mid_r <= clear_mid_r + 1'b1;
            end
        end else begin
            ready <= 1'b1;
            if (s0_is_component)
                old_mid[s0_sym] <= s0_new_mid;
        end
`endif
    end

    // Spread calculation and trade signal generation
    //
    // Spread uses PROSPECTIVE next values so the output register
    // (clocked on the same edge as the accumulate) sees the
    // post-update state.
    logic signed [AW-1:0] next_accum;
    logic        [31:0]   next_actual;
    logic signed [AW-1:0] actual_q;
    logic signed [AW-1:0] spread;
    logic        [AW-1:0] abs_spread;
    logic                  trade_fire;
    logic                  trade_dir;  // 0=BUY, 1=SELL

    always_comb begin
        next_accum  = mp3_is_component ? (index_accum + $signed(mp3_product))
                                       : index_accum;
        next_actual = mp3_is_index     ? mp3_new_mid : actual_mid;

        actual_q   = AW'(next_actual) <<< FRAC;
        spread     = next_accum - actual_q;
        abs_spread = spread[AW-1] ? AW'(-spread) : AW'(spread);
        trade_fire = (abs_spread > threshold) && (next_actual != 32'd0);
        trade_dir  = ~spread[AW-1];  // positive spread → SELL
    end

    // Output register
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            out_trade <= '0;
        end else if (!ready) begin
            out_trade <= '0;
        end else begin
            out_trade.valid          <= mp3_valid && trade_fire;
            out_trade.direction      <= trade_dir;
            out_trade.spread         <= spread;
            out_trade.computed_index <= next_accum;
            out_trade.actual_price   <= next_actual;
        end
    end

    // Monitor port — always reflects latest index value
    assign out_index = index_accum;

endmodule
