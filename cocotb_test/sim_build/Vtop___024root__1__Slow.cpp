// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vtop___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vtop___024root___eval_triggers_vec__stl(Vtop___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtop___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtop___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vtop___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtop___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtop___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge hft_pipeline_top.u_filter.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(posedge hft_pipeline_top.u_framer.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @(posedge hft_pipeline_top.u_demux.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 3U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 3 is active: @(posedge hft_pipeline_top.u_order_lookup.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 4U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 4 is active: @(negedge hft_pipeline_top.u_order_lookup.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 5U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 5 is active: @(posedge hft_pipeline_top.u_order_lookup.u_order_table.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 6U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 6 is active: @(negedge hft_pipeline_top.u_order_lookup.u_order_table.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 7U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 7 is active: @(posedge hft_pipeline_top.u_tob.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 8U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 8 is active: @(posedge hft_pipeline_top.u_index_arb.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 9U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 9 is active: @(posedge hft_pipeline_top.u_tx.clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->rx_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1766576090537307473ull);
    vlSelf->rx_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3699250831578094818ull);
    vlSelf->rx_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16901844738938546726ull);
    vlSelf->rx_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17313850635725891322ull);
    vlSelf->rx_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17008955825989863508ull);
    vlSelf->tx_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 15280893002004073044ull);
    vlSelf->tx_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8073956047196029554ull);
    vlSelf->tx_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1514037243208384647ull);
    vlSelf->tx_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9048229713893083557ull);
    vlSelf->tx_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14215705844279024247ull);
    vlSelf->tx_enable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11129962599077753393ull);
    vlSelf->wt_wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15517756907737324508ull);
    vlSelf->wt_wr_addr = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 16223348707427448375ull);
    vlSelf->wt_wr_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6447999509987322569ull);
    vlSelf->threshold = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 6329978984072683215ull);
    vlSelf->ol_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9588565970071027097ull);
    VL_SCOPED_RAND_RESET_W(138, vlSelf->tob_snapshot, __VscopeHash, 133155815851829196ull);
    VL_SCOPED_RAND_RESET_W(162, vlSelf->trade_signal, __VscopeHash, 2563736808664121828ull);
    vlSelf->computed_index = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 4925944991225965516ull);
    vlSelf->order_count = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 795828523642253550ull);
    vlSelf->net_position = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13324049443235176874ull);
    vlSelf->pkt_sent = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2899769578999202093ull);
    vlSelf->hft_pipeline_top__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2152104005945004858ull);
    vlSelf->hft_pipeline_top__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1022384862500571535ull);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 10853537656838041252ull);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8215792643370540805ull);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7539527733434332995ull);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17244062289479817151ull);
    vlSelf->hft_pipeline_top__DOT__rx_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9008278600633288901ull);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 15211820263278331735ull);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4255446074998157245ull);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17456637796214930260ull);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16791994407971778129ull);
    vlSelf->hft_pipeline_top__DOT__tx_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12619674759656402902ull);
    vlSelf->hft_pipeline_top__DOT__tx_enable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18170891549914586272ull);
    vlSelf->hft_pipeline_top__DOT__wt_wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16187486977206200153ull);
    vlSelf->hft_pipeline_top__DOT__wt_wr_addr = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 4137745694223979405ull);
    vlSelf->hft_pipeline_top__DOT__wt_wr_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3267418292038278366ull);
    vlSelf->hft_pipeline_top__DOT__threshold = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5124647952586165349ull);
    vlSelf->hft_pipeline_top__DOT__ol_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13164198025992605215ull);
    VL_SCOPED_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__tob_snapshot, __VscopeHash, 17298023546762970068ull);
    VL_SCOPED_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__trade_signal, __VscopeHash, 4859222082664080906ull);
    vlSelf->hft_pipeline_top__DOT__computed_index = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 13213614011424824981ull);
    vlSelf->hft_pipeline_top__DOT__order_count = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1137903909550414945ull);
    vlSelf->hft_pipeline_top__DOT__net_position = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3295586182487497938ull);
    vlSelf->hft_pipeline_top__DOT__pkt_sent = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7172723268666838066ull);
    vlSelf->hft_pipeline_top__DOT__f2f_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 3534071713870836391ull);
    vlSelf->hft_pipeline_top__DOT__f2f_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 15128826196466181543ull);
    vlSelf->hft_pipeline_top__DOT__f2f_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8971037699410783511ull);
    vlSelf->hft_pipeline_top__DOT__f2f_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1070595243125435227ull);
    vlSelf->hft_pipeline_top__DOT__f2f_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 633779237374665046ull);
    vlSelf->hft_pipeline_top__DOT__f2d_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 15788862029381802870ull);
    vlSelf->hft_pipeline_top__DOT__f2d_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 776715586403991583ull);
    vlSelf->hft_pipeline_top__DOT__f2d_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16167168057273369481ull);
    vlSelf->hft_pipeline_top__DOT__f2d_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5692484189215205930ull);
    vlSelf->hft_pipeline_top__DOT__f2d_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4968668980779455392ull);
    VL_SCOPED_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__decoded_msg, __VscopeHash, 810097150264545961ull);
    VL_SCOPED_RAND_RESET_W(402, vlSelf->hft_pipeline_top__DOT__ol_out, __VscopeHash, 2777176893501833788ull);
    VL_SCOPED_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__tob_out, __VscopeHash, 3227757859363834815ull);
    VL_SCOPED_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__arb_trade, __VscopeHash, 5291603759962233444ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5940806352492381689ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3081133670649667692ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8955713326387870900ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5334013592579880854ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6056081199401207882ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8380181904822018329ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8434411915121706822ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 3368647336806034135ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14820028063824099946ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4182993668287634947ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14754773907879824004ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__m_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17271726542720835581ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__state = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10252504691339450067ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__state_next = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5842248691932619033ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__beat_count = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 9428911409127087874ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__beat_count_next = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 15889372447818619305ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ethertype = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12291481413867827025ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ip_proto = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3369334362164112144ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__udp_dport = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2657280289813340191ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ethertype_ok = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3887010507248287391ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__ip_proto_ok = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18263474089096118017ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__udp_dport_ok = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9421631105296097961ull);
    vlSelf->hft_pipeline_top__DOT__u_filter__DOT__beat_transferred = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13088748917481921685ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3652150254472118063ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12967000892444276789ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 6784473066991865524ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12370325984497574502ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2432467474592969032ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2060522186559927439ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1909396093870903808ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 10495139247799301612ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 1233114489441029565ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1320774047219476007ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11507548176163365455ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__m_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10317522570435510901ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__state_r = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4428045627644801581ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__state_next = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 910407376422717433ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15517652994995333596ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__prev_data_r = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1586983812294382423ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10396073277199582176ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__prev_last_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4350563970014963811ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__combined_data = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 4975294966117907761ull);
    vlSelf->hft_pipeline_top__DOT__u_framer__DOT__combined_keep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 18119181370167489435ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6145604547166248328ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9404578685770853316ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 4339836771097901123ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 358347857236437586ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1593869547613350281ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 464129110885720696ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__s_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8012044315721015237ull);
    VL_SCOPED_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__u_demux__DOT__out_msg, __VscopeHash, 9713514678565310681ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__state_r = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14577069385232923713ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__state_next = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 949494003905178342ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__beat_cnt_r = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 12352130764750676861ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__beat_transferred = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2286183647002944738ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__msg_type_r = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3090226446608785255ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__symbol_index_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9239154745831195204ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__symbol_seq_num_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8392213214358372613ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__order_id_r = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5791699079500026471ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__new_order_id_r = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 3296744071122131526ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__price_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8527931286771187521ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__qty_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11588714034233403986ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__trade_id_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7809521661966666245ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__side_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3036774638046495525ull);
    vlSelf->hft_pipeline_top__DOT__u_demux__DOT__printable_r = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11057478378253097684ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12702974021804486374ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 847058625360297963ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 816264095659126129ull);
    VL_SCOPED_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in, __VscopeHash, 14004442981150161888ull);
    VL_SCOPED_RAND_RESET_W(402, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__out, __VscopeHash, 14168246701350774353ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13681132035216261252ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17830483771494319014ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_key = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 14685480280772859841ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_result_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10247077818653057694ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_hit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2591189499178246349ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value, __VscopeHash, 2886357666606545858ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12031897632629796082ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_key = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 9356449840173879568ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_value, __VscopeHash, 4841040773042112330ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12238885369473050437ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6645421229584434164ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_fail = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9057509928351294183ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16407154480940046439ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_key = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1861660253824540288ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14232452239950841714ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16811685023723118320ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_not_found = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3501114355967683404ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__exec_remaining = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14454385067801573878ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__state_r = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 11908983734370665210ull);
    VL_SCOPED_RAND_RESET_W(301, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r, __VscopeHash, 13359002255600506925ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_hit_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9523072854167358106ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_price_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4071445251706560545ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_qty_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10061432739830689965ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__lu_side_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15561328541163279634ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__wr_key_r = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 11052229728554010777ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__wr_val_r, __VscopeHash, 3494867631447343973ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__repl_pending_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 466426837090009385ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8241233798893467911ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4417376858963791711ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16207195646352704161ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8352894145682457106ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_key = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 3531213665039991647ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_result_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18368684071440057882ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_hit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15848624489505867700ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value, __VscopeHash, 15680403020333527315ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4883117187174929404ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_key = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 722392205245218255ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_value, __VscopeHash, 6504725486248554128ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9705979239109345119ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8214141908306808501ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_fail = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6123598547339322534ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14587542152547367055ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_key = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 7745994048617545417ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9168221256540839692ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3052800124735978392ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_not_found = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16850851721328773198ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_addr_a[__Vi0] = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 10885954667652938455ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a[__Vi0], __VscopeHash, 16610775787866559474ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_addr_b[__Vi0] = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 6076917150177271057ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[__Vi0], __VscopeHash, 16255716124158336777ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_wdata_b[__Vi0], __VscopeHash, 17409342664706424495ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_wen_b[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1884955507446760744ull);
    }
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15256469444222180140ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_key = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 418099415915299284ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_hit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7007546032146988178ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value, __VscopeHash, 12255114022320976347ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__state_r = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4044245937705771226ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_is_insert_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18094763638987937554ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_key_r = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 3854959482184916542ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_value_r, __VscopeHash, 4344314816250956963ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__clear_addr_r = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 15903475188577803886ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__sel_way_r = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15797630178248275073ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__sel_idx_r = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 17541183416601484161ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__needs_evict_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15968510057081030044ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_key_r = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 2689809650796725250ull);
    VL_SCOPED_RAND_RESET_W(65, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_value_r, __VscopeHash, 5405854179228078955ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_dst_way_r = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16723909731780685949ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_dst_idx_r = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 7944368822013479243ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__chain_cnt_r = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 6908349444687937203ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_existing = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2544534457347608450ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18327345723978369188ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_way = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 236275948392175569ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_idx = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 14846705297289836650ull);
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_slot_empty = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8045252358056212097ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__0__KET____DOT__mem[__Vi0], __VscopeHash, 4917635759329669291ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__1__KET____DOT__mem[__Vi0], __VscopeHash, 12600840337927184268ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__2__KET____DOT__mem[__Vi0], __VscopeHash, 3004708837401934308ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(130, vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__gen_bram__BRA__3__KET____DOT__mem[__Vi0], __VscopeHash, 5347041798099300893ull);
    }
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk3__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk7__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk8__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk9__DOT__w = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hffe5f552__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hffe5ed2a__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc01e3d52__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc01e555a__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h1a010b47__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h74f2d96b__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h813121f6__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h81311a8e__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd5cb8263__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h104c0e2d__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h104c1625__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd59551cd__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h3945d898__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc27b3eda__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc27b46c2__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h332dfccf__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd32e541e__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h99a7bc22__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 951226465712440808ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11677745549858421479ull);
    VL_SCOPED_RAND_RESET_W(402, vlSelf->hft_pipeline_top__DOT__u_tob__DOT__in_update, __VscopeHash, 12918407267445199431ull);
    VL_SCOPED_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__u_tob__DOT__out_tob, __VscopeHash, 6576200689313137011ull);
    for (int __Vi0 = 0; __Vi0 < 500; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 3; ++__Vi1) {
            vlSelf->hft_pipeline_top__DOT__u_tob__DOT__bid_book[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1986746674261798238ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 500; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 3; ++__Vi1) {
            vlSelf->hft_pipeline_top__DOT__u_tob__DOT__ask_book[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 15931288693141936933ull);
        }
    }
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__sym = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 12209049206968822044ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__sym_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14373950035529771483ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__msg_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5341774703769189749ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__do_remove = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8709718876414464266ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__do_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15357605054686749550ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rm_price = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10778609972175517241ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rm_qty = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5290841037316577877ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_price = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2971560200173197806ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_qty = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9495840821368952148ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__op_side = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10056844600274414895ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__is_bid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 882622167722400876ull);
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_tob__DOT__cur[__Vi0] = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 10465023282802376758ull);
    }
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_tob__DOT__after_rm[__Vi0] = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 12412528482612783459ull);
    }
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__rm_found = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5819673297095752916ull);
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_tob__DOT__after_add[__Vi0] = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1246117782347114983ull);
    }
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_match_found = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2728501819030077116ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__add_inserted = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13324675521716946743ull);
    vlSelf->hft_pipeline_top__DOT__u_tob__DOT__better_than = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 17111860729451958012ull);
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
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT____Vlvbound_h3a018198__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT____Vlvbound_hde47aab4__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT____Vlvbound_hc446bf0d__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT____Vlvbound_he88251c8__0 = 0;
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17944168761605318133ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7413844174575655535ull);
    VL_SCOPED_RAND_RESET_W(138, vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__in_tob, __VscopeHash, 12628454275022152107ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__wt_wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11398555423431812395ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__wt_wr_addr = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 18026757596604078093ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__wt_wr_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13073243708255705541ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__threshold = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 13097775230643307880ull);
    VL_SCOPED_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__out_trade, __VscopeHash, 15981657913705401432ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__out_index = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 1987104590268801749ull);
    for (int __Vi0 = 0; __Vi0 < 500; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__weight_tbl[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 988222522684050437ull);
    }
    for (int __Vi0 = 0; __Vi0 < 500; ++__Vi0) {
        vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__old_mid[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9454932180416681770ull);
    }
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__index_accum = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 11012833280957263184ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__actual_mid = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18310865415578230622ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10287262834636926351ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_is_index = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12791193954535017163ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_is_component = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11531936942019379163ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 10062834529276553058ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_new_mid = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1726349158800067522ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_delta = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 17615979697670118411ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_weight = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2671836592308037065ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s0_two_sided = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4378042341478916073ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15475684737419886631ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_is_index = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 961975401811442132ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_is_component = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17613038252347445178ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_sym = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 4834538126270428201ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_new_mid = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13037511063504391723ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_delta = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 14415756606270415775ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__s1_weight = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15598013880901884184ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__weighted_delta = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 954870943793371108ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__next_accum = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8258499997549421948ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__next_actual = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 845012206694965711ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__actual_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 17770569731898499369ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__spread = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 13526652802589909196ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__abs_spread = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 2753793234898754792ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__trade_fire = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 502286640148425730ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__trade_dir = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5827765631165945936ull);
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__unnamedblk1__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_index_arb__DOT__unnamedblk2__DOT__i = 0;
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9235571601146018982ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6042970718416617565ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__enable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1124876931698339890ull);
    VL_SCOPED_RAND_RESET_W(162, vlSelf->hft_pipeline_top__DOT__u_tx__DOT__in_trade, __VscopeHash, 10601394691595925656ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tdata = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 17425185533673575415ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tkeep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 415293091900025322ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13125723405520551404ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8357219366758954038ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__m_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5975634557291532709ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__order_count = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17305606287509727117ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__net_position = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17972756696965800514ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__pkt_sent = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16164126592337039405ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__state_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10254908833594984539ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_r = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15964987133595753444ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_dir = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 287874875262375705ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_price = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17632133366884709253ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_spread = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 17924877422149423613ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__lat_oid = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5918206301260471988ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__cd_cnt = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3550683685134095192ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__cd_ok = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3797813001417497539ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__next_pos = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8287245753703733993ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__pos_ok = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6301309254336521026ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__oid_seq = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 11171551939756744801ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_data = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 17341012796327316336ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_keep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5062733564310166777ull);
    vlSelf->hft_pipeline_top__DOT__u_tx__DOT__beat_last = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4469419274845230378ull);
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5__key = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5__way = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5____Vlvbound_hcf4b30f4__0 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5__seed_k = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5__mixed = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5__folded = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__5__rot = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__key = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_hcf4b30f4__0 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__rot = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__key = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_hcf4b30f4__0 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__rot = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__key = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__way = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9____Vlvbound_hcf4b30f4__0 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__seed_k = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__mixed = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__folded = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__9__rot = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__key = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__way = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10____Vlvbound_hcf4b30f4__0 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__seed_k = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__mixed = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__folded = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__10__rot = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__14__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__14__b0 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__14__b1 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b0 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b1 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b2 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b3 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b4 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b5 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b6 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__15__b7 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b2 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b3 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b4 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b5 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b6 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__16__b7 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b2 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b3 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b4 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__17__b5 = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__18__Vfuncout = 0;
    vlSelf->__Vfunc_hft_pipeline_top__DOT__u_tx__DOT__pk__18__b4 = 0;
    VL_ZERO_RESET_W(130, vlSelf->__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0);
    vlSelf->__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0 = 0;
    VL_ZERO_RESET_W(130, vlSelf->__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1);
    vlSelf->__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1 = 0;
    VL_ZERO_RESET_W(130, vlSelf->__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2);
    vlSelf->__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2 = 0;
    VL_ZERO_RESET_W(130, vlSelf->__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3);
    vlSelf->__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_filter__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_framer__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_demux__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_order_lookup__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_order_lookup__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_tob__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_index_arb__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__hft_pipeline_top__DOT__u_tx__DOT__clk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
