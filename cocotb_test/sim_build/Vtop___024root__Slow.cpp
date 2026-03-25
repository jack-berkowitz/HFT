// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

// Parameter definitions for Vtop___024root
constexpr CData/*2:0*/ Vtop___024root::hft_pipeline_top__DOT__u_filter__DOT__BEAT_ETHERTYPE;
constexpr CData/*2:0*/ Vtop___024root::hft_pipeline_top__DOT__u_filter__DOT__BEAT_IP_PROTO;
constexpr CData/*2:0*/ Vtop___024root::hft_pipeline_top__DOT__u_filter__DOT__BEAT_UDP_DPORT;
constexpr CData/*2:0*/ Vtop___024root::hft_pipeline_top__DOT__u_filter__DOT__BEAT_DECISION;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__FILTER_PORT;
constexpr SData/*8:0*/ Vtop___024root::hft_pipeline_top__DOT__IDX_SYM;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__SRC_PORT;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__DST_PORT;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__u_filter__DOT__FILTER_PORT;
constexpr SData/*8:0*/ Vtop___024root::hft_pipeline_top__DOT__u_index_arb__DOT__IDX_SYM;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__SRC_PORT;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__DST_PORT;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__ORDER_MSG_NEW;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__IP_TOTAL_LEN;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__UDP_LEN;
constexpr SData/*15:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__IP_CSUM;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__HEADER_BYTES;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__HT_N_WAYS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__HT_TBL_ENTRIES;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__HT_MAX_CHAIN;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__TOB_N_LEVELS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__TOB_N_SYMBOLS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__N_COMPONENTS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__SRC_IP;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__DST_IP;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__ORDER_QTY;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__COOLDOWN_CYC;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__MAX_POS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__PRICE_OFFSET;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__IDX_SYM_ID;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_framer__DOT__HEADER_BYTES;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_framer__DOT__KEEP_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_framer__DOT__SKIP_BEATS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_framer__DOT__OFFSET;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_framer__DOT__REMAIN;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_framer__DOT__CNT_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__N_WAYS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__TBL_ENTRIES;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__MAX_CHAIN;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__KEY_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__VALUE_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__P_HI;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__P_LO;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__Q_HI;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__Q_LO;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__S_BIT;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__KEY_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__VALUE_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__N_WAYS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__TBL_ENTRIES;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__MAX_CHAIN;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__IDX_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__ENTRY_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__WAY_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__CHN_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__V_BIT;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__KEY_HI;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__KEY_LO;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__VAL_HI;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__VAL_LO;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tob__DOT__N_LEVELS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tob__DOT__N_SYMBOLS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tob__DOT__SYM_W;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_index_arb__DOT__N_COMPONENTS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_index_arb__DOT__FRAC;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_index_arb__DOT__AW;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_index_arb__DOT__WW;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_index_arb__DOT__SYMW;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__SRC_IP;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__DST_IP;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__ORDER_QTY;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__COOLDOWN_CYC;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__MAX_POS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__PRICE_OFFSET;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__IDX_SYM_ID;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__N_BEATS;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__LAST_BEAT;
constexpr IData/*31:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__BEAT_W;
constexpr QData/*47:0*/ Vtop___024root::hft_pipeline_top__DOT__SRC_MAC;
constexpr QData/*47:0*/ Vtop___024root::hft_pipeline_top__DOT__DST_MAC;
constexpr QData/*47:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__SRC_MAC;
constexpr QData/*47:0*/ Vtop___024root::hft_pipeline_top__DOT__u_tx__DOT__DST_MAC;
constexpr VlUnpacked<QData/*63:0*/, 8> Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_SEEDS;
constexpr VlUnpacked<IData/*31:0*/, 8> Vtop___024root::hft_pipeline_top__DOT__u_order_lookup__DOT__u_order_table__DOT__HASH_ROT;


void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf);

Vtop___024root::Vtop___024root(Vtop__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vtop___024root___ctor_var_reset(this);
}

void Vtop___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vtop___024root::~Vtop___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
