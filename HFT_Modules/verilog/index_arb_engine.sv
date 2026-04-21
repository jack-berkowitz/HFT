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

    // RAM sizing: at least one address bit, covering every component index.
    localparam RAM_ADDR_W = (N_COMPONENTS <= 2) ? 1 : $clog2(N_COMPONENTS);
    localparam RAM_DEPTH  = 1 << RAM_ADDR_W;

    // ------------------------------------------------------------------
    // Cycle-0 decode (purely combinational from in_tob)
    // ------------------------------------------------------------------
    logic [SYMW-1:0]    in_sym;
    logic [31:0]        in_new_mid;
    logic               in_two_sided;
    logic               in_is_index;
    logic               in_is_component;

    assign in_sym        = in_tob.symbol_idx;
    assign in_new_mid    = (in_tob.best_bid.price + in_tob.best_ask.price) >> 1;
    assign in_two_sided  = (in_tob.best_bid.qty != 32'd0) &&
                           (in_tob.best_ask.qty != 32'd0);
    assign in_is_index   = in_tob.valid && in_two_sided &&
                           (in_tob.symbol_idx == IDX_SYM[SYMW-1:0]);
    assign in_is_component = in_tob.valid && in_two_sided &&
                             (in_tob.symbol_idx != IDX_SYM[SYMW-1:0]) &&
                             (in_tob.symbol_idx < SYMW'(N_COMPONENTS));

    // ------------------------------------------------------------------
    // dp_ram instances (replace huge register arrays with block RAM).
    //   weight_ram : external wt_wr interface writes; pipeline reads
    //   old_mid_ram: pipeline writes via Port B; pipeline reads via Port A
    // ------------------------------------------------------------------
    logic [RAM_ADDR_W-1:0] w_a_addr;
    logic [WW-1:0]          w_a_rdata;
    logic [RAM_ADDR_W-1:0] w_b_addr;
    logic [WW-1:0]          w_b_wdata;
    logic                    w_b_wen;
    logic [WW-1:0]          w_b_rdata;

    dp_ram #(
        .ADDR_W (RAM_ADDR_W),
        .DATA_W (WW),
        .DEPTH  (RAM_DEPTH)
    ) weight_ram (
        .clk     (clk),
        .rst_n   (rst_n),
        .a_addr  (w_a_addr),
        .a_rdata (w_a_rdata),
        .b_addr  (w_b_addr),
        .b_wdata (w_b_wdata),
        .b_wen   (w_b_wen),
        .b_rdata (w_b_rdata)
    );

    logic [RAM_ADDR_W-1:0] m_a_addr;
    logic [31:0]            m_a_rdata;
    logic [RAM_ADDR_W-1:0] m_b_addr;
    logic [31:0]            m_b_wdata;
    logic                    m_b_wen;
    logic [31:0]            m_b_rdata;

    dp_ram #(
        .ADDR_W (RAM_ADDR_W),
        .DATA_W (32),
        .DEPTH  (RAM_DEPTH)
    ) old_mid_ram (
        .clk     (clk),
        .rst_n   (rst_n),
        .a_addr  (m_a_addr),
        .a_rdata (m_a_rdata),
        .b_addr  (m_b_addr),
        .b_wdata (m_b_wdata),
        .b_wen   (m_b_wen),
        .b_rdata (m_b_rdata)
    );

    // ------------------------------------------------------------------
    // Reset-time clear: after rst_n deasserts, walk the RAMs and zero
    // every entry so the pipeline never reads X from uninitialized RAM.
    // (RAM_DEPTH cycles ~= a few µs at 250 MHz — negligible vs market
    // feed cadence.)
    // ------------------------------------------------------------------
    typedef enum logic [0:0] { S_CLEAR, S_READY } state_t;
    state_t                state_r;
    logic [RAM_ADDR_W-1:0] clear_addr_r;
    logic                   ready;

    assign ready = (state_r == S_READY);

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            state_r      <= S_CLEAR;
            clear_addr_r <= '0;
        end else begin
            case (state_r)
                S_CLEAR: begin
                    if (clear_addr_r == RAM_ADDR_W'(RAM_DEPTH - 1))
                        state_r <= S_READY;
                    else
                        clear_addr_r <= clear_addr_r + 1'b1;
                end
                default: ;
            endcase
        end
    end

    // weight_ram: read on in_sym.
    //   During S_CLEAR: port B zeroes each entry.
    //   During S_READY: port B accepts external wt_wr updates.
    assign w_a_addr  = in_sym[RAM_ADDR_W-1:0];
    assign w_b_addr  = ready ? wt_wr_addr[RAM_ADDR_W-1:0] : clear_addr_r;
    assign w_b_wdata = ready ? wt_wr_data : '0;
    assign w_b_wen   = ready ? (wt_wr_en &&
                                (wt_wr_addr < SYMW'(N_COMPONENTS)))
                             : 1'b1;

    // ------------------------------------------------------------------
    // Lookup (lt) stage — 1 cycle after input.  Holds the fields we need
    // while the RAMs deliver a_rdata on the following cycle.
    // ------------------------------------------------------------------
    logic               lt_valid;
    logic               lt_is_index;
    logic               lt_is_component;
    logic [SYMW-1:0]    lt_sym;
    logic [31:0]        lt_new_mid;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            lt_valid        <= 1'b0;
            lt_is_index     <= 1'b0;
            lt_is_component <= 1'b0;
            lt_sym          <= '0;
            lt_new_mid      <= '0;
        end else begin
            lt_valid        <= in_tob.valid && ready;
            lt_is_index     <= in_is_index && ready;
            lt_is_component <= in_is_component && ready;
            lt_sym          <= in_sym;
            lt_new_mid      <= in_new_mid;
        end
    end

    // old_mid_ram write port: issued 1 cycle after the matching read.
    // That means the read of mem[lt_sym] on port A at this cycle still
    // sees the PRE-write value (sampled at the previous posedge), so we
    // need a bypass register for back-to-back same-symbol updates.
    // During S_CLEAR, port B zeroes each entry.
    assign m_a_addr  = in_sym[RAM_ADDR_W-1:0];
    assign m_b_addr  = ready ? lt_sym[RAM_ADDR_W-1:0] : clear_addr_r;
    assign m_b_wdata = ready ? lt_new_mid : 32'd0;
    assign m_b_wen   = ready ? lt_is_component : 1'b1;

    // Write-bypass: when this cycle's lt_sym matches the sym we wrote
    // LAST cycle, the RAM's a_rdata still holds the older value — use
    // the bypass register instead.
    logic [SYMW-1:0]    wr_bp_sym;
    logic [31:0]        wr_bp_mid;
    logic               wr_bp_valid;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            wr_bp_sym   <= '0;
            wr_bp_mid   <= '0;
            wr_bp_valid <= 1'b0;
        end else begin
            wr_bp_sym   <= lt_sym;
            wr_bp_mid   <= lt_new_mid;
            wr_bp_valid <= lt_is_component;
        end
    end

    logic [31:0] old_mid_bypassed;
    always_comb begin
        if (wr_bp_valid && (wr_bp_sym == lt_sym))
            old_mid_bypassed = wr_bp_mid;
        else
            old_mid_bypassed = m_a_rdata;
    end

    // ------------------------------------------------------------------
    // Compute delta/weight in the lt cycle and register into s1.
    // ------------------------------------------------------------------
    logic signed [32:0] lt_delta;
    logic [WW-1:0]      lt_weight;

    assign lt_delta  = $signed({1'b0, lt_new_mid}) -
                       $signed({1'b0, old_mid_bypassed});
    assign lt_weight = w_a_rdata;

    // ------------------------------------------------------------------
    // s1 register: feeds the multiply pipeline.
    // ------------------------------------------------------------------
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
            s1_valid        <= lt_valid;
            s1_is_index     <= lt_is_index;
            s1_is_component <= lt_is_component;
            s1_new_mid      <= lt_new_mid;
            // Zero the operand path when lt isn't valid so X from the
            // uninitialized dp_ram (pre-S_CLEAR, or IDX_SYM's unused
            // entry) cannot leak into the multiplier pipeline.
            s1_mcand        <= lt_valid
                                 ? {{(AW-33){lt_delta[32]}}, lt_delta}
                                 : '0;
            s1_mplier       <= lt_valid
                                 ? {{(AW-33){1'b0}}, 1'b0, lt_weight}
                                 : '0;
        end
    end

    // ------------------------------------------------------------------
    // 3-stage shift-and-add multiply (unchanged).
    // ------------------------------------------------------------------
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
            mp1_is_index     <= s1_valid && s1_is_index;
            mp1_is_component <= s1_valid && s1_is_component;
            mp1_new_mid      <= s1_new_mid;
            mp1_sum          <= s1_valid ? pp1 : '0;
            mp1_mcand        <= s1_valid
                                  ? {s1_mcand[AW-1-SHIFT:0], {SHIFT{1'b0}}}
                                  : '0;
            mp1_mplier       <= s1_valid
                                  ? {{SHIFT{1'b0}}, s1_mplier[AW-1:SHIFT]}
                                  : '0;
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
            mp2_is_index     <= mp1_valid && mp1_is_index;
            mp2_is_component <= mp1_valid && mp1_is_component;
            mp2_new_mid      <= mp1_new_mid;
            mp2_sum          <= mp1_valid ? (mp1_sum + pp2) : '0;
            mp2_mcand        <= mp1_valid
                                  ? {mp1_mcand[AW-1-SHIFT:0], {SHIFT{1'b0}}}
                                  : '0;
            mp2_mplier       <= mp1_valid
                                  ? {{SHIFT{1'b0}}, mp1_mplier[AW-1:SHIFT]}
                                  : '0;
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
            mp3_is_index     <= mp2_valid && mp2_is_index;
            mp3_is_component <= mp2_valid && mp2_is_component;
            mp3_new_mid      <= mp2_new_mid;
            mp3_product      <= mp2_valid ? (mp2_sum + pp3) : '0;
        end
    end

    // ------------------------------------------------------------------
    // Accumulator + trade decision.
    //
    // X-prop hardening: the enable terms (mp3_is_component/is_index) are
    // ANDed with mp3_valid and `ready` before gating the flop update.  If
    // anything in the upstream pipeline glitches to X during gate-level
    // simulation (VCS +xprop=tmerge is far more aggressive than iverilog),
    // the gate squashes it instead of latching X into the accumulator.
    // ------------------------------------------------------------------
    logic signed [AW-1:0] index_accum;
    logic [31:0]          actual_mid;
    logic                  accum_en;
    logic                  actual_en;

    assign accum_en  = mp3_valid && mp3_is_component && ready;
    assign actual_en = mp3_valid && mp3_is_index     && ready;

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            index_accum <= '0;
            actual_mid  <= '0;
        end else begin
            if (accum_en)
                index_accum <= index_accum + $signed(mp3_product);

            if (actual_en)
                actual_mid <= mp3_new_mid;
        end
    end

    logic mp3_valid_d;

    always_ff @(posedge clk) begin
        if (!rst_n)
            mp3_valid_d <= 1'b0;
        else
            mp3_valid_d <= mp3_valid;
    end

    logic signed [AW-1:0] actual_q;
    logic signed [AW-1:0] spread;
    logic        [AW-1:0] abs_spread;
    logic                  trade_fire;
    logic                  trade_dir;

    always_comb begin
        actual_q   = AW'(actual_mid) <<< FRAC;
        spread     = $signed(index_accum) - actual_q;
        abs_spread = spread[AW-1] ? AW'(-spread) : AW'(spread);
        trade_fire = (abs_spread > threshold) && (actual_mid != 32'd0);
        trade_dir  = spread[AW-1];
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            out_trade <= '0;
        end else begin
            out_trade.valid          <= mp3_valid_d && trade_fire;
            out_trade.direction      <= trade_dir;
            out_trade.spread         <= spread;
            out_trade.computed_index <= index_accum;
            out_trade.actual_price   <= actual_mid;
        end
    end

    assign out_index = index_accum;

endmodule
