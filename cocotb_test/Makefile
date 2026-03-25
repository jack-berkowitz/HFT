# ============================================================
# Cocotb Makefile for hft_pipeline_top
# ============================================================
SIM          ?= icarus
TOPLEVEL_LANG = verilog

# RTL sources — path relative to this Makefile
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

# Icarus: enable SystemVerilog, add include path
ifeq ($(SIM),icarus)
    COMPILE_ARGS += -g2012 -I$(VDIR)
endif

# Verilator alternative
ifeq ($(SIM),verilator)
    EXTRA_ARGS += --sv -I$(VDIR) -Wno-fatal --timing
endif

# Timescale
export COCOTB_HDL_TIMEUNIT      = 1ns
export COCOTB_HDL_TIMEPRECISION = 100ps

# Pass config to Python via environment
export HFT_NUM_COMPONENTS ?= 20
export HFT_GUI_ENABLE     ?= 1

include $(shell cocotb-config --makefiles)/Makefile.sim