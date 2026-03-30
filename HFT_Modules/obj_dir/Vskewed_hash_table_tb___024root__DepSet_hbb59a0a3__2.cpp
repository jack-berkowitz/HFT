// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vskewed_hash_table_tb.h for the primary calling header

#include "Vskewed_hash_table_tb__pch.h"
#include "Vskewed_hash_table_tb___024root.h"

void Vskewed_hash_table_tb___024root___nba_sequent__TOP__0(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___nba_sequent__TOP__1(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___nba_sequent__TOP__2(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___act_comb__TOP__0(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___nba_comb__TOP__1(Vskewed_hash_table_tb___024root* vlSelf);

void Vskewed_hash_table_tb___024root___eval_nba(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vskewed_hash_table_tb___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vskewed_hash_table_tb___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vskewed_hash_table_tb___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((5ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vskewed_hash_table_tb___024root___act_comb__TOP__0(vlSelf);
    }
    if ((3ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vskewed_hash_table_tb___024root___nba_comb__TOP__1(vlSelf);
    }
}

void Vskewed_hash_table_tb___024root___timing_resume(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___timing_resume\n"); );
    // Body
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VtrigSched_hfda5f40f__0.resume("@(posedge skewed_hash_table_tb.clk)");
    }
    if ((4ULL & vlSelf->__VactTriggered.word(0U))) {
        vlSelf->__VdlySched.resume();
    }
}

void Vskewed_hash_table_tb___024root___timing_commit(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___timing_commit\n"); );
    // Body
    if ((! (1ULL & vlSelf->__VactTriggered.word(0U)))) {
        vlSelf->__VtrigSched_hfda5f40f__0.commit("@(posedge skewed_hash_table_tb.clk)");
    }
}

void Vskewed_hash_table_tb___024root___eval_triggers__act(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___eval_act(Vskewed_hash_table_tb___024root* vlSelf);

bool Vskewed_hash_table_tb___024root___eval_phase__act(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<3> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vskewed_hash_table_tb___024root___eval_triggers__act(vlSelf);
    Vskewed_hash_table_tb___024root___timing_commit(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vskewed_hash_table_tb___024root___timing_resume(vlSelf);
        Vskewed_hash_table_tb___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vskewed_hash_table_tb___024root___eval_phase__nba(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vskewed_hash_table_tb___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskewed_hash_table_tb___024root___dump_triggers__nba(Vskewed_hash_table_tb___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vskewed_hash_table_tb___024root___dump_triggers__act(Vskewed_hash_table_tb___024root* vlSelf);
#endif  // VL_DEBUG

void Vskewed_hash_table_tb___024root___eval(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vskewed_hash_table_tb___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("test/skewed_hash_table_tb.sv", 21, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vskewed_hash_table_tb___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("test/skewed_hash_table_tb.sv", 21, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vskewed_hash_table_tb___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vskewed_hash_table_tb___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vskewed_hash_table_tb___024root___eval_debug_assertions(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___eval_debug_assertions\n"); );
}
#endif  // VL_DEBUG
