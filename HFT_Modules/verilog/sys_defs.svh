/////////////////////////////////////////////////////////////////////////
//                                                                     //
//   Modulename :  sys_defs.svh                                        //
//                                                                     //
//  Description :  This file defines macros and data structures used   //
//                 throughout the processor.                           //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

`ifndef __SYS_DEFS_SVH__
`define __SYS_DEFS_SVH__


`define DATA_W 64
typedef logic [`DATA_W-1:0]  AXI_TDATA;
`define KEEP_W 64/8
typedef logic [`KEEP_W-1:0]  AXI_KTEEP;

`define MDP3_PORT 16'd14310
`define HDR_BEATS 6







// all files should `include "sys_defs.svh" to at least define the timescale
`timescale 1ns/100ps

`ifndef SYNTH
    //`define DEBUG
    //`define WAVE_FORM
`endif

// RS types: ALU, MULT, LS, BRJU

typedef enum logic [4:0] {
    // Zero register means that we should get the value
    // from the imm feild, for actual references to the zero
    // register then set the imm field to zero!

    ALU_ADD     = 'd0,
    ALU_SLT     = 'd1,
    ALU_SUB     = 'd2,
    ALU_SLTU    = 'd3,
    ALU_XOR     = 'd4,
    ALU_OR      = 'd5,
    ALU_AND     = 'd6,
    ALU_SLL     = 'd7,
    ALU_SRL     = 'd8,
    ALU_SRA     = 'd9,

    ALU_LUI     = 'd10
} ALU_FUNC;

typedef enum logic [4:0] {
    MULT_MUL        = 'd0,
    MULT_MULH       = 'd1,
    MULT_MULHSU     = 'd2,
    MULT_MULHU      = 'd3

    //DIV        = 'd4,
    //DIVU       = 'd5,
    
    //REM        = 'd6,
    //REMU       = 'd7
} MULT_FUNC;

typedef enum logic [4:0] {
    LOAD_LB      = 'd0,
    LOAD_LH      = 'd1,
    LOAD_LW      = 'd2,
    LOAD_LBU     = 'd3,
    LOAD_LHU     = 'd4
} LOAD_FUNC;

typedef enum logic [4:0] {
    STORE_SB      = 'd0,
    STORE_SH      = 'd1,
    STORE_SW      = 'd2
} STORE_FUNC;

typedef enum logic [4:0] {
    BRJU_AUIPC   = 'd0,
    BRJU_JAL     = 'd1,
    BRJU_JALR    = 'd2,
    BRJU_BEQ     = 'd3,
    BRJU_BNE     = 'd4,
    BRJU_BLT     = 'd5,
    BRJU_BGE     = 'd6,
    BRJU_BLTU    = 'd7,
    BRJU_BGEU    = 'd8,
    BRJU_WFI     = 'd9
} BRJU_FUNC;

typedef union packed {
    ALU_FUNC   alu_func;
    BRJU_FUNC  brju_func;
    LOAD_FUNC  load_func;
    MULT_FUNC  mult_func;
    STORE_FUNC store_func;
} fu_operation;

`define ALU_FU_ID   'd0
`define MULT_FU_ID  'd1
`define LOAD_FU_ID  'd2
`define STORE_FU_ID 'd3
`define BRJU_FU_ID  'd4
`define SYS_FU_ID   'd5

typedef enum logic [2:0] {
    ALU     = `ALU_FU_ID,
    MULT    = `MULT_FU_ID,
    L       = `LOAD_FU_ID,
    S       = `STORE_FU_ID,
    BRJU    = `BRJU_FU_ID,
    SYS     = `SYS_FU_ID
} fu_type;


///////////////////////////////////
// ---- Starting Parameters ---- //
///////////////////////////////////

// some starting parameters that you should set
// this is *your* processor, you decide these values (try analyzing which is best!)

// superscalar width
`define N_WAY 2
`define CDB_SZ `N_WAY // This MUST match your superscalar width

`define load_wait_q_sz 4
typedef logic [$clog2(`load_wait_q_sz)-1:0] WAITQPTR;


// sizes
`define ROB_SIZE (1 << 5)
`define PHYS_REG_SZ_R10K (`N_ARCH_REG + `ROB_SIZE + 4)

// Arch Reg defs
`define N_ARCH_REG 32
//typedef logic [`ARCH_REG_SZ_R10K-1:0] ARCH_TAG;


`define BR_QUEUE_SZ 6
typedef logic  [`BR_QUEUE_SZ-1:0] BR_MASK;
typedef logic  [`PHYS_REG_SZ_R10K-1:0] PR_TAG;
typedef PR_TAG [`N_ARCH_REG-1:0] REG_MAPPINGS;
typedef logic [`PHYS_REG_SZ_R10K-1:0] FREE_MAPPINGS;
typedef logic [$clog2(`ROB_SIZE)-1:0] ROB_PTR;

`define ZERO_PR_TAG '0

// worry about these later
`define BRANCH_PRED_SZ xx
`define LSQ_SZ xx
`define LSQ_HT_SIZE

`define SQ_size 4
`define SQ_PTR_LEN $clog2(`SQ_size)
typedef logic [`SQ_PTR_LEN-1:0] SQ_PTR;
///////////////////////////////
// ---- Basic Constants ---- //
///////////////////////////////

// NOTE: the global CLOCK_PERIOD is defined in the Makefile

// useful boolean single-bit definitions
`define FALSE 1'h0
`define TRUE  1'h1

`define BR_HISTORY_BITS 10
`define BR_BTB_SIZE 1024
`define BR_ENTRY_SIZE 8

// word and register sizes
typedef logic [31:0] ADDR;
typedef logic [31:0] DATA;
typedef logic [4:0] REG_IDX;
typedef logic [$clog2(`N_WAY)-1:0] CDB_IDX;
typedef logic [`BR_HISTORY_BITS-1:0] BR_HISTORY;

// the zero register
// In RISC-V, any read of this register returns zero and any writes are thrown away
`define ZERO_REG 5'd0

// Basic NOP instruction. Allows pipline registers to clearly be reset with
// an instruction that does nothing instead of Zero which is really an ADDI x0, x0, 0
`define NOP 32'h00000013



///////////////////////////////
// ---- CDB struct ---- //
///////////////////////////////

typedef struct packed {
     PR_TAG [`N_WAY-1:0] pr;
     DATA   [`N_WAY-1:0] data;
     logic  [`N_WAY-1:0] valid;
} CDB_PCKT;


//////////////////////////////////
// ---- Memory Definitions ---- //
//////////////////////////////////

// you are not allowed to change this definition for your final processor
// the project 3 processor has a massive boost in performance just from having no mem latency
// see if you can beat it's CPI in project 4 even with a 100ns latency!
//`define MEM_LATENCY_IN_CYCLES 2
`define MEM_LATENCY_IN_CYCLES (100.0/`CLOCK_PERIOD+0.49999)
// the 0.49999 is to force ceiling(100/period). The default behavior for
// float to integer conversion is rounding to nearest

// memory tags represent a unique id for outstanding mem transactions
// 0 is a sentinel value and is not a valid tag
`define NUM_MEM_TAGS 15
typedef logic [3:0] MEM_TAG;

// icache definitions
`define ICACHE_LINES 32
`define ICACHE_LINE_BITS $clog2(`ICACHE_LINES)

`define MEM_SIZE_IN_BYTES (64*1024)
`define MEM_64BIT_LINES   (`MEM_SIZE_IN_BYTES/8)

// A memory or cache block
typedef union packed {
    logic [7:0][7:0]  byte_level;
    logic [3:0][15:0] half_level;
    logic [1:0][31:0] word_level;
    logic      [63:0] dbbl_level;
} MEM_BLOCK;

typedef enum logic [1:0] {
    BYTE   = 2'h0,
    HALF   = 2'h1,
    WORD   = 2'h2,
    DOUBLE = 2'h3
} MEM_SIZE;

// Memory bus commands
typedef enum logic [1:0] {
    MEM_NONE   = 2'h0,
    MEM_LOAD   = 2'h1,
    MEM_STORE  = 2'h2
} MEM_COMMAND;


// icache tag struct
typedef struct packed {
    logic [12-`ICACHE_LINE_BITS:0] tags;
    logic                          valid;
} ICACHE_TAG;

///////////////////////////////
// ---- Exception Codes ---- //
///////////////////////////////

/**
 * Exception codes for when something goes wrong in the processor.
 * Note that we use HALTED_ON_WFI to signify the end of computation.
 * It's original meaning is to 'Wait For an Interrupt', but we generally
 * ignore interrupts in 470
 *
 * This mostly follows the RISC-V Privileged spec
 * except a few add-ons for our infrastructure
 * The majority of them won't be used, but it's good to know what they are
 */

typedef enum logic [3:0] {
    INST_ADDR_MISALIGN  = 4'h0,
    INST_ACCESS_FAULT   = 4'h1,
    ILLEGAL_INST        = 4'h2,
    BREAKPOINT          = 4'h3,
    LOAD_ADDR_MISALIGN  = 4'h4,
    LOAD_ACCESS_FAULT   = 4'h5,
    STORE_ADDR_MISALIGN = 4'h6,
    STORE_ACCESS_FAULT  = 4'h7,
    ECALL_U_MODE        = 4'h8,
    ECALL_S_MODE        = 4'h9,
    NO_ERROR            = 4'ha, // a reserved code that we use to signal no errors
    ECALL_M_MODE        = 4'hb,
    INST_PAGE_FAULT     = 4'hc,
    LOAD_PAGE_FAULT     = 4'hd,
    HALTED_ON_WFI       = 4'he, // 'Wait For Interrupt'. In 470, signifies the end of computation
    STORE_PAGE_FAULT    = 4'hf
} EXCEPTION_CODE;

///////////////////////////////////
// ---- Instruction Typedef ---- //
///////////////////////////////////

// from the RISC-V ISA spec
typedef union packed {
    logic [31:0] inst;
    struct packed {
        logic [6:0] funct7;
        logic [4:0] rs2; // source register 2
        logic [4:0] rs1; // source register 1
        logic [2:0] funct3;
        logic [4:0] rd; // destination register
        logic [6:0] opcode;
    } r; // register-to-register instructions
    struct packed {
        logic [11:0] imm; // immediate value for calculating address
        logic [4:0]  rs1; // source register 1 (used as address base)
        logic [2:0]  funct3;
        logic [4:0]  rd;  // destination register
        logic [6:0]  opcode;
    } i; // immediate or load instructions
    struct packed {
        logic [6:0] off; // offset[11:5] for calculating address
        logic [4:0] rs2; // source register 2
        logic [4:0] rs1; // source register 1 (used as address base)
        logic [2:0] funct3;
        logic [4:0] set; // offset[4:0] for calculating address
        logic [6:0] opcode;
    } s; // store instructions
    struct packed {
        logic       of;  // offset[12]
        logic [5:0] s;   // offset[10:5]
        logic [4:0] rs2; // source register 2
        logic [4:0] rs1; // source register 1
        logic [2:0] funct3;
        logic [3:0] et;  // offset[4:1]
        logic       f;   // offset[11]
        logic [6:0] opcode;
    } b; // branch instructions
    struct packed {
        logic [19:0] imm; // immediate value
        logic [4:0]  rd; // destination register
        logic [6:0]  opcode;
    } u; // upper-immediate instructions
    struct packed {
        logic       of; // offset[20]
        logic [9:0] et; // offset[10:1]
        logic       s;  // offset[11]
        logic [7:0] f;  // offset[19:12]
        logic [4:0] rd; // destination register
        logic [6:0] opcode;
    } j;  // jump instructions

// extensions for other instruction types
`ifdef ATOMIC_EXT
    struct packed {
        logic [4:0] funct5;
        logic       aq;
        logic       rl;
        logic [4:0] rs2;
        logic [4:0] rs1;
        logic [2:0] funct3;
        logic [4:0] rd;
        logic [6:0] opcode;
    } a; // atomic instructions
`endif
`ifdef SYSTEM_EXT
    struct packed {
        logic [11:0] csr;
        logic [4:0]  rs1;
        logic [2:0]  funct3;
        logic [4:0]  rd;
        logic [6:0]  opcode;
    } sys; // system call instructions
`endif

} INST; // instruction typedef, this should cover all types of instructions

////////////////////////////////////////
// ---- Datapath Control Signals ---- //
////////////////////////////////////////

typedef enum logic {
    OP_IS_REG    = 1'h0,
    OP_IS_IMM    = 1'h1
} OP_SELECT;

/**
 * Commit Packet:
 * This is an output of the processor and used in the testbench for counting
 * committed instructions
 *
 * It also acts as a "WB_PACKET", and can be reused in the final project with
 * some slight changes
 */
typedef struct packed {
    ADDR    NPC;
    DATA    data;
    REG_IDX reg_idx;
    logic   halt;
    logic   illegal;
    logic   valid;
} COMMIT_PACKET;

typedef struct packed {
    REG_IDX dst_arch;
    REG_IDX src1_arch;
    REG_IDX src2_arch;
    OP_SELECT src2_select;

    fu_type fu;
    fu_operation func;

    logic predicted_taken;
    ADDR  predicted_target;
    
    logic wfi_flag;
    DATA imm;
    BR_MASK br_mask;
    BR_HISTORY br_history;
    //INST inst;
    DATA pc;
} decoded_inst;

typedef struct packed {
    INST  inst;
    ADDR  pc;
    logic taken;
    ADDR  npc;
    BR_HISTORY history;
} fetched_inst;

typedef struct packed {
    ROB_PTR       rob_tail_ptr;
    SQ_PTR        sq_tail_ptr;
    REG_MAPPINGS  map_snapshot;
    FREE_MAPPINGS free_snapshot;
    BR_MASK       mask_snapshot;
} cpu_state_t;

typedef enum logic [1:0] {
    STRONGLY_NOT_TAKEN = 2'd0,
    WEAKLY_NOT_TAKEN = 2'd1,
    WEAKLY_TAKEN = 2'd2,
    STRONGLY_TAKEN = 2'd3
} branch_counter_t;

typedef struct packed {
    PR_TAG old_tag;
    PR_TAG dst_tag;
    logic wfi_flag;
    logic has_dest;

    ADDR    pc;
    REG_IDX dst_reg;
} rob_entry_t;

// If b is older than a
function automatic logic is_older_or_same_rob(
    input ROB_PTR a,
    input ROB_PTR b,
    input ROB_PTR head
);
    // branchless, relative-to-head compare
    ROB_PTR da = a - head; // unsigned wrap
    ROB_PTR db = b - head;
    return (db <= da); // strict
endfunction

// Check if instruction is a conditional branch
function automatic logic is_conditional_branch(INST inst);
    unique casez (inst)
        `RV32_BEQ,
        `RV32_BNE,
        `RV32_BLT,
        `RV32_BGE,
        `RV32_BLTU,
        `RV32_BGEU:
            return 1'b1;
        default:
            return 1'b0;
    endcase
endfunction

// Check if instruction is an unconditional jump
function automatic logic is_jump(INST inst);
    unique casez (inst)
        `RV32_JAL,
        `RV32_JALR:
            return 1'b1;
        default:
            return 1'b0;
    endcase
endfunction

// Check if instruction is any branch/jump
function automatic logic is_branch(INST inst);
    return is_conditional_branch(inst) || is_jump(inst);
endfunction

`endif // __SYS_DEFS_SVH__
////////////////////////////////
// ---- Datapath Packets ---- //
////////////////////////////////