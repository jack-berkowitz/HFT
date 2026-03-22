// ============================================================================
// skewed_hash_table.sv
//
// Parameterizable N-way skewed (cuckoo) hash table for FPGA.
//
// Architecture
// ------------
//   N independent BRAM tables, each with its own hash function.
//   Lookup :  2-cycle pipelined, throughput-1.  All N BRAMs are read in
//             parallel via port A; stored keys are compared and the
//             matching value is muxed out.
//   Insert :  Multi-cycle state machine via BRAM port B.
//             Cuckoo eviction with configurable max chain depth.
//   Delete :  Multi-cycle via the same state machine (port B).
//
// BRAM ports
// ----------
//   Port A (read-only)  — lookup pipeline.  Never stalls.
//   Port B (read/write) — insert / delete controller.
//
// Hash function
// -------------
//   Default: per-way seed XOR -> bit rotation -> XOR fold to index width.
//   Replace hash_fn() for CRC-32, Zobrist, perfect hash, etc.
//
// Reset
// -----
//   After rst_n deasserts the controller clears every valid bit
//   (TBL_ENTRIES cycles).  `ready` is low until the sweep completes.
//
// ============================================================================

module skewed_hash_table #(
    parameter KEY_W       = 32,
    parameter VALUE_W     = 64,
    parameter N_WAYS      = 4,
    parameter TBL_ENTRIES = 1024,
    parameter MAX_CHAIN   = 16
) (
    input  logic                  clk,
    input  logic                  rst_n,

    output logic                  ready,

    // Lookup — 2-cycle pipeline, 1/cycle throughput
    input  logic                  lookup_valid,
    input  logic [KEY_W-1:0]      lookup_key,
    output logic                  lookup_result_valid,
    output logic                  lookup_hit,
    output logic [VALUE_W-1:0]    lookup_value,

    // Insert
    input  logic                  insert_valid,
    input  logic [KEY_W-1:0]      insert_key,
    input  logic [VALUE_W-1:0]    insert_value,
    output logic                  insert_ready,
    output logic                  insert_done,
    output logic                  insert_fail,

    // Delete
    input  logic                  delete_valid,
    input  logic [KEY_W-1:0]      delete_key,
    output logic                  delete_ready,
    output logic                  delete_done,
    output logic                  delete_not_found
);

    // =================================================================
    // Derived constants
    // =================================================================
    localparam IDX_W   = $clog2(TBL_ENTRIES);
    localparam ENTRY_W = 1 + KEY_W + VALUE_W;        // {valid, key, value}
    localparam WAY_W   = (N_WAYS > 1) ? $clog2(N_WAYS) : 1;
    localparam CHN_W   = $clog2(MAX_CHAIN + 1);

    // Entry bit-field positions
    localparam V_BIT  = KEY_W + VALUE_W;
    localparam KEY_HI = KEY_W + VALUE_W - 1;
    localparam KEY_LO = VALUE_W;
    localparam VAL_HI = VALUE_W - 1;
    localparam VAL_LO = 0;

    // =================================================================
    // Hash seeds (splitmix64 / murmur-family constants, up to 8 ways)
    // =================================================================
    localparam logic [63:0] HASH_SEEDS [0:7] = '{
        64'h9E3779B97F4A7C15, 64'h517CC1B727220A95,
        64'h6C62272E07BB0142, 64'hBF58476D1CE4E5B9,
        64'h94D049BB133111EB, 64'hC6A4A7935BD1E995,
        64'hE7037ED1A0B428DB, 64'h27D4EB2F165B7D1A
    };
    localparam int HASH_ROT [0:7] = '{3, 7, 13, 19, 23, 29, 37, 41};

    // =================================================================
    // Hash function — replace body for CRC / Zobrist / perfect hash
    // =================================================================
    function automatic [IDX_W-1:0] hash_fn(
        input [KEY_W-1:0] key,
        input int          way
    );
        logic [KEY_W-1:0] seed_k, mixed;
        logic [IDX_W-1:0] folded;
        int rot;

        // Tile 64-bit seed to fill KEY_W bits
        for (int b = 0; b < KEY_W; b++)
            seed_k[b] = HASH_SEEDS[way][b % 64];

        mixed = key ^ seed_k;

        // Rotate left by per-way coprime amount
        rot   = HASH_ROT[way] % KEY_W;
        mixed = (mixed << rot) | (mixed >> (KEY_W - rot));

        // XOR-fold to index width
        folded = '0;
        for (int b = 0; b < KEY_W; b++)
            folded[b % IDX_W] = folded[b % IDX_W] ^ mixed[b];

        return folded;
    endfunction

    // =================================================================
    // BRAM storage — N tables, true dual-port
    //   Port A : lookup reads   (addr_a, rdata_a)
    //   Port B : ctrl read/write (addr_b, rdata_b, wdata_b, wen_b)
    // =================================================================
    logic [IDX_W-1:0]   bram_addr_a  [N_WAYS];
    logic [ENTRY_W-1:0] bram_rdata_a [N_WAYS];

    logic [IDX_W-1:0]   bram_addr_b  [N_WAYS];
    logic [ENTRY_W-1:0] bram_rdata_b [N_WAYS];
    logic [ENTRY_W-1:0] bram_wdata_b [N_WAYS];
    logic               bram_wen_b   [N_WAYS];

    generate
        for (genvar w = 0; w < N_WAYS; w++) begin : gen_bram
            (* ram_style = "block" *)
            logic [ENTRY_W-1:0] mem [0:TBL_ENTRIES-1];

            // Port A — read only (lookup)
            always_ff @(posedge clk) begin
                bram_rdata_a[w] <= mem[bram_addr_a[w]];
            end

            // Port B — read/write (controller)
            always_ff @(posedge clk) begin
                if (bram_wen_b[w])
                    mem[bram_addr_b[w]] <= bram_wdata_b[w];
                bram_rdata_b[w] <= mem[bram_addr_b[w]];
            end
        end
    endgenerate

    // =================================================================
    // Lookup pipeline  (port A only — never stalls)
    //
    //   Cycle 0 : hash computed, BRAM addr driven    (combinational)
    //   Cycle 1 : BRAM data ready, key compare + mux (combinational)
    //             pipeline reg captures {valid, key}
    //   Cycle 2 : output register
    // =================================================================

    // Stage 0: drive BRAM read addresses
    always_comb begin
        for (int w = 0; w < N_WAYS; w++)
            bram_addr_a[w] = hash_fn(lookup_key, w);
    end

    // Pipeline register: stage 0 -> 1
    logic             s1_valid;
    logic [KEY_W-1:0] s1_key;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) s1_valid <= 1'b0;
        else        s1_valid <= lookup_valid;
    end
    always_ff @(posedge clk) begin
        s1_key <= lookup_key;
    end

    // Stage 1: compare stored keys, select hit (combinational)
    logic               s1_hit;
    logic [VALUE_W-1:0] s1_value;

    always_comb begin
        s1_hit   = 1'b0;
        s1_value = '0;
        for (int w = 0; w < N_WAYS; w++) begin
            if (bram_rdata_a[w][V_BIT] &&
                bram_rdata_a[w][KEY_HI:KEY_LO] == s1_key) begin
                s1_hit   = 1'b1;
                s1_value = bram_rdata_a[w][VAL_HI:VAL_LO];
            end
        end
    end

    // Output register: stage 1 -> 2
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            lookup_result_valid <= 1'b0;
            lookup_hit          <= 1'b0;
            lookup_value        <= '0;
        end else begin
            lookup_result_valid <= s1_valid;
            lookup_hit          <= s1_hit;
            lookup_value        <= s1_value;
        end
    end

    // =================================================================
    // Controller FSM — insert / delete  (port B)
    // =================================================================
    typedef enum logic [3:0] {
        ST_CLEAR,
        ST_IDLE,
        ST_READ,
        ST_WAIT,
        ST_COMPARE,
        ST_WRITE,
        ST_EVICT_READ,
        ST_EVICT_WAIT,
        ST_EVICT_DECIDE
    } state_t;

    state_t              state_r;

    // Operation context
    logic                op_is_insert_r;
    logic [KEY_W-1:0]    op_key_r;
    logic [VALUE_W-1:0]  op_value_r;

    // Clear counter
    logic [IDX_W-1:0]    clear_addr_r;

    // Write target (latched in ST_COMPARE)
    logic [WAY_W-1:0]    sel_way_r;
    logic [IDX_W-1:0]    sel_idx_r;

    // Eviction context
    logic                needs_evict_r;
    logic [KEY_W-1:0]    evict_key_r;
    logic [VALUE_W-1:0]  evict_value_r;
    logic [WAY_W-1:0]    evict_dst_way_r;
    logic [IDX_W-1:0]    evict_dst_idx_r;
    logic [CHN_W-1:0]    chain_cnt_r;

    // -----------------------------------------------------------------
    // Combinational: scan port-B read data  (used in ST_COMPARE)
    // -----------------------------------------------------------------
    logic              cmp_found_existing;
    logic              cmp_found_empty;
    logic [WAY_W-1:0]  cmp_sel_way;
    logic [IDX_W-1:0]  cmp_sel_idx;

    always_comb begin
        cmp_found_existing = 1'b0;
        cmp_found_empty    = 1'b0;
        cmp_sel_way        = '0;
        cmp_sel_idx        = hash_fn(op_key_r, 0);

        // Priority: existing match first
        for (int w = 0; w < N_WAYS; w++) begin
            if (bram_rdata_b[w][V_BIT] &&
                bram_rdata_b[w][KEY_HI:KEY_LO] == op_key_r) begin
                cmp_found_existing = 1'b1;
                cmp_sel_way        = WAY_W'(w);
                cmp_sel_idx        = hash_fn(op_key_r, w);
            end
        end

        // Then first empty slot
        if (!cmp_found_existing) begin
            for (int w = 0; w < N_WAYS; w++) begin
                if (!bram_rdata_b[w][V_BIT] && !cmp_found_empty) begin
                    cmp_found_empty = 1'b1;
                    cmp_sel_way     = WAY_W'(w);
                    cmp_sel_idx     = hash_fn(op_key_r, w);
                end
            end
        end
    end

    // -----------------------------------------------------------------
    // Combinational: check eviction destination slot (ST_EVICT_DECIDE)
    // -----------------------------------------------------------------
    logic evict_slot_empty;
    assign evict_slot_empty = !bram_rdata_b[evict_dst_way_r][V_BIT];

    // -----------------------------------------------------------------
    // Port B address / write-data / write-enable mux
    // -----------------------------------------------------------------
    always_comb begin
        for (int w = 0; w < N_WAYS; w++) begin
            bram_addr_b[w]  = '0;
            bram_wdata_b[w] = '0;
            bram_wen_b[w]   = 1'b0;
        end

        case (state_r)
            ST_CLEAR: begin
                for (int w = 0; w < N_WAYS; w++) begin
                    bram_addr_b[w]  = clear_addr_r;
                    bram_wdata_b[w] = '0;               // valid = 0
                    bram_wen_b[w]   = 1'b1;
                end
            end

            ST_READ: begin
                for (int w = 0; w < N_WAYS; w++)
                    bram_addr_b[w] = hash_fn(op_key_r, w);
            end

            ST_WRITE: begin
                bram_addr_b[sel_way_r]  = sel_idx_r;
                bram_wdata_b[sel_way_r] = op_is_insert_r
                    ? {1'b1, op_key_r, op_value_r}      // insert: valid = 1
                    : '0;                                // delete: valid = 0
                bram_wen_b[sel_way_r]   = 1'b1;
            end

            ST_EVICT_READ: begin
                bram_addr_b[evict_dst_way_r] = evict_dst_idx_r;
            end

            ST_EVICT_DECIDE: begin
                bram_addr_b[evict_dst_way_r]  = evict_dst_idx_r;
                bram_wdata_b[evict_dst_way_r] = {1'b1, evict_key_r, evict_value_r};
                bram_wen_b[evict_dst_way_r]   = 1'b1;
            end

            default: ;
        endcase
    end

    // -----------------------------------------------------------------
    // State machine — sequential
    // -----------------------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state_r         <= ST_CLEAR;
            clear_addr_r    <= '0;
            op_is_insert_r  <= 1'b0;
            op_key_r        <= '0;
            op_value_r      <= '0;
            sel_way_r       <= '0;
            sel_idx_r       <= '0;
            needs_evict_r   <= 1'b0;
            evict_key_r     <= '0;
            evict_value_r   <= '0;
            evict_dst_way_r <= '0;
            evict_dst_idx_r <= '0;
            chain_cnt_r     <= '0;
            insert_done     <= 1'b0;
            insert_fail     <= 1'b0;
            delete_done     <= 1'b0;
            delete_not_found <= 1'b0;
        end else begin
            // Clear single-cycle pulses
            insert_done      <= 1'b0;
            insert_fail      <= 1'b0;
            delete_done      <= 1'b0;
            delete_not_found <= 1'b0;

            case (state_r)
                // =====================================================
                ST_CLEAR: begin
                    if (clear_addr_r == IDX_W'(TBL_ENTRIES - 1))
                        state_r <= ST_IDLE;
                    else
                        clear_addr_r <= clear_addr_r + 1'b1;
                end

                // =====================================================
                ST_IDLE: begin
                    needs_evict_r <= 1'b0;
                    chain_cnt_r   <= '0;
                    if (insert_valid) begin
                        op_is_insert_r <= 1'b1;
                        op_key_r       <= insert_key;
                        op_value_r     <= insert_value;
                        state_r        <= ST_READ;
                    end else if (delete_valid) begin
                        op_is_insert_r <= 1'b0;
                        op_key_r       <= delete_key;
                        state_r        <= ST_READ;
                    end
                end

                // =====================================================
                ST_READ: state_r <= ST_WAIT;

                // =====================================================
                ST_WAIT: state_r <= ST_COMPARE;

                // =====================================================
                ST_COMPARE: begin
                    sel_way_r <= cmp_sel_way;
                    sel_idx_r <= cmp_sel_idx;

                    if (op_is_insert_r) begin
                        if (cmp_found_existing || cmp_found_empty) begin
                            // Direct insert or update
                            state_r <= ST_WRITE;
                        end else begin
                            // All ways occupied — cuckoo eviction
                            // Displace way 0, move its occupant to way 1
                            sel_way_r       <= '0;
                            sel_idx_r       <= hash_fn(op_key_r, 0);
                            evict_key_r     <= bram_rdata_b[0][KEY_HI:KEY_LO];
                            evict_value_r   <= bram_rdata_b[0][VAL_HI:VAL_LO];
                            evict_dst_way_r <= WAY_W'(1 % N_WAYS);
                            evict_dst_idx_r <= hash_fn(
                                bram_rdata_b[0][KEY_HI:KEY_LO],
                                1 % N_WAYS
                            );
                            needs_evict_r   <= 1'b1;
                            chain_cnt_r     <= CHN_W'(1);
                            state_r         <= ST_WRITE;
                        end
                    end else begin
                        // Delete
                        if (cmp_found_existing)
                            state_r <= ST_WRITE;
                        else begin
                            delete_not_found <= 1'b1;
                            state_r          <= ST_IDLE;
                        end
                    end
                end

                // =====================================================
                ST_WRITE: begin
                    if (needs_evict_r) begin
                        state_r <= ST_EVICT_READ;
                    end else begin
                        if (op_is_insert_r) insert_done <= 1'b1;
                        else                delete_done <= 1'b1;
                        state_r <= ST_IDLE;
                    end
                end

                // =====================================================
                ST_EVICT_READ: state_r <= ST_EVICT_WAIT;

                // =====================================================
                ST_EVICT_WAIT: state_r <= ST_EVICT_DECIDE;

                // =====================================================
                ST_EVICT_DECIDE: begin
                    if (evict_slot_empty) begin
                        // Victim placed — done
                        // (write driven combinationally above)
                        insert_done   <= 1'b1;
                        needs_evict_r <= 1'b0;
                        chain_cnt_r   <= '0;
                        state_r       <= ST_IDLE;
                    end else if (chain_cnt_r >= CHN_W'(MAX_CHAIN)) begin
                        // Chain exhausted — fail
                        insert_fail   <= 1'b1;
                        needs_evict_r <= 1'b0;
                        chain_cnt_r   <= '0;
                        state_r       <= ST_IDLE;
                    end else begin
                        // Cascade: save displaced entry, write current victim
                        // (write driven combinationally above)
                        evict_key_r     <= bram_rdata_b[evict_dst_way_r][KEY_HI:KEY_LO];
                        evict_value_r   <= bram_rdata_b[evict_dst_way_r][VAL_HI:VAL_LO];
                        evict_dst_way_r <= WAY_W'((evict_dst_way_r + 1) % N_WAYS);
                        evict_dst_idx_r <= hash_fn(
                            bram_rdata_b[evict_dst_way_r][KEY_HI:KEY_LO],
                            (evict_dst_way_r + 1) % N_WAYS
                        );
                        chain_cnt_r <= chain_cnt_r + 1'b1;
                        state_r     <= ST_EVICT_READ;
                    end
                end

                // =====================================================
                default: state_r <= ST_IDLE;
            endcase
        end
    end

    // =================================================================
    // Handshake outputs
    // =================================================================
    assign ready        = (state_r == ST_IDLE);
    assign insert_ready = (state_r == ST_IDLE);
    assign delete_ready = (state_r == ST_IDLE);

endmodule