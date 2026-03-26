# ============================================================
# Cocotb Makefile for hft_pipeline_top
# ============================================================
SIM          ?= icarus
TOPLEVEL_LANG = verilog

VDIR = ../HFT_modules/verilog

VERILOG_SOURCES  = $(VDIR)/hft_pipeline_top.sv    \
                   $(VDIR)/ipv4_udp_port_filter.sv \
                   $(VDIR)/XDP_packet_framer.sv    \
                   $(VDIR)/xdp_msg_demux.sv        \
                   $(VDIR)/skewed_hash_table.sv    \
                   $(VDIR)/order_lookup.sv          \
                   $(VDIR)/top_of_book.sv           \
                   $(VDIR)/index_arb_engine.sv      \
                   $(VDIR)/order_gen_tx.sv

TOPLEVEL = hft_pipeline_top
MODULE   = test_hft_pipeline_top

# Number of index components — set on command line to scale up
#   make SIM=verilator HFT_NUM_COMPONENTS=500    (full S&P 500)
#   make SIM=verilator HFT_NUM_COMPONENTS=20     (fast sim, default)
export HFT_NUM_COMPONENTS ?= 20
export HFT_GUI_ENABLE     ?= 1
export HFT_DELAY          ?= 0.4

# Derived parameters — all scale automatically from HFT_NUM_COMPONENTS
HFT_IDX_SYM     := $(HFT_NUM_COMPONENTS)
HFT_TOB_SYMBOLS := $(shell echo $$(( $(HFT_NUM_COMPONENTS) + 2 )))
# Hash table needs ~8x headroom over max orders (N_COMP * 6 levels)
# Round up to next power of 2 for efficient hashing
HFT_HT_ENTRIES  := $(shell python3 -c "import math; print(1 << math.ceil(math.log2(max(256, $(HFT_NUM_COMPONENTS) * 16))))")

# Icarus
ifeq ($(SIM),icarus)
    COMPILE_ARGS += -g2012 -I$(VDIR)
endif

# Verilator — override RTL parameters to match Python test config
ifeq ($(SIM),verilator)
    EXTRA_ARGS += --sv -I$(VDIR) -Wno-fatal --timing
    EXTRA_ARGS += -GN_COMPONENTS=$(HFT_NUM_COMPONENTS)
    EXTRA_ARGS += -GIDX_SYM=$(HFT_IDX_SYM)
    EXTRA_ARGS += -GTOB_N_SYMBOLS=$(HFT_TOB_SYMBOLS)
    EXTRA_ARGS += -GIDX_SYM_ID=$(HFT_IDX_SYM)
    EXTRA_ARGS += -GCOOLDOWN_CYC=20
    EXTRA_ARGS += -GMAX_POS=10000
    EXTRA_ARGS += -GPRICE_OFFSET=10
    EXTRA_ARGS += -GHT_TBL_ENTRIES=$(HFT_HT_ENTRIES)
endif

# Timescale
export COCOTB_HDL_TIMEUNIT      = 1ns
export COCOTB_HDL_TIMEPRECISION = 100ps

include $(shell cocotb-config --makefiles)/Makefile.sim