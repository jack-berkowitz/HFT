// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vskewed_hash_table_tb__pch.h"

//============================================================
// Constructors

Vskewed_hash_table_tb::Vskewed_hash_table_tb(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vskewed_hash_table_tb__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vskewed_hash_table_tb::Vskewed_hash_table_tb(const char* _vcname__)
    : Vskewed_hash_table_tb(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vskewed_hash_table_tb::~Vskewed_hash_table_tb() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vskewed_hash_table_tb___024root___eval_debug_assertions(Vskewed_hash_table_tb___024root* vlSelf);
#endif  // VL_DEBUG
void Vskewed_hash_table_tb___024root___eval_static(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___eval_initial(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___eval_settle(Vskewed_hash_table_tb___024root* vlSelf);
void Vskewed_hash_table_tb___024root___eval(Vskewed_hash_table_tb___024root* vlSelf);

void Vskewed_hash_table_tb::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vskewed_hash_table_tb::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vskewed_hash_table_tb___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vskewed_hash_table_tb___024root___eval_static(&(vlSymsp->TOP));
        Vskewed_hash_table_tb___024root___eval_initial(&(vlSymsp->TOP));
        Vskewed_hash_table_tb___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vskewed_hash_table_tb___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vskewed_hash_table_tb::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vskewed_hash_table_tb::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vskewed_hash_table_tb::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vskewed_hash_table_tb___024root___eval_final(Vskewed_hash_table_tb___024root* vlSelf);

VL_ATTR_COLD void Vskewed_hash_table_tb::final() {
    Vskewed_hash_table_tb___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vskewed_hash_table_tb::hierName() const { return vlSymsp->name(); }
const char* Vskewed_hash_table_tb::modelName() const { return "Vskewed_hash_table_tb"; }
unsigned Vskewed_hash_table_tb::threads() const { return 1; }
void Vskewed_hash_table_tb::prepareClone() const { contextp()->prepareClone(); }
void Vskewed_hash_table_tb::atClone() const {
    contextp()->threadPoolpOnClone();
}

//============================================================
// Trace configuration

VL_ATTR_COLD void Vskewed_hash_table_tb::trace(VerilatedVcdC* tfp, int levels, int options) {
    vl_fatal(__FILE__, __LINE__, __FILE__,"'Vskewed_hash_table_tb::trace()' called on model that was Verilated without --trace option");
}
