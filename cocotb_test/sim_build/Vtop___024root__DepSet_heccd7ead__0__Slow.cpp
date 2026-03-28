// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"
#include "Vtop___024root.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    // Body
    Vtop___024root___eval_initial__TOP(vlSelf);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    // Body
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk3__DOT__w = 1U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk3__DOT__w = 2U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk3__DOT__w = 3U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk3__DOT__w = 4U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w = 1U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w = 2U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w = 3U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w = 4U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w = 1U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w = 2U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w = 3U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w = 4U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk7__DOT__w = 1U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk7__DOT__w = 2U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk7__DOT__w = 3U;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk7__DOT__w = 4U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i = 1U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i = 2U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i = 3U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i = 1U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i = 2U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i = 3U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i = 1U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i = 2U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i = 3U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i = 1U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i = 2U;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i = 3U;
}

VL_ATTR_COLD void Vtop___024root___eval_final(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelf->__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY((0x64U < __VstlIterCount))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("../HFT_modules/verilog/hft_pipeline_top.sv", 22, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtop___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf);
void Vtop___024root___ico_sequent__TOP__1(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vtop___024root___ico_sequent__TOP__0(vlSelf);
        Vtop___024root___ico_sequent__TOP__1(vlSelf);
    }
}

VL_ATTR_COLD void Vtop___024root___eval_triggers__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtop___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vtop___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VicoTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst_n = VL_RAND_RESET_I(1);
    vlSelf->rx_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->rx_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->rx_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->rx_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->rx_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->tx_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->tx_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->tx_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->tx_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->tx_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->tx_enable = VL_RAND_RESET_I(1);
    vlSelf->wt_wr_en = VL_RAND_RESET_I(1);
    vlSelf->wt_wr_addr = VL_RAND_RESET_I(9);
    vlSelf->wt_wr_data = VL_RAND_RESET_I(32);
    vlSelf->threshold = VL_RAND_RESET_Q(64);
    vlSelf->ol_ready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(138, vlSelf->tob_snapshot);
    VL_RAND_RESET_W(162, vlSelf->trade_signal);
    vlSelf->computed_index = VL_RAND_RESET_Q(64);
    vlSelf->order_count = VL_RAND_RESET_I(32);
    vlSelf->net_position = VL_RAND_RESET_I(32);
    vlSelf->pkt_sent = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__tx_enable = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__wt_wr_en = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__wt_wr_addr = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__wt_wr_data = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__threshold = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__ol_ready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__tob_snapshot);
    VL_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__trade_signal);
    vlSelf->hft_pipeline_top__DOT__computed_index = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__order_count = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__net_position = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__pkt_sent = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__f2f_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__f2f_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__f2f_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__f2f_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__f2f_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__f2d_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__f2d_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__f2d_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__f2d_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__f2d_tready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__decoded_msg);
    VL_RAND_RESET_W(402, vlSelf->hft_pipeline_top__DOT__ol_out);
    VL_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__tob_out);
    VL_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__arb_trade);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__state = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__state_next = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__beat_count = VL_RAND_RESET_I(3);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__beat_count_next = VL_RAND_RESET_I(3);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ethertype = VL_RAND_RESET_I(16);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ip_proto = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__udp_dport = VL_RAND_RESET_I(16);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ethertype_ok = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ip_proto_ok = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__udp_dport_ok = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__beat_transferred = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__state_r = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__state_next = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__prev_data_r = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__prev_last_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__combined_data = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__combined_keep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__u_demux__DOT__out_msg);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__state_r = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__state_next = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__beat_cnt_r = VL_RAND_RESET_I(3);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__beat_transferred = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__msg_type_r = VL_RAND_RESET_I(3);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__symbol_index_r = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__symbol_seq_num_r = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__order_id_r = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__new_order_id_r = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__price_r = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__qty_r = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__trade_id_r = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__side_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__printable_r = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ready = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in);
    VL_RAND_RESET_W(402, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__out);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_key = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_result_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_hit = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_key = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_value);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_ready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_done = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_fail = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_key = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_ready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_done = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_not_found = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__exec_remaining = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__state_r = VL_RAND_RESET_I(4);
    VL_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_hit_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_price_r = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_qty_r = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_side_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__wr_key_r = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__wr_val_r);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__repl_pending_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__ready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_key = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_result_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_hit = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_key = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_value);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_ready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_done = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_fail = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_key = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_ready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_done = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_not_found = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_addr_a[__Vi0] = VL_RAND_RESET_I(9);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        VL_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a[__Vi0]);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_addr_b[__Vi0] = VL_RAND_RESET_I(9);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        VL_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[__Vi0]);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        VL_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_wdata_b[__Vi0]);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_wen_b[__Vi0] = VL_RAND_RESET_I(1);
    }
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_key = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_hit = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__state_r = VL_RAND_RESET_I(4);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_is_insert_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_key_r = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_value_r);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__clear_addr_r = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__sel_way_r = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__sel_idx_r = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__needs_evict_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_key_r = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_value_r);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_dst_way_r = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_dst_idx_r = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__chain_cnt_r = VL_RAND_RESET_I(5);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_existing = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_empty = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_way = VL_RAND_RESET_I(2);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_idx = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_slot_empty = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 512; ++__Vi0) {
        VL_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__0__KET____DOT__mem[__Vi0]);
    }
    for (int __Vi0 = 0; __Vi0 < 512; ++__Vi0) {
        VL_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__1__KET____DOT__mem[__Vi0]);
    }
    for (int __Vi0 = 0; __Vi0 < 512; ++__Vi0) {
        VL_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__2__KET____DOT__mem[__Vi0]);
    }
    for (int __Vi0 = 0; __Vi0 < 512; ++__Vi0) {
        VL_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__3__KET____DOT__mem[__Vi0]);
    }
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk3__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk7__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk8__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk9__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT____Vlvbound_hb313f869__0 = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rst_n = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(402, vlSelf->hft_pipeline_top__DOT__u_tob__DOT__in_update);
    VL_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__u_tob__DOT__out_tob);
    for (int __Vi0 = 0; __Vi0 < 22; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 3; ++__Vi1) {
            vlSelf->hft_pipeline_top__DOT__u_tob__DOT__bid_book[__Vi0][__Vi1] = VL_RAND_RESET_Q(64);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 22; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 3; ++__Vi1) {
            vlSelf->hft_pipeline_top__DOT__u_tob__DOT__ask_book[__Vi0][__Vi1] = VL_RAND_RESET_Q(64);
        }
    }
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__sym = VL_RAND_RESET_I(5);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__sym_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__msg_type = VL_RAND_RESET_I(3);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__do_remove = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__do_add = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rm_price = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rm_qty = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_price = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_qty = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__op_side = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__is_bid = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_tob__DOT__cur[__Vi0] = VL_RAND_RESET_Q(64);
    }
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_tob__DOT__after_rm[__Vi0] = VL_RAND_RESET_Q(64);
    }
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rm_found = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_tob__DOT__after_add[__Vi0] = VL_RAND_RESET_Q(64);
    }
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_match_found = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_inserted = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__better_than = VL_RAND_RESET_I(3);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_he393dcfc__0 = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h8199cc1e__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h81b53c54__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_ha1ace092__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_ha1b6c32c__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h0974e947__0 = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_ha9a88706__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_ha9a2f2c4__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h960b21d1__0 = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_he95362a5__0 = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__rst_n = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__in_tob);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__wt_wr_en = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__wt_wr_addr = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__wt_wr_data = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__threshold = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__out_trade);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__out_index = VL_RAND_RESET_Q(64);
    for (int __Vi0 = 0; __Vi0 < 20; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__weight_tbl[__Vi0] = VL_RAND_RESET_I(32);
    }
    for (int __Vi0 = 0; __Vi0 < 20; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__old_mid[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__index_accum = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__actual_mid = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_is_index = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_is_component = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_new_mid = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_delta = VL_RAND_RESET_Q(33);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_weight = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_two_sided = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_valid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_is_index = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_is_component = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_sym = VL_RAND_RESET_I(9);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_new_mid = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_delta = VL_RAND_RESET_Q(33);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_weight = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__weighted_delta = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__next_accum = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__next_actual = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__actual_q = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__spread = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__abs_spread = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__trade_fire = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__trade_dir = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__unnamedblk1__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__unnamedblk2__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT____Vlvbound_h536e47fe__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT____Vlvbound_hb6d5d6c1__0 = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__rst_n = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__enable = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__u_tx__DOT__in_trade);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tdata = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tkeep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tvalid = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tlast = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tready = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__order_count = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__net_position = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__pkt_sent = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__state_r = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_r = VL_RAND_RESET_I(4);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_dir = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_price = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_spread = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_oid = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__cd_cnt = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__cd_ok = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__next_pos = VL_RAND_RESET_I(32);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__pos_ok = VL_RAND_RESET_I(1);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__oid_seq = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_data = VL_RAND_RESET_Q(64);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_keep = VL_RAND_RESET_I(8);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_last = VL_RAND_RESET_I(1);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__Vfuncout = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__key = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__Vfuncout = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__key = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__Vfuncout = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__key = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__mixed = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__folded = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__Vfuncout = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__key = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__mixed = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__folded = VL_RAND_RESET_I(9);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__14__Vfuncout = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__14__b0 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__14__b1 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__Vfuncout = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b0 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b1 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b2 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b3 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b4 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b5 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b6 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b7 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__Vfuncout = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b2 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b3 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b4 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b5 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b6 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b7 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__Vfuncout = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b2 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b3 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b4 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b5 = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__18__Vfuncout = VL_RAND_RESET_Q(64);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__18__b4 = VL_RAND_RESET_I(8);
    VL_RAND_RESET_W(130, vlSelf->__Vdlyvval__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0);
    vlSelf->__Vdlyvset__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0 = 0;
    VL_RAND_RESET_W(130, vlSelf->__Vdlyvval__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1);
    vlSelf->__Vdlyvset__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1 = 0;
    VL_RAND_RESET_W(130, vlSelf->__Vdlyvval__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2);
    vlSelf->__Vdlyvset__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2 = 0;
    VL_RAND_RESET_W(130, vlSelf->__Vdlyvval__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3);
    vlSelf->__Vdlyvset__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3 = 0;
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_RAND_RESET_I(1);
}
