##########################
# ---- Introduction ---- #
##########################
# HFT Pipeline Makefile (NYSE XDP/Pillar)
#
# Single-module targets:
#   make ipv4_udp_port_filter.out   <- compile & run filter testbench
#   make xdp_packet_framer.out      <- compile & run framer testbench
#   make xdp_msg_demux.out          <- compile & run demux testbench
#
# Chain (integration) target:
#   make xdp_chain.out              <- compile & run full pipeline TB
#                                      (filter → framer → demux)
#
# Other targets:
#   make <module>.verdi             <- open waveforms in Verdi
#   make <module>.pass              <- grep for @@@ Passed / Failed
#   make clean                      <- remove build files
#
# Directory layout:
#   verilog/sys_defs.svh            <- shared defines and typedefs
#   verilog/<module>.sv             <- RTL implementation
#   test/<module>_tb.sv             <- testbench

.DEFAULT_GOAL = xdp_chain.pass

######################################################
# ---- Compilation Commands and Other Variables ---- #
######################################################

export CLOCK_PERIOD = 4.0   # 250 MHz

VCS = module load vcs/2023.12-SP2-1 && \
      vcs -sverilog -xprop=tmerge +vc -Mupdate -Mdir=build/csrc \
          -line -full64 -kdb -lca -nc \
          -debug_access+all+reverse \
          $(VCS_BAD_WARNINGS) \
          +define+CLOCK_PERIOD=$(CLOCK_PERIOD) \
          +incdir+verilog

VCS_BAD_WARNINGS = +warn=noTFIPC +warn=noDEBUG_DEP +warn=noENUMASSIGN +warn=noLCA_FEATURES_ENABLED

RUN_VERDI = -gui=verdi -verdi_opts "-ultra"

SHELL := $(SHELL) -o pipefail

GREP = grep -E --color=auto

################################
# ---- Module Definitions ---- #
################################
# Individual modules (each has its own testbench)
MODULES = ipv4_udp_port_filter \
          xdp_packet_framer    \
          xdp_msg_demux

# Chain (integration) targets — testbench compiles multiple RTL files
CHAIN_MODULES = xdp_chain

# All targets that can be built and run
ALL_TARGETS = $(MODULES) $(CHAIN_MODULES)

# ---- Per-module RTL dependencies ---- #
# These list every .sv file needed to compile each target

ipv4_udp_port_filter_DEPS = verilog/ipv4_udp_port_filter.sv

xdp_packet_framer_DEPS    = verilog/xdp_packet_framer.sv

xdp_msg_demux_DEPS        = verilog/xdp_msg_demux.sv

# Chain TB needs all three RTL modules
xdp_chain_DEPS            = verilog/ipv4_udp_port_filter.sv \
                             verilog/xdp_packet_framer.sv    \
                             verilog/xdp_msg_demux.sv

###############################
# ---- Compile Targets ---- #
###############################

build/ipv4_udp_port_filter.simv: test/ipv4_udp_port_filter_tb.sv $(ipv4_udp_port_filter_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling ipv4_udp_port_filter...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/xdp_packet_framer.simv: test/xdp_packet_framer_tb.sv $(xdp_packet_framer_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling xdp_packet_framer...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/xdp_msg_demux.simv: test/xdp_msg_demux_tb.sv $(xdp_msg_demux_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling xdp_msg_demux...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

# ---- Chain (integration) testbench ---- #
# Compiles the chain TB together with all three RTL modules
build/xdp_chain.simv: test/xdp_chain_tb.sv $(xdp_chain_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling xdp_chain (filter + framer + demux)...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

#####################
# ---- Running ---- #
#####################

# Compile then run, save output to build/<target>.out
$(ALL_TARGETS:%=build/%.out): build/%.out: build/%.simv
	@$(call PRINT_COLOR, 5, running $<)
	cd build && ./$(<F) | tee $(@F)

# Shorthand: make <target>.out  →  make build/<target>.out
$(ALL_TARGETS:%=%.out): %.out: build/%.out
	@$(call PRINT_COLOR, 2, output saved to: build/$*.out)
.PHONY: $(ALL_TARGETS:%=%.out)

# Grep for @@@ Passed / @@@ Failed
$(ALL_TARGETS:%=%.pass): %.pass: build/%.out
	@$(call PRINT_COLOR, 6, grepping for pass/fail in build/$*.out:)
	@GREP_COLOR="01;31" $(GREP) -i '@@@ ?Failed' build/$*.out || \
	 GREP_COLOR="01;32" $(GREP) -i '@@@ ?Passed' build/$*.out
.PHONY: $(ALL_TARGETS:%=%.pass)

#####################
# ---- Verdi ---- #
#####################

$(ALL_TARGETS:%=%.verdi): %.verdi: build/%.simv
	@$(call PRINT_COLOR, 5, opening verdi for $*)
	cd build && ./$(<F) $(RUN_VERDI)
.PHONY: $(ALL_TARGETS:%=%.verdi)

################################
# ---- Output Directories ---- #
################################

build:
	mkdir -p $@

#####################
# ---- Cleanup ---- #
#####################

clean:
	@$(call PRINT_COLOR, 3, removing build files...)
	rm -rf build/
	rm -rf *simv *.daidir csrc *.key
	rm -rf vcdplus.vpd vc_hdrs.h
	rm -rf unifiedInference.log xprop.log
	rm -rf verdi* novas* *fsdb*
	rm -rf *.vcd
.PHONY: clean

######################
# ---- Printing ---- #
######################

PRINT_COLOR = if [ -t 1 ]; then tput setaf $(1); fi; echo $(2); if [ -t 1 ]; then tput sgr0; fi