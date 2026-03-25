// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VTOP__SYMS_H_
#define VERILATED_VTOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vtop.h"

// INCLUDE MODULE CLASSES
#include "Vtop___024root.h"
#include "Vtop___024unit.h"

// DPI TYPES for DPI Export callbacks (Internal use)

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vtop__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtop* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtop___024root                 TOP;

    // SCOPE NAMES
    VerilatedScope* __Vscopep_TOP;
    VerilatedScope* __Vscopep_hft_pipeline_top;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_demux;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_filter;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_framer;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_index_arb;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_index_arb__unnamedblk1;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_index_arb__unnamedblk2;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__gen_bram__BRA__0__KET__;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__gen_bram__BRA__1__KET__;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__gen_bram__BRA__2__KET__;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__gen_bram__BRA__3__KET__;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__unnamedblk3;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__unnamedblk4;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__unnamedblk5;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__unnamedblk6;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__unnamedblk7;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__unnamedblk8;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_order_lookup__u_order_table__unnamedblk9;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk1;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk10;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk10__unnamedblk11;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk12;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk13;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk2;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk3;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk3__unnamedblk4;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk5;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk6;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk7;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk8;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tob__unnamedblk8__unnamedblk9;
    VerilatedScope* __Vscopep_hft_pipeline_top__u_tx;

    // SCOPE HIERARCHY
    VerilatedHierarchy __Vhier;

    // CONSTRUCTORS
    Vtop__Syms(VerilatedContext* contextp, const char* namep, Vtop* modelp);
    ~Vtop__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
