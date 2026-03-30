// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vskewed_hash_table_tb.h for the primary calling header

#include "Vskewed_hash_table_tb__pch.h"
#include "Vskewed_hash_table_tb__Syms.h"
#include "Vskewed_hash_table_tb___024root.h"

VL_ATTR_COLD void Vskewed_hash_table_tb___024root___eval_initial__TOP(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___eval_initial__TOP\n"); );
    // Init
    VlWide<6>/*191:0*/ __Vtemp_1;
    // Body
    vlSelf->skewed_hash_table_tb__DOT__clk = 0U;
    __Vtemp_1[0U] = 0x2e766364U;
    __Vtemp_1[1U] = 0x655f7462U;
    __Vtemp_1[2U] = 0x7461626cU;
    __Vtemp_1[3U] = 0x6173685fU;
    __Vtemp_1[4U] = 0x65645f68U;
    __Vtemp_1[5U] = 0x736b6577U;
    vlSymsp->_vm_contextp__->dumpfile(VL_CVT_PACK_STR_NW(6, __Vtemp_1));
    VL_PRINTF_MT("-Info: test/skewed_hash_table_tb.sv:105: $dumpvar ignored, as Verilated without --trace\n");
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vskewed_hash_table_tb___024root___dump_triggers__stl(Vskewed_hash_table_tb___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vskewed_hash_table_tb___024root___eval_triggers__stl(Vskewed_hash_table_tb___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vskewed_hash_table_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vskewed_hash_table_tb___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (IData)(vlSelf->__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vskewed_hash_table_tb___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
