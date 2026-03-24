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
// Pipelined: 2-cycle latency from tob input to trade output.
//   Stage 0: compute mid, read old_mid + weight, compute delta
//   Stage 1: multiply, accumulate, compare, output
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

    // Trade signal output
    output trade_signal_t              out_trade,

    // Index value monitor port (active every cycle)
    output logic [`ACCUM_W-1:0]       out_index
);

    localparam FRAC = `WEIGHT_FRAC_BITS;   // 20
    localparam AW   = `ACCUM_W;            // 64
    localparam WW   = `WEIGHT_W;           // 32
    localparam SYMW = `SYM_IDX_W;          // 9

    // ----------------------------------------------------------------
    // Storage
    // ----------------------------------------------------------------

    // Per-component weight table (Q12.20, written at startup)
    logic [WW-1:0]  weight_tbl [N_COMPONENTS];

    // Per-component last-known mid-price (32-bit integer, e.g. cents)
    logic [31:0]    old_mid    [N_COMPONENTS];

    // Running weighted index accumulator (Q44.20 signed)
    logic signed [AW-1:0] index_accum;

    // Tracked actual index instrument mid-price
    logic [31:0]          actual_mid;

    // ----------------------------------------------------------------
    // Weight load port
    // ----------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            for (int i = 0; i < N_COMPONENTS; i++)
                weight_tbl[i] <= '0;
        end else if (wt_wr_en && wt_wr_addr < SYMW'(N_COMPONENTS)) begin
            weight_tbl[wt_wr_addr] <= wt_wr_data;
        end
    end

    // ----------------------------------------------------------------
    // Stage 0: compute mid-price, delta, read weight
    //
    // Mid-price = (bid + ask) / 2
    // Only update if both bid and ask are nonzero (two-sided market).
    // If one side is empty, skip the update — old_mid holds the
    // last valid price and the index stays consistent.
    // ----------------------------------------------------------------
    logic               s0_valid;
    logic               s0_is_index;      // is this the actual index symbol?
    logic               s0_is_component;  // is this a component update?
    logic [SYMW-1:0]    s0_sym;
    logic [31:0]        s0_new_mid;
    logic signed [32:0] s0_delta;         // signed 33-bit: new_mid - old_mid
    logic [WW-1:0]      s0_weight;
    logic               s0_two_sided;     // both bid and ask present

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

    // Pipeline register: stage 0 → stage 1
    logic               s1_valid;
    logic               s1_is_index;
    logic               s1_is_component;
    logic [SYMW-1:0]    s1_sym;
    logic [31:0]        s1_new_mid;
    logic signed [32:0] s1_delta;
    logic [WW-1:0]      s1_weight;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            s1_valid        <= 1'b0;
            s1_is_index     <= 1'b0;
            s1_is_component <= 1'b0;
            s1_sym          <= '0;
            s1_new_mid      <= '0;
            s1_delta        <= '0;
            s1_weight       <= '0;
        end else begin
            s1_valid        <= s0_valid;
            s1_is_index     <= s0_is_index && in_tob.valid && s0_two_sided;
            s1_is_component <= s0_is_component;
            s1_sym          <= s0_sym;
            s1_new_mid      <= s0_new_mid;
            s1_delta        <= s0_delta;
            s1_weight       <= s0_weight;
        end
    end

    // Update old_mid at end of stage 0 (registered)
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            for (int i = 0; i < N_COMPONENTS; i++)
                old_mid[i] <= 32'd0;
        end else if (s0_is_component) begin
            old_mid[s0_sym] <= s0_new_mid;
        end
    end

    // ----------------------------------------------------------------
    // Stage 1: multiply, accumulate, compare, output
    // ----------------------------------------------------------------

    // Multiply: signed delta (33b) × unsigned weight (32b) → signed 65b
    // We keep 64 bits (Q44.20) — the MSB overflow is negligible
    // for reasonable price ranges.
    logic signed [AW-1:0] weighted_delta;

    always_comb begin
        weighted_delta = AW'($signed(s1_delta) * $signed({1'b0, s1_weight}));
    end

    // Accumulate index value and track actual index price
    always_ff @(posedge clk) begin
        if (!rst_n) begin
            index_accum <= '0;
            actual_mid  <= '0;
        end else begin
            if (s1_is_component)
                index_accum <= index_accum + weighted_delta;

            if (s1_is_index)
                actual_mid <= s1_new_mid;
        end
    end

    // ----------------------------------------------------------------
    // Spread calculation and trade signal generation
    //
    // KEY: spread must be computed from the PROSPECTIVE next values
    // of index_accum and actual_mid, because out_trade is registered
    // on the same edge that updates the accumulator.  Using the old
    // registered values would cause a 1-cycle misalignment where
    // s1_valid is high but spread still reflects the pre-update state.
    // ----------------------------------------------------------------
    logic signed [AW-1:0] next_accum;
    logic        [31:0]   next_actual;
    logic signed [AW-1:0] actual_q;
    logic signed [AW-1:0] spread;
    logic        [AW-1:0] abs_spread;
    logic                  trade_fire;
    logic                  trade_dir;  // 0=BUY, 1=SELL

    always_comb begin
        // Prospective values: what accum/actual will be AFTER this edge
        next_accum  = s1_is_component ? (index_accum + weighted_delta) : index_accum;
        next_actual = s1_is_index     ? s1_new_mid                    : actual_mid;

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
        end else begin
            out_trade.valid          <= s1_valid && trade_fire;
            out_trade.direction      <= trade_dir;
            out_trade.spread         <= spread;
            out_trade.computed_index <= next_accum;
            out_trade.actual_price   <= next_actual;
        end
    end

    // Monitor port — always reflects latest index value
    assign out_index = index_accum;

endmodule