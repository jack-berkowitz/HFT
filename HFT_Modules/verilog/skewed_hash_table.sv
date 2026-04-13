module skewed_hash_table #(
    parameter KEY_W       = 32,
    parameter VALUE_W     = 64,
    parameter N_WAYS      = 4,
    parameter TBL_ENTRIES = 256,
    parameter MAX_CHAIN   = 16
) (
    input  logic                  clk,
    input  logic                  rst_n,

    output logic                  ready,

    input  logic                  lookup_valid,
    input  logic [KEY_W-1:0]      lookup_key,
    output logic                  lookup_result_valid,
    output logic                  lookup_hit,
    output logic [VALUE_W-1:0]    lookup_value,

    input  logic                  insert_valid,
    input  logic [KEY_W-1:0]      insert_key,
    input  logic [VALUE_W-1:0]    insert_value,
    output logic                  insert_ready,
    output logic                  insert_done,
    output logic                  insert_fail,

    input  logic                  delete_valid,
    input  logic [KEY_W-1:0]      delete_key,
    output logic                  delete_ready,
    output logic                  delete_done,
    output logic                  delete_not_found
);

    localparam IDX_W   = $clog2(TBL_ENTRIES);
    localparam ENTRY_W = 1 + KEY_W + VALUE_W;
    localparam WAY_W   = (N_WAYS > 1) ? $clog2(N_WAYS) : 1;
    localparam CHN_W   = $clog2(MAX_CHAIN + 1);

    localparam V_BIT  = KEY_W + VALUE_W;
    localparam KEY_HI = KEY_W + VALUE_W - 1;
    localparam KEY_LO = VALUE_W;
    localparam VAL_HI = VALUE_W - 1;
    localparam VAL_LO = 0;

    localparam [31:0] CRC32_POLY = 32'h04C1_1DB7;

    localparam [31:0] CRC_INIT_0 = 32'hFFFF_FFFF;
    localparam [31:0] CRC_INIT_1 = 32'h1EDC_6F41;
    localparam [31:0] CRC_INIT_2 = 32'hA833_982B;
    localparam [31:0] CRC_INIT_3 = 32'h04C1_1DB7;
    localparam [31:0] CRC_INIT_4 = 32'h8141_41AB;
    localparam [31:0] CRC_INIT_5 = 32'hD419_CC15;
    localparam [31:0] CRC_INIT_6 = 32'hE306_9283;
    localparam [31:0] CRC_INIT_7 = 32'hF4AC_FB13;

    function automatic [IDX_W-1:0] hash_fn(
        input [KEY_W-1:0]      key,
        input logic [WAY_W-1:0] way
    );
        logic [31:0]      crc;
        logic              xor_bit;
        logic [IDX_W-1:0] folded;

        case (way)
            0: crc = CRC_INIT_0;
            1: crc = CRC_INIT_1;
            2: crc = CRC_INIT_2;
            3: crc = CRC_INIT_3;
            4: crc = CRC_INIT_4;
            5: crc = CRC_INIT_5;
            6: crc = CRC_INIT_6;
            default: crc = CRC_INIT_7;
        endcase

        for (int b = KEY_W - 1; b >= 0; b--) begin
            xor_bit = crc[31] ^ key[b];
            crc = {crc[30:0], 1'b0};
            if (xor_bit) crc = crc ^ CRC32_POLY;
        end

        folded = '0;
        for (int b = 0; b < 32; b++)
            folded[b % IDX_W] = folded[b % IDX_W] ^ crc[b];

        return folded;
    endfunction

    logic [IDX_W-1:0]   bram_addr_a  [N_WAYS];
    logic [ENTRY_W-1:0] bram_rdata_a [N_WAYS];

    logic [IDX_W-1:0]   bram_addr_b  [N_WAYS];
    logic [ENTRY_W-1:0] bram_rdata_b [N_WAYS];
    logic [ENTRY_W-1:0] bram_wdata_b [N_WAYS];
    logic               bram_wen_b   [N_WAYS];

    generate
        for (genvar w = 0; w < N_WAYS; w++) begin : gen_bram
            dp_ram #(
                .ADDR_W (IDX_W),
                .DATA_W (ENTRY_W),
                .DEPTH  (TBL_ENTRIES)
            ) u_ram (
                .clk     (clk),
                .a_addr  (bram_addr_a[w]),
                .a_rdata (bram_rdata_a[w]),
                .b_addr  (bram_addr_b[w]),
                .b_wdata (bram_wdata_b[w]),
                .b_wen   (bram_wen_b[w]),
                .b_rdata (bram_rdata_b[w])
            );
        end
    endgenerate

    always_comb begin
        for (int w = 0; w < N_WAYS; w++)
            bram_addr_a[w] = hash_fn(lookup_key, w);
    end

    logic             s1_valid;
    logic [KEY_W-1:0] s1_key;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) s1_valid <= 1'b0;
        else        s1_valid <= lookup_valid;
    end
    always_ff @(posedge clk) begin
        s1_key <= lookup_key;
    end

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

    logic                op_is_insert_r;
    logic [KEY_W-1:0]    op_key_r;
    logic [VALUE_W-1:0]  op_value_r;

    logic [IDX_W-1:0]    clear_addr_r;

    logic [WAY_W-1:0]    sel_way_r;
    logic [IDX_W-1:0]    sel_idx_r;

    logic                needs_evict_r;
    logic [KEY_W-1:0]    evict_key_r;
    logic [VALUE_W-1:0]  evict_value_r;
    logic [WAY_W-1:0]    evict_dst_way_r;
    logic [IDX_W-1:0]    evict_dst_idx_r;
    logic [CHN_W-1:0]    chain_cnt_r;

    logic              cmp_found_existing;
    logic              cmp_found_empty;
    logic [WAY_W-1:0]  cmp_sel_way;
    logic [IDX_W-1:0]  cmp_sel_idx;

    always_comb begin
        cmp_found_existing = 1'b0;
        cmp_found_empty    = 1'b0;
        cmp_sel_way        = '0;
        cmp_sel_idx        = hash_fn(op_key_r, 0);

        for (int w = 0; w < N_WAYS; w++) begin
            if (bram_rdata_b[w][V_BIT] &&
                bram_rdata_b[w][KEY_HI:KEY_LO] == op_key_r) begin
                cmp_found_existing = 1'b1;
                cmp_sel_way        = WAY_W'(w);
                cmp_sel_idx        = hash_fn(op_key_r, w);
            end
        end

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

    logic evict_slot_empty;
    assign evict_slot_empty = !bram_rdata_b[evict_dst_way_r][V_BIT];

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
                    bram_wdata_b[w] = '0;
                    bram_wen_b[w]   = 1'b1;
                end
            end

            ST_READ, ST_WAIT, ST_COMPARE: begin
                for (int w = 0; w < N_WAYS; w++)
                    bram_addr_b[w] = hash_fn(op_key_r, w);
            end

            ST_WRITE: begin
                bram_addr_b[sel_way_r]  = sel_idx_r;
                bram_wdata_b[sel_way_r] = op_is_insert_r
                    ? {1'b1, op_key_r, op_value_r}
                    : '0;
                bram_wen_b[sel_way_r]   = 1'b1;
            end

            ST_EVICT_READ, ST_EVICT_WAIT: begin
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
            insert_done      <= 1'b0;
            insert_fail      <= 1'b0;
            delete_done      <= 1'b0;
            delete_not_found <= 1'b0;

            case (state_r)
                ST_CLEAR: begin
                    if (clear_addr_r == IDX_W'(TBL_ENTRIES - 1))
                        state_r <= ST_IDLE;
                    else
                        clear_addr_r <= clear_addr_r + 1'b1;
                end

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

                ST_READ: state_r <= ST_WAIT;

                ST_WAIT: state_r <= ST_COMPARE;

                ST_COMPARE: begin
                    sel_way_r <= cmp_sel_way;
                    sel_idx_r <= cmp_sel_idx;

                    if (op_is_insert_r) begin
                        if (cmp_found_existing || cmp_found_empty) begin
                            state_r <= ST_WRITE;
                        end else begin
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
                        if (cmp_found_existing)
                            state_r <= ST_WRITE;
                        else begin
                            delete_not_found <= 1'b1;
                            state_r          <= ST_IDLE;
                        end
                    end
                end

                ST_WRITE: begin
                    if (needs_evict_r) begin
                        state_r <= ST_EVICT_READ;
                    end else begin
                        if (op_is_insert_r) insert_done <= 1'b1;
                        else                delete_done <= 1'b1;
                        state_r <= ST_IDLE;
                    end
                end

                ST_EVICT_READ: state_r <= ST_EVICT_WAIT;

                ST_EVICT_WAIT: state_r <= ST_EVICT_DECIDE;

                ST_EVICT_DECIDE: begin
                    if (evict_slot_empty) begin
                        insert_done   <= 1'b1;
                        needs_evict_r <= 1'b0;
                        chain_cnt_r   <= '0;
                        state_r       <= ST_IDLE;
                    end else if (chain_cnt_r >= CHN_W'(MAX_CHAIN)) begin
                        insert_fail   <= 1'b1;
                        needs_evict_r <= 1'b0;
                        chain_cnt_r   <= '0;
                        state_r       <= ST_IDLE;
                    end else begin
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

                default: state_r <= ST_IDLE;
            endcase
        end
    end

    assign ready        = (state_r == ST_IDLE);
    assign insert_ready = (state_r == ST_IDLE);
    assign delete_ready = (state_r == ST_IDLE);

endmodule
