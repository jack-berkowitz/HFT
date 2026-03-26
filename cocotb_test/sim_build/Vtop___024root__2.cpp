// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

void Vtop___024root___nba_sequent__TOP__7(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__7\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__rst_n) {
        if (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred) {
            if ((1U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                if ((2U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto 
                        = (0x000000ffU & (IData)((vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                  >> 0x38U)));
                }
                if ((2U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                    if ((4U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport 
                            = ((0x0000ff00U & ((IData)(
                                                       (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                        >> 0x20U)) 
                                               << 8U)) 
                               | (0x000000ffU & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                         >> 0x28U))));
                    }
                }
            }
            if ((1U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype 
                    = ((0x0000ff00U & ((IData)((vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                >> 0x20U)) 
                                       << 8U)) | (0x000000ffU 
                                                  & (IData)(
                                                            (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                             >> 0x28U))));
            }
        }
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state 
            = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state_next;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count 
            = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count_next;
    } else {
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count = 0U;
    }
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto_ok 
        = (0x11U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto));
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport_ok 
        = (0x2b5dU == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport));
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype_ok 
        = (0x0800U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype));
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tvalid 
        = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid) 
           & (2U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state)));
    vlSelfRef.hft_pipeline_top__DOT__f2f_tvalid = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tvalid;
    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid 
        = vlSelfRef.hft_pipeline_top__DOT__f2f_tvalid;
}

void Vtop___024root___nba_sequent__TOP__8(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__8\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    QData/*63:0*/ __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0;
    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0 = 0;
    CData/*1:0*/ __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0;
    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0 = 0;
    CData/*4:0*/ __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0;
    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0 = 0;
    QData/*63:0*/ __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0;
    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0 = 0;
    CData/*1:0*/ __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0;
    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0 = 0;
    CData/*4:0*/ __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0;
    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0 = 0;
    IData/*31:0*/ __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1;
    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 = 0;
    CData/*1:0*/ __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1;
    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 = 0;
    CData/*4:0*/ __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1;
    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 = 0;
    QData/*63:0*/ __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1;
    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 = 0;
    IData/*31:0*/ __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2;
    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 = 0;
    CData/*1:0*/ __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2;
    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 = 0;
    CData/*4:0*/ __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2;
    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 = 0;
    QData/*63:0*/ __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2;
    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 = 0;
    IData/*31:0*/ __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1;
    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 = 0;
    CData/*1:0*/ __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1;
    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 = 0;
    CData/*4:0*/ __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1;
    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 = 0;
    QData/*63:0*/ __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1;
    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 = 0;
    IData/*31:0*/ __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2;
    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 = 0;
    CData/*1:0*/ __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2;
    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 = 0;
    CData/*4:0*/ __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2;
    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 = 0;
    QData/*63:0*/ __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2;
    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 = 0;
    // Body
    if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rst_n) {
        if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym_valid) {
            if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__is_bid) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l = 0U;
                while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l)) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h247cbf38__0 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add
                        [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l))
                           ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l)
                           : 0U)];
                    if (VL_LIKELY((((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l)) 
                                    && (0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)))))) {
                        __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0 
                            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h247cbf38__0;
                        __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0 
                            = (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l);
                        __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0 
                            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym;
                        vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__bid_book.enqueue(__VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0, 0xffffffffffffffffULL, (IData)(__VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0), __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v0);
                    }
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l 
                        = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk12__DOT__l);
                }
            } else {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l = 0U;
                while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l)) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h0f0b0d31__0 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add
                        [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l))
                           ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l)
                           : 0U)];
                    if (VL_LIKELY((((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l)) 
                                    && (0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)))))) {
                        __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0 
                            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h0f0b0d31__0;
                        __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0 
                            = (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l);
                        __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0 
                            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym;
                        vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__ask_book.enqueue(__VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0, 0xffffffffffffffffULL, (IData)(__VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0), __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v0);
                    }
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l 
                        = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk13__DOT__l);
                }
            }
        }
    } else {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s = 0U;
        while (VL_GTS_III(32, 0x00000016U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s)) {
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l = 0U;
            while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l)) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_haf3acc64__0 = 0U;
                if (VL_LIKELY((((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l)) 
                                && (0x15U >= (0x0000001fU 
                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s)))))) {
                    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_haf3acc64__0;
                    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 
                        = (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l);
                    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 
                        = (0x0000001fU & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s);
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 = 0ULL;
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1 
                        = ((0x00000000ffffffffULL & __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1) 
                           | ((QData)((IData)(__VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1)) 
                              << 0x00000020U));
                    vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__bid_book.enqueue(__VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1, 0xffffffff00000000ULL, (IData)(__VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1), __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v1);
                }
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_haf3ac47c__0 = 0U;
                if (VL_LIKELY((((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l)) 
                                && (0x15U >= (0x0000001fU 
                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s)))))) {
                    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_haf3ac47c__0;
                    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 
                        = (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l);
                    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 
                        = (0x0000001fU & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s);
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 = 0ULL;
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2 
                        = ((0xffffffff00000000ULL & __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2) 
                           | (IData)((IData)(__VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2)));
                    vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__bid_book.enqueue(__VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2, 0x00000000ffffffffULL, (IData)(__VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2), __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__bid_book__v2);
                }
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hf30588e6__0 = 0U;
                if (VL_LIKELY((((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l)) 
                                && (0x15U >= (0x0000001fU 
                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s)))))) {
                    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hf30588e6__0;
                    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 
                        = (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l);
                    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 
                        = (0x0000001fU & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s);
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 = 0ULL;
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1 
                        = ((0x00000000ffffffffULL & __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1) 
                           | ((QData)((IData)(__VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1)) 
                              << 0x00000020U));
                    vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__ask_book.enqueue(__VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1, 0xffffffff00000000ULL, (IData)(__VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1), __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v1);
                }
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hf307708e__0 = 0U;
                if (VL_LIKELY((((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l)) 
                                && (0x15U >= (0x0000001fU 
                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s)))))) {
                    __VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hf307708e__0;
                    __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 
                        = (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l);
                    __VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 
                        = (0x0000001fU & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s);
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 = 0ULL;
                    __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2 
                        = ((0xffffffff00000000ULL & __VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2) 
                           | (IData)((IData)(__VdlyVal__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2)));
                    vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__ask_book.enqueue(__VdlyElem__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2, 0x00000000ffffffffULL, (IData)(__VdlyDim1__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2), __VdlyDim0__hft_pipeline_top__DOT__u_tob__DOT__ask_book__v2);
                }
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l 
                    = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__unnamedblk11__DOT__l);
            }
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s 
                = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk10__DOT__s);
        }
    }
    if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rst_n) {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[4U] 
            = (0x000003ffU & (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym_valid) 
                               << 9U) | (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)));
        if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym_valid) {
            if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__is_bid) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[2U] 
                    = (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[0U]);
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[3U] 
                    = (IData)((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[0U] 
                               >> 0x00000020U));
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[1U] 
                    = (IData)((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__ask_book
                               [((0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym))
                                  ? (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)
                                  : 0U)][0U] >> 0x00000020U));
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[0U] 
                    = (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__ask_book
                              [((0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym))
                                 ? (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)
                                 : 0U)][0U]);
            } else {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[3U] 
                    = (IData)((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__bid_book
                               [((0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym))
                                  ? (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)
                                  : 0U)][0U] >> 0x00000020U));
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[2U] 
                    = (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__bid_book
                              [((0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym))
                                 ? (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)
                                 : 0U)][0U]);
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[0U] 
                    = (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[0U]);
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[1U] 
                    = (IData)((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[0U] 
                               >> 0x00000020U));
            }
        }
    } else {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[0U] = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[1U] = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[2U] = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[3U] = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[4U] = 0U;
    }
    vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__ask_book.commit(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__ask_book);
    vlSelfRef.__VdlyCommitQueuehft_pipeline_top__DOT__u_tob__DOT__bid_book.commit(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__bid_book);
    vlSelfRef.hft_pipeline_top__DOT__tob_out[0U] = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[0U];
    vlSelfRef.hft_pipeline_top__DOT__tob_out[1U] = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[1U];
    vlSelfRef.hft_pipeline_top__DOT__tob_out[2U] = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[2U];
    vlSelfRef.hft_pipeline_top__DOT__tob_out[3U] = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[3U];
    vlSelfRef.hft_pipeline_top__DOT__tob_out[4U] = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__out_tob[4U];
    vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[0U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[0U];
    vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[1U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[1U];
    vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[2U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[2U];
    vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[3U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[3U];
    vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[4U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[4U];
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[0U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[0U];
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[1U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[1U];
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[2U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[2U];
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[3U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[3U];
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[4U] 
        = vlSelfRef.hft_pipeline_top__DOT__tob_out[4U];
    vlSelfRef.tob_snapshot[0U] = vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[0U];
    vlSelfRef.tob_snapshot[1U] = vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[1U];
    vlSelfRef.tob_snapshot[2U] = vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[2U];
    vlSelfRef.tob_snapshot[3U] = vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[3U];
    vlSelfRef.tob_snapshot[4U] = vlSelfRef.hft_pipeline_top__DOT__tob_snapshot[4U];
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_valid 
        = (1U & (vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[4U] 
                 >> 9U));
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_two_sided 
        = ((0U != vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[2U]) 
           & (0U != vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[0U]));
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_is_index 
        = (0x0014U == (0x000001ffU & vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[4U]));
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_is_component 
        = ((((vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[4U] 
              >> 9U) & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_is_index))) 
            & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_two_sided)) 
           & (0x0014U > (0x000001ffU & vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[4U])));
}

void Vtop___024root___nba_sequent__TOP__9(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__9\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[0U] 
        = vlSelfRef.hft_pipeline_top__DOT__arb_trade[0U];
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[1U] 
        = vlSelfRef.hft_pipeline_top__DOT__arb_trade[1U];
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[2U] 
        = vlSelfRef.hft_pipeline_top__DOT__arb_trade[2U];
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[3U] 
        = vlSelfRef.hft_pipeline_top__DOT__arb_trade[3U];
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[4U] 
        = vlSelfRef.hft_pipeline_top__DOT__arb_trade[4U];
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[5U] 
        = vlSelfRef.hft_pipeline_top__DOT__arb_trade[5U];
}

void Vtop___024root___nba_comb__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready = 1U;
    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready 
        = (1U & ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))
                  ? ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) 
                     && ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r))) 
                         && (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tready)))
                  : (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))));
    vlSelfRef.hft_pipeline_top__DOT__f2f_tready = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready;
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tready 
        = vlSelfRef.hft_pipeline_top__DOT__f2f_tready;
}

void Vtop___024root___nba_sequent__TOP__10(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__10\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_hit = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value[0U] = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value[1U] = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value[2U] = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w = 0U;
    while (VL_GTS_III(32, 4U, vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)) {
        if (((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a
              [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)][4U] 
              >> 1U) & ((((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a
                                          [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)][4U])) 
                          << 0x0000003fU) | (((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a
                                                              [
                                                              (3U 
                                                               & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)][3U])) 
                                              << 0x0000001fU) 
                                             | ((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a
                                                                [
                                                                (3U 
                                                                 & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)][2U])) 
                                                >> 1U))) 
                        == vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_key))) {
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_hit = 1U;
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value[0U] 
                = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a
                [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)][0U];
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value[1U] 
                = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a
                [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)][1U];
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__s1_value[2U] 
                = (1U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_a
                   [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w)][2U]);
        }
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w 
            = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk4__DOT__w);
    }
    if (vlSelfRef.__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[0U][0U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0[0U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[0U][1U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0[1U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[0U][2U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0[2U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[0U][3U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0[3U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[0U][4U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v0[4U];
    }
    if (vlSelfRef.__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[1U][0U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1[0U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[1U][1U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1[1U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[1U][2U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1[2U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[1U][3U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1[3U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[1U][4U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v1[4U];
    }
    if (vlSelfRef.__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[2U][0U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2[0U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[2U][1U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2[1U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[2U][2U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2[2U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[2U][3U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2[3U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[2U][4U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v2[4U];
    }
    if (vlSelfRef.__VdlySet__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[3U][0U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3[0U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[3U][1U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3[1U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[3U][2U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3[2U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[3U][3U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3[3U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b[3U][4U] 
            = vlSelfRef.__VdlyVal__hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b__v3[4U];
    }
}

void Vtop___024root___nba_sequent__TOP__11(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__11\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_done 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_done;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_fail 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_fail;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_done 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_done;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_not_found 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_not_found;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_hit 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_hit;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[0U] 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value[0U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[1U] 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value[1U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[2U] 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value[2U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_ready 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_ready;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_ready 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_ready;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ready 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__ready;
}

void Vtop___024root___nba_sequent__TOP__12(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__12\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[0U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[0U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[1U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[1U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[2U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[2U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[3U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[3U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[4U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[4U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[5U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[5U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[6U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[6U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[7U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[7U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[8U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[8U];
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[9U] 
        = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[9U];
}

void Vtop___024root___nba_comb__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid = 0U;
    if ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
        if ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r)))) {
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast 
                = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r) 
                   || (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                        & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast)) 
                       & (~ (0U != (0x0000003fU & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep) 
                                                   >> 2U))))));
            if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep 
                    = VL_SHIFTR_III(8,8,32, (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r), 2U);
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid 
                    = (0U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep));
            } else {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep 
                    = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__combined_keep;
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid 
                    = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid;
            }
        }
    }
    vlSelfRef.hft_pipeline_top__DOT__f2d_tlast = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast;
    vlSelfRef.hft_pipeline_top__DOT__f2d_tkeep = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep;
    vlSelfRef.hft_pipeline_top__DOT__f2d_tvalid = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid;
    vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tlast 
        = vlSelfRef.hft_pipeline_top__DOT__f2d_tlast;
    vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tkeep 
        = vlSelfRef.hft_pipeline_top__DOT__f2d_tkeep;
    vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid 
        = vlSelfRef.hft_pipeline_top__DOT__f2d_tvalid;
}

void Vtop___024root___nba_sequent__TOP__13(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__13\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym 
        = (0x0000001fU & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[11U] 
                          >> 0x0000000dU));
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym_valid 
        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[12U] 
            >> 0x00000011U) & (0x00000016U > ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[12U] 
                                               << 0x00000013U) 
                                              | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[11U] 
                                                 >> 0x0000000dU))));
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_add = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_qty = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_remove = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_price = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_qty = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__op_side = 0U;
    if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym_valid) {
        if ((4U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type))) {
            if ((1U & (~ ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type)))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_add = 1U;
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_qty 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[5U] 
                            << 0x00000013U) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[4U] 
                                               >> 0x0000000dU));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[6U] 
                            << 0x00000013U) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[5U] 
                                               >> 0x0000000dU));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_remove = 1U;
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_price 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[2U] 
                            << 0x0000001cU) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[1U] 
                                               >> 4U));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_qty 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[1U] 
                            << 0x0000001cU) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[0U] 
                                               >> 4U));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__op_side 
                        = (1U & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[0U] 
                                 >> 3U));
                }
            }
        } else {
            if ((1U & (~ ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type) 
                          >> 1U)))) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_add = 1U;
                if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_qty 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[5U] 
                            << 0x00000013U) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[4U] 
                                               >> 0x0000000dU));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[6U] 
                            << 0x00000013U) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[5U] 
                                               >> 0x0000000dU));
                } else {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_qty 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[5U] 
                            << 0x00000013U) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[4U] 
                                               >> 0x0000000dU));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[6U] 
                            << 0x00000013U) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[5U] 
                                               >> 0x0000000dU));
                }
            }
            if ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type))) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_remove = 1U;
                if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_price 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[2U] 
                            << 0x0000001cU) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[1U] 
                                               >> 4U));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_qty 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[5U] 
                            << 0x00000013U) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[4U] 
                                               >> 0x0000000dU));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__op_side 
                        = (1U & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[0U] 
                                 >> 3U));
                } else {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_price 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[2U] 
                            << 0x0000001cU) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[1U] 
                                               >> 4U));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_qty 
                        = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[1U] 
                            << 0x0000001cU) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[0U] 
                                               >> 4U));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__op_side 
                        = (1U & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[0U] 
                                 >> 3U));
                }
            } else if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__msg_type))) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_remove = 1U;
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_price 
                    = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[2U] 
                        << 0x0000001cU) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[1U] 
                                           >> 4U));
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_qty 
                    = ((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[1U] 
                        << 0x0000001cU) | (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[0U] 
                                           >> 4U));
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__op_side 
                    = (1U & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[0U] 
                             >> 3U));
            } else {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__op_side 
                    = (1U & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__in_update[3U] 
                             >> 0x0000000cU));
            }
        }
    }
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__is_bid 
        = (1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__op_side)));
}

void Vtop___024root___nba_comb__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym 
        = (0x000001ffU & vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[4U]);
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_new_mid 
        = VL_SHIFTR_III(32,32,32, (vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[3U] 
                                   + vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[1U]), 1U);
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_delta 
        = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_new_mid)) 
                                    - (QData)((IData)(
                                                      ((0x13U 
                                                        >= 
                                                        (0x0000001fU 
                                                         & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym)))
                                                        ? vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__old_mid
                                                       [
                                                       (0x0000001fU 
                                                        & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym))]
                                                        : 0U)))));
    vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_weight 
        = ((0x13U >= (0x0000001fU & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym)))
            ? vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__weight_tbl
           [(0x0000001fU & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym))]
            : 0U);
}

void Vtop___024root___nba_comb__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__next_pos 
        = (IData)((0x00000001ffffffffULL & ((1U & vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[5U])
                                             ? (VL_EXTENDS_QI(33,32, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__net_position) 
                                                - 0x0000000000000064ULL)
                                             : (0x0000000000000064ULL 
                                                + VL_EXTENDS_QI(33,32, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__net_position)))));
    vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__pos_ok 
        = (VL_LTES_III(32, 0xffffd8f0U, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__next_pos) 
           & VL_GTES_III(32, 0x00002710U, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__next_pos));
}

extern const VlUnpacked<CData/*1:0*/, 32> Vtop__ConstPool__TABLE_h585bf363_0;
extern const VlUnpacked<CData/*1:0*/, 1024> Vtop__ConstPool__TABLE_hcf5788b1_0;

void Vtop___024root___nba_comb__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*9:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*4:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready 
        = ((2U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state)) 
           | (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tready));
    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next 
        = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r;
    if ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
        if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
            if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 0U;
            }
        } else if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r) {
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next 
                = ((0U != (0x0000003fU & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r) 
                                          >> 2U))) ? 
                   (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid) 
                     & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tready))
                     ? 3U : 2U) : 3U);
        } else if ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                     & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready)) 
                    & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast))) {
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next 
                = ((0U != (0x0000003fU & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep) 
                                          >> 2U))) ? 2U
                    : 3U);
        }
    } else if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 2U;
    } else if (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready))) {
        if ((1U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r))) {
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 1U;
        } else if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast) {
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 3U;
        }
    }
    vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__beat_transferred 
        = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tready) 
           & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid));
    vlSelfRef.hft_pipeline_top__DOT__rx_axis_tready 
        = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready;
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred 
        = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready) 
           & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid));
    __Vtableidx2 = (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tlast) 
                     << 4U) | (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__beat_transferred) 
                                << 3U) | (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid) 
                                           << 2U) | (IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__state_r))));
    vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__state_next 
        = Vtop__ConstPool__TABLE_h585bf363_0[__Vtableidx2];
    vlSelfRef.rx_axis_tready = vlSelfRef.hft_pipeline_top__DOT__rx_axis_tready;
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count_next 
        = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count;
    if (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred) {
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count_next 
            = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tlast)
                ? 0U : (7U & ((IData)(1U) + (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))));
    }
    __Vtableidx1 = ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid) 
                      << 9U) | ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype_ok) 
                                  & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto_ok) 
                                     & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport_ok)))
                                  ? 2U : 3U) << 7U)) 
                    | ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count) 
                         << 4U) | ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tlast) 
                                   << 3U)) | (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred) 
                                               << 2U) 
                                              | (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state))));
    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state_next 
        = Vtop__ConstPool__TABLE_hcf5788b1_0[__Vtableidx1];
}

void Vtop___024root___nba_comb__TOP__5(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__5\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*8:0*/ __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__Vfuncout;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__Vfuncout = 0;
    QData/*63:0*/ __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__key;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__key = 0;
    CData/*0:0*/ __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 = 0;
    QData/*63:0*/ __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed = 0;
    SData/*8:0*/ __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded = 0;
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_slot_empty 
        = (1U & (~ (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b
                    [vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__evict_dst_way_r][4U] 
                    >> 1U)));
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_existing = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_empty = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_way = 0U;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__key 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_key_r;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
        = (0x9e3779b97f4a7c15ULL ^ __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__key);
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
        = (VL_SHIFTL_QQI(64,64,32, __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed, 3U) 
           | VL_SHIFTR_QQI(64,64,32, __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed, 0x0000003dU));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded = 0U;
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 1U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 1U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fdU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 1U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 2U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 2U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fbU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 2U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 3U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 3U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01f7U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 3U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 4U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 4U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01efU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 4U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 5U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 5U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01dfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 5U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 6U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 6U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01bfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 6U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 7U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 7U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x017fU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 7U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 8U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 8U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x00ffU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 8U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                 ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                            >> 9U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 1U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x0aU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fdU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 1U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 2U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x0bU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fbU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 2U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 3U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x0cU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01f7U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 3U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 4U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x0dU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01efU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 4U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 5U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x0eU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01dfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 5U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 6U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x0fU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01bfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 6U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 7U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x10U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x017fU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 7U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 8U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x11U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x00ffU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 8U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                 ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                            >> 0x12U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 1U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x13U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fdU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 1U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 2U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x14U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fbU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 2U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 3U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x15U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01f7U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 3U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 4U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x16U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01efU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 4U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 5U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x17U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01dfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 5U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 6U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x18U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01bfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 6U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 7U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x19U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x017fU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 7U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 8U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x1aU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x00ffU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 8U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                 ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                            >> 0x1bU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 1U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x1cU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fdU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 1U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 2U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x1dU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fbU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 2U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 3U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x1eU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01f7U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 3U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 4U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x1fU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01efU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 4U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 5U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x20U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01dfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 5U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 6U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x21U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01bfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 6U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 7U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x22U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x017fU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 7U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 8U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x23U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x00ffU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 8U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                 ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                            >> 0x24U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 1U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x25U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fdU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 1U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 2U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x26U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fbU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 2U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 3U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x27U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01f7U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 3U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 4U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x28U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01efU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 4U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 5U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x29U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01dfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 5U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 6U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x2aU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01bfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 6U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 7U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x2bU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x017fU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 7U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 8U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x2cU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x00ffU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 8U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                 ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                            >> 0x2dU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 1U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x2eU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fdU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 1U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 2U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x2fU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fbU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 2U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 3U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x30U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01f7U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 3U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 4U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x31U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01efU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 4U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 5U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x32U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01dfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 5U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 6U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x33U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01bfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 6U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 7U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x34U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x017fU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 7U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 8U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x35U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x00ffU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 8U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                 ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                            >> 0x36U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 1U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x37U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fdU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 1U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 2U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x38U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01fbU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 2U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 3U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x39U))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01f7U & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 3U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 4U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x3aU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01efU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 4U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 5U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x3bU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01dfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 5U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 6U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x3cU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01bfU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 6U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 7U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x3dU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x017fU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 7U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & (((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                  >> 8U) ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                                    >> 0x3eU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x00ffU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0) 
              << 8U));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0 
        = (1U & ((IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded) 
                 ^ (IData)((__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__mixed 
                            >> 0x3fU))));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded 
        = ((0x01feU & (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded)) 
           | (IData)(__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6____Vlvbound_h61001783__0));
    __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__Vfuncout 
        = __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__folded;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_idx 
        = __Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__6__Vfuncout;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w = 0U;
    while (VL_GTS_III(32, 4U, vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w)) {
        if (((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b
              [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w)][4U] 
              >> 1U) & ((((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b
                                          [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w)][4U])) 
                          << 0x0000003fU) | (((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b
                                                              [
                                                              (3U 
                                                               & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w)][3U])) 
                                              << 0x0000001fU) 
                                             | ((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b
                                                                [
                                                                (3U 
                                                                 & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w)][2U])) 
                                                >> 1U))) 
                        == vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_key_r))) {
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_existing = 1U;
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_way 
                = (3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w);
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way 
                = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w;
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__key 
                = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_key_r;
            vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 = 0;
            vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__Vfuncout = 0;
            vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k = 0;
            vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed = 0;
            vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded = 0;
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__rot = 0U;
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffffffeULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | (IData)((IData)((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                   [
                                                   (7U 
                                                    & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)])))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffffffdULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 1U))))) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffffffbULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 2U))))) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffffff7ULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 3U))))) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffffffefULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 4U))))) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffffffdfULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 5U))))) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffffffbfULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 6U))))) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffffff7fULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 7U))))) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffffeffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 8U))))) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffffdffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 9U))))) 
                      << 9U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffffbffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x0aU))))) 
                      << 0x0000000aU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffff7ffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x0bU))))) 
                      << 0x0000000bU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffffefffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x0cU))))) 
                      << 0x0000000cU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffffdfffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x0dU))))) 
                      << 0x0000000dU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffffbfffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x0eU))))) 
                      << 0x0000000eU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffff7fffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x0fU))))) 
                      << 0x0000000fU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffeffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x10U))))) 
                      << 0x00000010U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffdffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x11U))))) 
                      << 0x00000011U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffffbffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x12U))))) 
                      << 0x00000012U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffff7ffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x13U))))) 
                      << 0x00000013U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffefffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x14U))))) 
                      << 0x00000014U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffdfffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x15U))))) 
                      << 0x00000015U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffffbfffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x16U))))) 
                      << 0x00000016U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffff7fffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x17U))))) 
                      << 0x00000017U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffeffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x18U))))) 
                      << 0x00000018U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffdffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x19U))))) 
                      << 0x00000019U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffffbffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x1aU))))) 
                      << 0x0000001aU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffff7ffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x1bU))))) 
                      << 0x0000001bU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffefffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x1cU))))) 
                      << 0x0000001cU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffdfffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x1dU))))) 
                      << 0x0000001dU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffffbfffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x1eU))))) 
                      << 0x0000001eU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffff7fffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x1fU))))) 
                      << 0x0000001fU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffeffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x20U))))) 
                      << 0x00000020U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffdffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x21U))))) 
                      << 0x00000021U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffffbffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x22U))))) 
                      << 0x00000022U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffff7ffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x23U))))) 
                      << 0x00000023U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffefffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x24U))))) 
                      << 0x00000024U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffdfffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x25U))))) 
                      << 0x00000025U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffffbfffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x26U))))) 
                      << 0x00000026U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffff7fffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x27U))))) 
                      << 0x00000027U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffeffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x28U))))) 
                      << 0x00000028U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffdffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x29U))))) 
                      << 0x00000029U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffffbffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x2aU))))) 
                      << 0x0000002aU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffff7ffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x2bU))))) 
                      << 0x0000002bU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffefffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x2cU))))) 
                      << 0x0000002cU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffdfffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x2dU))))) 
                      << 0x0000002dU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffffbfffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x2eU))))) 
                      << 0x0000002eU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffff7fffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x2fU))))) 
                      << 0x0000002fU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffeffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x30U))))) 
                      << 0x00000030U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffdffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x31U))))) 
                      << 0x00000031U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfffbffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x32U))))) 
                      << 0x00000032U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfff7ffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x33U))))) 
                      << 0x00000033U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffefffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x34U))))) 
                      << 0x00000034U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffdfffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x35U))))) 
                      << 0x00000035U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xffbfffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x36U))))) 
                      << 0x00000036U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xff7fffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x37U))))) 
                      << 0x00000037U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfeffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x38U))))) 
                      << 0x00000038U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfdffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x39U))))) 
                      << 0x00000039U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xfbffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x3aU))))) 
                      << 0x0000003aU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xf7ffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x3bU))))) 
                      << 0x0000003bU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xefffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x3cU))))) 
                      << 0x0000003cU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xdfffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x3dU))))) 
                      << 0x0000003dU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0xbfffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x3eU))))) 
                      << 0x0000003eU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k 
                = ((0x7fffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k) 
                   | ((QData)((IData)((1U & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                     [
                                                     (7U 
                                                      & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)] 
                                                     >> 0x3fU))))) 
                      << 0x0000003fU));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                = (vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__key 
                   ^ vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__seed_k);
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__rot 
                = VL_MODDIVS_III(32, vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_ROT
                                 [(7U & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__way)], (IData)(0x00000040U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                = (VL_SHIFTL_QQI(64,64,32, vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed, vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__rot) 
                   | VL_SHIFTR_QQI(64,64,32, vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed, 
                                   ((IData)(0x00000040U) 
                                    - vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__rot)));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded = 0U;
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 1U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 2U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 3U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 4U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 5U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 6U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 7U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 8U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                         ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                    >> 9U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x0aU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x0bU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x0cU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x0dU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x0eU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x0fU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x10U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x11U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                         ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                    >> 0x12U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x13U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x14U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x15U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x16U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x17U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x18U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x19U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x1aU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                         ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                    >> 0x1bU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x1cU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x1dU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x1eU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x1fU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x20U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x21U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x22U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x23U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                         ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                    >> 0x24U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x25U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x26U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x27U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x28U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x29U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x2aU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x2bU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x2cU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                         ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                    >> 0x2dU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x2eU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x2fU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x30U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x31U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x32U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x33U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x34U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x35U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                         ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                    >> 0x36U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x37U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 1U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x38U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 2U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x39U))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 3U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x3aU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 4U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x3bU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 5U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x3cU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 6U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x3dU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 7U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                          >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                            >> 0x3eU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0) 
                      << 8U));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0 
                = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded) 
                         ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__mixed 
                                    >> 0x3fU))));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded 
                = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded)) 
                   | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7____Vlvbound_h61001783__0));
            vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__Vfuncout 
                = vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__folded;
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_idx 
                = vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__7__Vfuncout;
        }
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w 
            = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk5__DOT__w);
    }
    if ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_existing)))) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w = 0U;
        while (VL_GTS_III(32, 4U, vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w)) {
            if ((1U & ((~ (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__bram_rdata_b
                           [(3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w)][4U] 
                           >> 1U)) & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_empty))))) {
                vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_found_empty = 1U;
                vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_way 
                    = (3U & vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w);
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way 
                    = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w;
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__key 
                    = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__op_key_r;
                vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 = 0;
                vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__Vfuncout = 0;
                vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k = 0;
                vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed = 0;
                vlSelf->__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded = 0;
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__rot = 0U;
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffffffeULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | (IData)((IData)((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                       [
                                                       (7U 
                                                        & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)])))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffffffdULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 1U))))) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffffffbULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 2U))))) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffffff7ULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 3U))))) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffffffefULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 4U))))) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffffffdfULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 5U))))) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffffffbfULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 6U))))) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffffff7fULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 7U))))) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffffeffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 8U))))) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffffdffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 9U))))) 
                          << 9U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffffbffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x0aU))))) 
                          << 0x0000000aU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffff7ffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x0bU))))) 
                          << 0x0000000bU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffffefffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x0cU))))) 
                          << 0x0000000cU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffffdfffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x0dU))))) 
                          << 0x0000000dU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffffbfffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x0eU))))) 
                          << 0x0000000eU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffff7fffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x0fU))))) 
                          << 0x0000000fU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffeffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x10U))))) 
                          << 0x00000010U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffdffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x11U))))) 
                          << 0x00000011U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffffbffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x12U))))) 
                          << 0x00000012U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffff7ffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x13U))))) 
                          << 0x00000013U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffefffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x14U))))) 
                          << 0x00000014U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffdfffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x15U))))) 
                          << 0x00000015U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffffbfffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x16U))))) 
                          << 0x00000016U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffff7fffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x17U))))) 
                          << 0x00000017U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffeffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x18U))))) 
                          << 0x00000018U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffdffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x19U))))) 
                          << 0x00000019U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffffbffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x1aU))))) 
                          << 0x0000001aU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffff7ffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x1bU))))) 
                          << 0x0000001bU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffefffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x1cU))))) 
                          << 0x0000001cU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffdfffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x1dU))))) 
                          << 0x0000001dU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffffbfffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x1eU))))) 
                          << 0x0000001eU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffff7fffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x1fU))))) 
                          << 0x0000001fU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffeffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x20U))))) 
                          << 0x00000020U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffdffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x21U))))) 
                          << 0x00000021U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffffbffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x22U))))) 
                          << 0x00000022U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffff7ffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x23U))))) 
                          << 0x00000023U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffefffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x24U))))) 
                          << 0x00000024U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffdfffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x25U))))) 
                          << 0x00000025U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffffbfffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x26U))))) 
                          << 0x00000026U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffff7fffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x27U))))) 
                          << 0x00000027U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffeffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x28U))))) 
                          << 0x00000028U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffdffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x29U))))) 
                          << 0x00000029U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffffbffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x2aU))))) 
                          << 0x0000002aU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffff7ffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x2bU))))) 
                          << 0x0000002bU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffefffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x2cU))))) 
                          << 0x0000002cU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffdfffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x2dU))))) 
                          << 0x0000002dU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffffbfffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x2eU))))) 
                          << 0x0000002eU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffff7fffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x2fU))))) 
                          << 0x0000002fU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffeffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x30U))))) 
                          << 0x00000030U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffdffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x31U))))) 
                          << 0x00000031U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfffbffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x32U))))) 
                          << 0x00000032U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfff7ffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x33U))))) 
                          << 0x00000033U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffefffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x34U))))) 
                          << 0x00000034U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffdfffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x35U))))) 
                          << 0x00000035U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xffbfffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x36U))))) 
                          << 0x00000036U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xff7fffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x37U))))) 
                          << 0x00000037U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfeffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x38U))))) 
                          << 0x00000038U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfdffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x39U))))) 
                          << 0x00000039U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xfbffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x3aU))))) 
                          << 0x0000003aU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xf7ffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x3bU))))) 
                          << 0x0000003bU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xefffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x3cU))))) 
                          << 0x0000003cU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xdfffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x3dU))))) 
                          << 0x0000003dU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0xbfffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x3eU))))) 
                          << 0x0000003eU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k 
                    = ((0x7fffffffffffffffULL & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k) 
                       | ((QData)((IData)((1U & (IData)(
                                                        (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS
                                                         [
                                                         (7U 
                                                          & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)] 
                                                         >> 0x3fU))))) 
                          << 0x0000003fU));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                    = (vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__key 
                       ^ vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__seed_k);
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__rot 
                    = VL_MODDIVS_III(32, vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_ROT
                                     [(7U & vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__way)], (IData)(0x00000040U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                    = (VL_SHIFTL_QQI(64,64,32, vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed, vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__rot) 
                       | VL_SHIFTR_QQI(64,64,32, vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed, 
                                       ((IData)(0x00000040U) 
                                        - vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__rot)));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded = 0U;
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 1U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 2U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 3U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 4U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 5U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 6U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 7U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 8U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                             ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                        >> 9U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x0aU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x0bU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x0cU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x0dU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x0eU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x0fU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x10U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x11U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                             ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                        >> 0x12U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x13U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x14U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x15U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x16U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x17U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x18U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x19U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x1aU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                             ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                        >> 0x1bU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x1cU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x1dU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x1eU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x1fU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x20U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x21U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x22U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x23U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                             ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                        >> 0x24U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x25U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x26U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x27U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x28U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x29U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x2aU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x2bU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x2cU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                             ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                        >> 0x2dU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x2eU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x2fU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x30U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x31U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x32U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x33U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x34U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x35U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                             ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                        >> 0x36U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 1U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x37U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fdU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 1U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 2U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x38U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01fbU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 2U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 3U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x39U))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01f7U & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 3U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 4U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x3aU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01efU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 4U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 5U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x3bU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01dfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 5U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 6U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x3cU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01bfU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 6U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 7U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x3dU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x017fU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 7U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & (((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                              >> 8U) ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                                >> 0x3eU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x00ffU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0) 
                          << 8U));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0 
                    = (1U & ((IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded) 
                             ^ (IData)((vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__mixed 
                                        >> 0x3fU))));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded 
                    = ((0x01feU & (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded)) 
                       | (IData)(vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8____Vlvbound_h61001783__0));
                vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__Vfuncout 
                    = vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__folded;
                vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__cmp_sel_idx 
                    = vlSelfRef.__Vfunc_hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__hash_fn__8__Vfuncout;
            }
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w 
                = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__unnamedblk6__DOT__w);
        }
    }
}

void Vtop___024root___nba_comb__TOP__6(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__6\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__exec_remaining 
        = ((((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[1U] 
              << 0x0000001fU) | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[0U] 
                                 >> 1U)) > ((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[2U] 
                                             << 0x00000017U) 
                                            | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[1U] 
                                               >> 9U)))
            ? (((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[1U] 
                 << 0x0000001fU) | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[0U] 
                                    >> 1U)) - ((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[2U] 
                                                << 0x00000017U) 
                                               | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[1U] 
                                                  >> 9U)))
            : 0U);
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid = 0U;
    if ((1U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
        if ((4U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
            if ((5U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
                vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid 
                    = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_ready;
            }
        }
        if ((4U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
            vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid 
                = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_ready;
        }
    }
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ready 
        = ((0U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r)) 
           & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ready));
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_valid 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid;
    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_valid 
        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid;
    vlSelfRef.hft_pipeline_top__DOT__ol_ready = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ready;
    vlSelfRef.ol_ready = vlSelfRef.hft_pipeline_top__DOT__ol_ready;
}

void Vtop___024root___nba_comb__TOP__7(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__7\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i = 0U;
    while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i)) {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h99a7bc22__0 
            = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__is_bid)
                ? vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__bid_book
               [((0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym))
                  ? (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)
                  : 0U)][((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i))
                           ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i)
                           : 0U)] : vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__ask_book
               [((0x15U >= (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym))
                  ? (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__sym)
                  : 0U)][((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i))
                           ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i)
                           : 0U)]);
        if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i))))) {
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur[(3U 
                                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i)] 
                = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h99a7bc22__0;
        }
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i 
            = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk1__DOT__i);
    }
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_found = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i = 0U;
    while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i)) {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd32e541e__0 
            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur
            [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i))
               ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i)
               : 0U)];
        if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i))))) {
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[(3U 
                                                                   & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i)] 
                = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd32e541e__0;
        }
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i 
            = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk2__DOT__i);
    }
    if (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_remove) {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i = 0U;
        while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)) {
            if ((((~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_found)) 
                  & (0U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur
                                   [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i))
                                      ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)
                                      : 0U)]))) & ((IData)(
                                                           (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur
                                                            [
                                                            ((2U 
                                                              >= 
                                                              (3U 
                                                               & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i))
                                                              ? 
                                                             (3U 
                                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)
                                                              : 0U)] 
                                                            >> 0x00000020U)) 
                                                   == vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_price))) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_found = 1U;
                if (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur
                             [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i))
                                ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)
                                : 0U)]) <= vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_qty)) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i;
                    while (VL_GTS_III(32, 2U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j)) {
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h332dfccf__0 
                            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur
                            [((2U >= (3U & ((IData)(1U) 
                                            + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j)))
                               ? (3U & ((IData)(1U) 
                                        + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j))
                               : 0U)];
                        if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j))))) {
                            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[(3U 
                                                                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j)] 
                                = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h332dfccf__0;
                        }
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j 
                            = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__unnamedblk4__DOT__j);
                    }
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[2U] 
                        = (0x00000000ffffffffULL & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[2U]);
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[2U] 
                        = (0xffffffff00000000ULL & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[2U]);
                } else {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc27b46c2__0 
                        = (IData)((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur
                                   [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i))
                                      ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)
                                      : 0U)] >> 0x00000020U));
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc27b3eda__0 
                        = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__cur
                                   [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i))
                                      ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)
                                      : 0U)]) - vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__rm_qty);
                    if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i))))) {
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[(3U 
                                                                               & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)] 
                            = ((0x00000000ffffffffULL 
                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                                [(3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)]) 
                               | ((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc27b46c2__0)) 
                                  << 0x00000020U));
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm[(3U 
                                                                               & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)] 
                            = ((0xffffffff00000000ULL 
                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                                [(3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i)]) 
                               | (IData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hc27b3eda__0)));
                    }
                }
            }
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i 
                = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk3__DOT__i);
        }
    }
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i = 0U;
    while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i)) {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h3945d898__0 
            = ((0U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                              [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i))
                                 ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i)
                                 : 0U)])) | ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__is_bid)
                                              ? (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price 
                                                 > (IData)(
                                                           (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                                                            [
                                                            ((2U 
                                                              >= 
                                                              (3U 
                                                               & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i))
                                                              ? 
                                                             (3U 
                                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i)
                                                              : 0U)] 
                                                            >> 0x00000020U)))
                                              : (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price 
                                                 < (IData)(
                                                           (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                                                            [
                                                            ((2U 
                                                              >= 
                                                              (3U 
                                                               & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i))
                                                              ? 
                                                             (3U 
                                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i)
                                                              : 0U)] 
                                                            >> 0x00000020U)))));
        if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i))))) {
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__better_than 
                = (((~ ((IData)(1U) << (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i))) 
                    & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__better_than)) 
                   | (7U & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h3945d898__0) 
                            << (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i))));
        }
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i 
            = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk5__DOT__i);
    }
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_match_found = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_inserted = 0U;
    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i = 0U;
    while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i)) {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd59551cd__0 
            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
            [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i))
               ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i)
               : 0U)];
        if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i))))) {
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[(3U 
                                                                    & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i)] 
                = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd59551cd__0;
        }
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i 
            = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk6__DOT__i);
    }
    if (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__do_add) 
         & (0U != vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_qty))) {
        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i = 0U;
        while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)) {
            if ((((~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_match_found)) 
                  & (0U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                                   [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i))
                                      ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)
                                      : 0U)]))) & ((IData)(
                                                           (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                                                            [
                                                            ((2U 
                                                              >= 
                                                              (3U 
                                                               & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i))
                                                              ? 
                                                             (3U 
                                                              & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)
                                                              : 0U)] 
                                                            >> 0x00000020U)) 
                                                   == vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price))) {
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h104c1625__0 
                    = (IData)((vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                               [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i))
                                  ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)
                                  : 0U)] >> 0x00000020U));
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_match_found = 1U;
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h104c0e2d__0 
                    = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                               [((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i))
                                  ? (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)
                                  : 0U)]) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_qty);
                if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i))))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[(3U 
                                                                            & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)] 
                        = ((0x00000000ffffffffULL & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add
                            [(3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)]) 
                           | ((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h104c1625__0)) 
                              << 0x00000020U));
                }
                if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i))))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[(3U 
                                                                            & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)] 
                        = ((0xffffffff00000000ULL & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add
                            [(3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i)]) 
                           | (IData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h104c0e2d__0)));
                }
            }
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i 
                = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk7__DOT__i);
        }
        if ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_match_found)))) {
            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i = 0U;
            while (VL_GTS_III(32, 3U, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)) {
                if (((~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_inserted)) 
                     & ((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)) 
                        && (1U & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__better_than) 
                                  >> (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)))))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_inserted = 1U;
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j = 2U;
                    while (VL_GTS_III(32, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j, vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)) {
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd5cb8263__0 
                            = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_rm
                            [((2U >= (3U & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j 
                                            - (IData)(1U))))
                               ? (3U & (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j 
                                        - (IData)(1U)))
                               : 0U)];
                        if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j))))) {
                            vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[(3U 
                                                                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j)] 
                                = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_hd5cb8263__0;
                        }
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j 
                            = (vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__unnamedblk9__DOT__j 
                               - (IData)(1U));
                    }
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h81311a8e__0 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_price;
                    vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h813121f6__0 
                        = vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__add_qty;
                    if (VL_LIKELY(((2U >= (3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i))))) {
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[(3U 
                                                                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)] 
                            = ((0x00000000ffffffffULL 
                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add
                                [(3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)]) 
                               | ((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h81311a8e__0)) 
                                  << 0x00000020U));
                        vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add[(3U 
                                                                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)] 
                            = ((0xffffffff00000000ULL 
                                & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__after_add
                                [(3U & vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i)]) 
                               | (IData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT____Vlvbound_h813121f6__0)));
                    }
                }
                vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i 
                    = ((IData)(1U) + vlSelfRef.hft_pipeline_top__DOT__u_tob__DOT__unnamedblk8__DOT__i);
            }
        }
    }
}

void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__1(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__2(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__3(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__5(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__6(Vtop___024root* vlSelf);

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*1:0*/ __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r;
    __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r;
    __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r = 0;
    SData/*9:0*/ __Vinline__nba_comb__TOP__4___Vtableidx1;
    __Vinline__nba_comb__TOP__4___Vtableidx1 = 0;
    CData/*4:0*/ __Vinline__nba_comb__TOP__4___Vtableidx2;
    __Vinline__nba_comb__TOP__4___Vtableidx2 = 0;
    // Body
    if ((0x0000000000000020ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((0x0000000000000100ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((0x0000000000000200ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__3(vlSelf);
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r;
        __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r;
        if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__rst_n) {
            if ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
                if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
                    __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r = 0U;
                    __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r = 0U;
                } else if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r) {
                    if ((3U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next))) {
                        __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r = 0U;
                        __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r = 0U;
                    }
                } else if (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                            & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_data_r 
                        = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tdata;
                    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r 
                        = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep;
                    __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r 
                        = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast;
                }
            } else if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
                __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r = 0U;
            } else if (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                        & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready))) {
                __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r 
                    = (3U & ((IData)(1U) + (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r)));
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_data_r 
                    = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tdata;
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r 
                    = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep;
                __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r 
                    = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast;
            }
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r 
                = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next;
        } else {
            __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r = 0U;
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_data_r = 0ULL;
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r = 0U;
            __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r = 0U;
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r = 3U;
        }
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r 
            = __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r;
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r 
            = __Vinline__nba_sequent__TOP__4___Vdly__hft_pipeline_top__DOT__u_framer__DOT__prev_last_r;
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__combined_data 
            = (((QData)((IData)((0x0000ffffU & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tdata)))) 
                << 0x00000030U) | (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_data_r 
                                   >> 0x00000010U));
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__combined_keep 
            = ((0x000000c0U & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep) 
                               << 6U)) | (0x0000003fU 
                                          & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r) 
                                             >> 2U)));
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tdata = 0ULL;
        if ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
            if ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r)))) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tdata 
                    = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r)
                        ? VL_SHIFTR_QQI(64,64,32, vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_data_r, 0x00000010U)
                        : vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__combined_data);
            }
        }
        vlSelfRef.hft_pipeline_top__DOT__f2d_tdata 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tdata;
        vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tdata 
            = vlSelfRef.hft_pipeline_top__DOT__f2d_tdata;
    }
    if ((0x0000000000000060ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__5(vlSelf);
    }
    if ((0x0000000000000018ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__6(vlSelf);
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__rst_n) {
            if (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred) {
                if ((1U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                    if ((2U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto 
                            = (0x000000ffU & (IData)(
                                                     (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                      >> 0x38U)));
                    }
                    if ((2U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                        if ((4U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport 
                                = ((0x0000ff00U & ((IData)(
                                                           (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                            >> 0x20U)) 
                                                   << 8U)) 
                                   | (0x000000ffU & (IData)(
                                                            (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                             >> 0x28U))));
                        }
                    }
                }
                if ((1U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype 
                        = ((0x0000ff00U & ((IData)(
                                                   (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                    >> 0x20U)) 
                                           << 8U)) 
                           | (0x000000ffU & (IData)(
                                                    (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tdata 
                                                     >> 0x28U))));
                }
            }
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state 
                = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state_next;
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count 
                = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count_next;
        } else {
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto = 0U;
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport = 0U;
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype = 0U;
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state = 0U;
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count = 0U;
        }
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto_ok 
            = (0x11U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto));
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport_ok 
            = (0x2b5dU == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport));
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype_ok 
            = (0x0800U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype));
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tvalid 
            = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid) 
               & (2U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state)));
        vlSelfRef.hft_pipeline_top__DOT__f2f_tvalid 
            = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tvalid;
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid 
            = vlSelfRef.hft_pipeline_top__DOT__f2f_tvalid;
    }
    if ((0x0000000000000080ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__8(vlSelf);
    }
    if ((0x0000000000000100ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[0U] 
            = vlSelfRef.hft_pipeline_top__DOT__arb_trade[0U];
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[1U] 
            = vlSelfRef.hft_pipeline_top__DOT__arb_trade[1U];
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[2U] 
            = vlSelfRef.hft_pipeline_top__DOT__arb_trade[2U];
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[3U] 
            = vlSelfRef.hft_pipeline_top__DOT__arb_trade[3U];
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[4U] 
            = vlSelfRef.hft_pipeline_top__DOT__arb_trade[4U];
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[5U] 
            = vlSelfRef.hft_pipeline_top__DOT__arb_trade[5U];
    }
    if ((6ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready = 1U;
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready 
            = (1U & ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))
                      ? ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) 
                         && ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r))) 
                             && (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tready)))
                      : (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))));
        vlSelfRef.hft_pipeline_top__DOT__f2f_tready 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tready 
            = vlSelfRef.hft_pipeline_top__DOT__f2f_tready;
    }
    if ((0x0000000000000020ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__10(vlSelf);
    }
    if ((0x0000000000000060ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_done 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_done;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_fail 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_fail;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_done 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_done;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_not_found 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_not_found;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_hit 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_hit;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[0U] 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value[0U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[1U] 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value[1U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[2U] 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__lookup_value[2U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_ready 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_ready;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_ready 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_ready;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ready 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__ready;
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[0U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[0U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[1U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[1U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[2U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[2U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[3U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[3U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[4U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[4U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[5U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[5U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[6U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[6U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[7U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[7U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[8U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[8U];
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_in[9U] 
            = vlSelfRef.hft_pipeline_top__DOT__decoded_msg[9U];
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid = 0U;
        if ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
            if ((1U & (~ (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r)))) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast 
                    = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r) 
                       || (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                            & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast)) 
                           & (~ (0U != (0x0000003fU 
                                        & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep) 
                                           >> 2U))))));
                if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r) {
                    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep 
                        = VL_SHIFTR_III(8,8,32, (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r), 2U);
                    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid 
                        = (0U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep));
                } else {
                    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep 
                        = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__combined_keep;
                    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid 
                        = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid;
                }
            }
        }
        vlSelfRef.hft_pipeline_top__DOT__f2d_tlast 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tlast;
        vlSelfRef.hft_pipeline_top__DOT__f2d_tkeep 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tkeep;
        vlSelfRef.hft_pipeline_top__DOT__f2d_tvalid 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid;
        vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tlast 
            = vlSelfRef.hft_pipeline_top__DOT__f2d_tlast;
        vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tkeep 
            = vlSelfRef.hft_pipeline_top__DOT__f2d_tkeep;
        vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid 
            = vlSelfRef.hft_pipeline_top__DOT__f2d_tvalid;
    }
    if ((0x0000000000000018ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__13(vlSelf);
    }
    if ((0x0000000000000180ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym 
            = (0x000001ffU & vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[4U]);
        vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_new_mid 
            = VL_SHIFTR_III(32,32,32, (vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[3U] 
                                       + vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__in_tob[1U]), 1U);
        vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_delta 
            = (0x00000001ffffffffULL & ((QData)((IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_new_mid)) 
                                        - (QData)((IData)(
                                                          ((0x13U 
                                                            >= 
                                                            (0x0000001fU 
                                                             & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym)))
                                                            ? vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__old_mid
                                                           [
                                                           (0x0000001fU 
                                                            & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym))]
                                                            : 0U)))));
        vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_weight 
            = ((0x13U >= (0x0000001fU & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym)))
                ? vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__weight_tbl
               [(0x0000001fU & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_index_arb__DOT__s0_sym))]
                : 0U);
    }
    if ((0x0000000000000300ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__next_pos 
            = (IData)((0x00000001ffffffffULL & ((1U 
                                                 & vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__in_trade[5U])
                                                 ? 
                                                (VL_EXTENDS_QI(33,32, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__net_position) 
                                                 - 0x0000000000000064ULL)
                                                 : 
                                                (0x0000000000000064ULL 
                                                 + 
                                                 VL_EXTENDS_QI(33,32, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__net_position)))));
        vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__pos_ok 
            = (VL_LTES_III(32, 0xffffd8f0U, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__next_pos) 
               & VL_GTES_III(32, 0x00002710U, vlSelfRef.hft_pipeline_top__DOT__u_tx__DOT__next_pos));
    }
    if ((7ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready 
            = ((2U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state)) 
               | (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__m_axis_tready));
        vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next 
            = vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r;
        if ((2U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
            if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
                if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) {
                    vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 0U;
                }
            } else if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_last_r) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next 
                    = ((0U != (0x0000003fU & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__prev_keep_r) 
                                              >> 2U)))
                        ? (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tvalid) 
                            & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__m_axis_tready))
                            ? 3U : 2U) : 3U);
            } else if ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                         & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready)) 
                        & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast))) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next 
                    = ((0U != (0x0000003fU & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tkeep) 
                                              >> 2U)))
                        ? 2U : 3U);
            }
        } else if ((1U & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_r))) {
            vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 2U;
        } else if (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tvalid) 
                    & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tready))) {
            if ((1U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__beat_cnt_r))) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 1U;
            } else if (vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__s_axis_tlast) {
                vlSelfRef.hft_pipeline_top__DOT__u_framer__DOT__state_next = 3U;
            }
        }
        vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__beat_transferred 
            = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tready) 
               & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid));
        vlSelfRef.hft_pipeline_top__DOT__rx_axis_tready 
            = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred 
            = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tready) 
               & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid));
        __Vinline__nba_comb__TOP__4___Vtableidx2 = 
            (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tlast) 
              << 4U) | (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__beat_transferred) 
                         << 3U) | (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__s_axis_tvalid) 
                                    << 2U) | (IData)(vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__state_r))));
        vlSelfRef.hft_pipeline_top__DOT__u_demux__DOT__state_next 
            = Vtop__ConstPool__TABLE_h585bf363_0[__Vinline__nba_comb__TOP__4___Vtableidx2];
        vlSelfRef.rx_axis_tready = vlSelfRef.hft_pipeline_top__DOT__rx_axis_tready;
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count_next 
            = vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count;
        if (vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred) {
            vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count_next 
                = ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tlast)
                    ? 0U : (7U & ((IData)(1U) + (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count))));
        }
        __Vinline__nba_comb__TOP__4___Vtableidx1 = 
            ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tvalid) 
               << 9U) | ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ethertype_ok) 
                           & ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__ip_proto_ok) 
                              & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__udp_dport_ok)))
                           ? 2U : 3U) << 7U)) | ((((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_count) 
                                                   << 4U) 
                                                  | ((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__s_axis_tlast) 
                                                     << 3U)) 
                                                 | (((IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__beat_transferred) 
                                                     << 2U) 
                                                    | (IData)(vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state))));
        vlSelfRef.hft_pipeline_top__DOT__u_filter__DOT__state_next 
            = Vtop__ConstPool__TABLE_hcf5788b1_0[__Vinline__nba_comb__TOP__4___Vtableidx1];
    }
    if ((0x0000000000000060ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_comb__TOP__5(vlSelf);
    }
    if ((0x0000000000000078ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__exec_remaining 
            = ((((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[1U] 
                  << 0x0000001fU) | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[0U] 
                                     >> 1U)) > ((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[2U] 
                                                 << 0x00000017U) 
                                                | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[1U] 
                                                   >> 9U)))
                ? (((vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[1U] 
                     << 0x0000001fU) | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_lu_value[0U] 
                                        >> 1U)) - (
                                                   (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[2U] 
                                                    << 0x00000017U) 
                                                   | (vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__msg_r[1U] 
                                                      >> 9U)))
                : 0U);
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid = 0U;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid = 0U;
        if ((1U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
            if ((4U != (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
                if ((5U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
                    vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid 
                        = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_ready;
                }
            }
            if ((4U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r))) {
                vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid 
                    = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_ready;
            }
        }
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ready 
            = ((0U == (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__state_r)) 
               & (IData)(vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ready));
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__delete_valid 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_del_valid;
        vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__insert_valid 
            = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ht_ins_valid;
        vlSelfRef.hft_pipeline_top__DOT__ol_ready = vlSelfRef.hft_pipeline_top__DOT__u_order_lookup__DOT__ready;
        vlSelfRef.ol_ready = vlSelfRef.hft_pipeline_top__DOT__ol_ready;
    }
    if ((0x0000000000000098ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_comb__TOP__7(vlSelf);
    }
}

void Vtop___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

void Vtop___024root___eval_triggers_vec__act(Vtop___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vtop___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vtop___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

bool Vtop___024root___eval_phase__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtop___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtop___024root___eval_nba(vlSelf);
        Vtop___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
bool Vtop___024root___eval_phase__ico(Vtop___024root* vlSelf);

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
            VL_FATAL_MT("../HFT_modules/verilog/hft_pipeline_top.sv", 22, "", "DIDNOTCONVERGE: Input combinational region did not converge after '--converge-limit' of 100 tries");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        vlSelfRef.__VicoPhaseResult = Vtop___024root___eval_phase__ico(vlSelf);
        vlSelfRef.__VicoFirstIteration = 0U;
    } while (vlSelfRef.__VicoPhaseResult);
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("../HFT_modules/verilog/hft_pipeline_top.sv", 22, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("../HFT_modules/verilog/hft_pipeline_top.sv", 22, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vtop___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vtop___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");
    }
    if (VL_UNLIKELY(((vlSelfRef.rx_axis_tvalid & 0xfeU)))) {
        Verilated::overWidthError("rx_axis_tvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.rx_axis_tlast & 0xfeU)))) {
        Verilated::overWidthError("rx_axis_tlast");
    }
    if (VL_UNLIKELY(((vlSelfRef.tx_axis_tready & 0xfeU)))) {
        Verilated::overWidthError("tx_axis_tready");
    }
    if (VL_UNLIKELY(((vlSelfRef.tx_enable & 0xfeU)))) {
        Verilated::overWidthError("tx_enable");
    }
    if (VL_UNLIKELY(((vlSelfRef.wt_wr_en & 0xfeU)))) {
        Verilated::overWidthError("wt_wr_en");
    }
    if (VL_UNLIKELY(((vlSelfRef.wt_wr_addr & 0xfe00U)))) {
        Verilated::overWidthError("wt_wr_addr");
    }
}
#endif  // VL_DEBUG
