// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vskewed_hash_table_tb.h for the primary calling header

#ifndef VERILATED_VSKEWED_HASH_TABLE_TB___024ROOT_H_
#define VERILATED_VSKEWED_HASH_TABLE_TB___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vskewed_hash_table_tb__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vskewed_hash_table_tb___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ skewed_hash_table_tb__DOT__clk;
        CData/*0:0*/ skewed_hash_table_tb__DOT__rst_n;
        CData/*0:0*/ skewed_hash_table_tb__DOT__lookup_valid;
        CData/*0:0*/ skewed_hash_table_tb__DOT__lookup_result_valid;
        CData/*0:0*/ skewed_hash_table_tb__DOT__lookup_hit;
        CData/*0:0*/ skewed_hash_table_tb__DOT__insert_valid;
        CData/*0:0*/ skewed_hash_table_tb__DOT__insert_done;
        CData/*0:0*/ skewed_hash_table_tb__DOT__insert_fail;
        CData/*0:0*/ skewed_hash_table_tb__DOT__delete_valid;
        CData/*0:0*/ skewed_hash_table_tb__DOT__delete_done;
        CData/*0:0*/ skewed_hash_table_tb__DOT__delete_not_found;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT__s1_valid;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT__s1_hit;
        CData/*3:0*/ skewed_hash_table_tb__DOT__dut__DOT__state_r;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT__op_is_insert_r;
        CData/*5:0*/ skewed_hash_table_tb__DOT__dut__DOT__clear_addr_r;
        CData/*1:0*/ skewed_hash_table_tb__DOT__dut__DOT__sel_way_r;
        CData/*5:0*/ skewed_hash_table_tb__DOT__dut__DOT__sel_idx_r;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT__needs_evict_r;
        CData/*1:0*/ skewed_hash_table_tb__DOT__dut__DOT__evict_dst_way_r;
        CData/*5:0*/ skewed_hash_table_tb__DOT__dut__DOT__evict_dst_idx_r;
        CData/*3:0*/ skewed_hash_table_tb__DOT__dut__DOT__chain_cnt_r;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT__cmp_found_existing;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT__cmp_found_empty;
        CData/*1:0*/ skewed_hash_table_tb__DOT__dut__DOT__cmp_sel_way;
        CData/*5:0*/ skewed_hash_table_tb__DOT__dut__DOT__cmp_sel_idx;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT__evict_slot_empty;
        CData/*0:0*/ skewed_hash_table_tb__DOT__dut__DOT____Vlvbound_heb4ee106__0;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__38__Vfuncout;
        CData/*0:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__38__xor_bit;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__38__folded;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__39__Vfuncout;
        CData/*0:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__39__xor_bit;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__39__folded;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__40__Vfuncout;
        CData/*0:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__40__xor_bit;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__40__folded;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__41__Vfuncout;
        CData/*0:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__41__xor_bit;
        CData/*5:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__41__folded;
        CData/*0:0*/ __Vdlyvset__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v0;
        CData/*0:0*/ __Vdlyvset__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v1;
        CData/*0:0*/ __Vdlyvset__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v2;
        CData/*0:0*/ __Vdlyvset__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v3;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __Vtrigprevexpr___TOP__skewed_hash_table_tb__DOT__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__skewed_hash_table_tb__DOT__rst_n__0;
        CData/*0:0*/ __VactContinue;
        IData/*31:0*/ skewed_hash_table_tb__DOT__lookup_key;
        IData/*31:0*/ skewed_hash_table_tb__DOT__insert_key;
        IData/*31:0*/ skewed_hash_table_tb__DOT__delete_key;
        IData/*31:0*/ skewed_hash_table_tb__DOT__cap_wr;
        IData/*31:0*/ skewed_hash_table_tb__DOT__dut__DOT__s1_key;
        IData/*31:0*/ skewed_hash_table_tb__DOT__dut__DOT__op_key_r;
        IData/*31:0*/ skewed_hash_table_tb__DOT__dut__DOT__evict_key_r;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__38__key;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__38__crc;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__39__key;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__39__crc;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__40__key;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__40__crc;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__41__key;
        IData/*31:0*/ __Vfunc_skewed_hash_table_tb__DOT__dut__DOT__hash_fn__41__crc;
        VlWide<4>/*96:0*/ __Vdlyvval__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v0;
    };
    struct {
        VlWide<4>/*96:0*/ __Vdlyvval__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v1;
        VlWide<4>/*96:0*/ __Vdlyvval__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v2;
        VlWide<4>/*96:0*/ __Vdlyvval__skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b__v3;
        IData/*31:0*/ __VactIterCount;
        QData/*63:0*/ skewed_hash_table_tb__DOT__lookup_value;
        QData/*63:0*/ skewed_hash_table_tb__DOT__insert_value;
        QData/*63:0*/ skewed_hash_table_tb__DOT__dut__DOT__s1_value;
        QData/*63:0*/ skewed_hash_table_tb__DOT__dut__DOT__op_value_r;
        QData/*63:0*/ skewed_hash_table_tb__DOT__dut__DOT__evict_value_r;
        VlUnpacked<CData/*0:0*/, 32> skewed_hash_table_tb__DOT__cap_hits;
        VlUnpacked<QData/*63:0*/, 32> skewed_hash_table_tb__DOT__cap_values;
        VlUnpacked<IData/*31:0*/, 8> skewed_hash_table_tb__DOT__test_keys;
        VlUnpacked<QData/*63:0*/, 8> skewed_hash_table_tb__DOT__test_values;
        VlUnpacked<IData/*31:0*/, 5> skewed_hash_table_tb__DOT__unnamedblk1__DOT__bb_keys;
        VlUnpacked<CData/*0:0*/, 5> skewed_hash_table_tb__DOT__unnamedblk1__DOT__bb_exp_hit;
        VlUnpacked<QData/*63:0*/, 5> skewed_hash_table_tb__DOT__unnamedblk1__DOT__bb_exp_val;
        VlUnpacked<CData/*5:0*/, 4> skewed_hash_table_tb__DOT__dut__DOT__bram_addr_a;
        VlUnpacked<VlWide<4>/*96:0*/, 4> skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_a;
        VlUnpacked<CData/*5:0*/, 4> skewed_hash_table_tb__DOT__dut__DOT__bram_addr_b;
        VlUnpacked<VlWide<4>/*96:0*/, 4> skewed_hash_table_tb__DOT__dut__DOT__bram_rdata_b;
        VlUnpacked<VlWide<4>/*96:0*/, 4> skewed_hash_table_tb__DOT__dut__DOT__bram_wdata_b;
        VlUnpacked<CData/*0:0*/, 4> skewed_hash_table_tb__DOT__dut__DOT__bram_wen_b;
        VlUnpacked<VlWide<4>/*96:0*/, 64> skewed_hash_table_tb__DOT__dut__DOT__gen_bram__BRA__0__KET____DOT__mem;
        VlUnpacked<VlWide<4>/*96:0*/, 64> skewed_hash_table_tb__DOT__dut__DOT__gen_bram__BRA__1__KET____DOT__mem;
        VlUnpacked<VlWide<4>/*96:0*/, 64> skewed_hash_table_tb__DOT__dut__DOT__gen_bram__BRA__2__KET____DOT__mem;
        VlUnpacked<VlWide<4>/*96:0*/, 64> skewed_hash_table_tb__DOT__dut__DOT__gen_bram__BRA__3__KET____DOT__mem;
    };
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_hfda5f40f__0;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<3> __VactTriggered;
    VlTriggerVec<3> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vskewed_hash_table_tb__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vskewed_hash_table_tb___024root(Vskewed_hash_table_tb__Syms* symsp, const char* v__name);
    ~Vskewed_hash_table_tb___024root();
    VL_UNCOPYABLE(Vskewed_hash_table_tb___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
