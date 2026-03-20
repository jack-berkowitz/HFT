##########################
# ---- Introduction ---- #
##########################
# HFT Pipeline Makefile
# Usage:
#   make ipv4_udp_port_filter.out   <- compile and run testbench
#   make mdp3_packet_framer.out     <- compile and run testbench
#   make sbe_template_demux.out     <- compile and run testbench
#   make <module>.verdi             <- open waveforms in Verdi
#   make clean                      <- remove build files
#
# Directory layout expected:
#   verilog/sys_defs.svh            <- shared defines and typedefs
#   verilog/<module>.sv             <- RTL implementation
#   test/<module>_tb.sv             <- testbench

.DEFAULT_GOAL = ipv4_udp_port_filter.out

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
          +incdir+verilog                        # `include "sys_defs.svh" resolves here

VCS_BAD_WARNINGS = +warn=noTFIPC +warn=noDEBUG_DEP +warn=noENUMASSIGN +warn=noLCA_FEATURES_ENABLED

RUN_VERDI = -gui=verdi -verdi_opts "-ultra"

SHELL := $(SHELL) -o pipefail

################################
# ---- Module Definitions ---- #
################################
# Add new modules here as the project grows
# Each module needs:
#   verilog/<module>.sv      - RTL implementation
#   test/<module>_tb.sv      - testbench

MODULES = ipv4_udp_port_filter \
          mdp3_packet_framer   \
          sbe_template_demux

# ---- Per-module source file lists ---- #
# Add dependencies here if a module includes other .sv files

ipv4_udp_port_filter_DEPS = verilog/ipv4_udp_port_filter.sv
mdp3_packet_framer_DEPS   = verilog/mdp3_packet_framer.sv
sbe_template_demux_DEPS   = verilog/sbe_template_demux.sv

###############################
# ---- Compile Targets ---- #
###############################

# Compile rule: test/<module>_tb.sv + verilog deps -> build/<module>.simv
build/ipv4_udp_port_filter.simv: test/ipv4_udp_port_filter_tb.sv $(ipv4_udp_port_filter_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling ipv4_udp_port_filter...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done compiling ipv4_udp_port_filter)

build/mdp3_packet_framer.simv: test/mdp3_packet_framer_tb.sv $(mdp3_packet_framer_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling mdp3_packet_framer...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done compiling mdp3_packet_framer)

build/sbe_template_demux.simv: test/sbe_template_demux_tb.sv $(sbe_template_demux_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling sbe_template_demux...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done compiling sbe_template_demux)

#####################
# ---- Run Targets ---- #
#####################

# build/module.out — compile then run, save output to build/module.out
$(MODULES:%=build/%.out): build/%.out: build/%.simv
	@$(call PRINT_COLOR, 5, running $<)
	cd build && ./$(<F) | tee $(@F)

# module.out — shorthand alias for build/module.out
# This is what lets you type: make ipv4_udp_port_filter.out
$(MODULES:%=%.out): %.out: build/%.out
	@$(call PRINT_COLOR, 2, output saved to: build/$@)
.PHONY: $(MODULES:%=%.out)

#####################
# ---- Verdi ---- #
#####################

$(MODULES:%=%.verdi): %.verdi: build/%.simv
	@$(call PRINT_COLOR, 5, opening verdi for $*)
	cd build && ./$(<F) $(RUN_VERDI)
.PHONY: $(MODULES:%=%.verdi)

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
.PHONY: clean

######################
# ---- Printing ---- #
######################

# Usage: $(call PRINT_COLOR, <color_num>, <message>)
# Colors: 0=black 1=red 2=green 3=yellow 4=blue 5=magenta 6=cyan 7=white
PRINT_COLOR = if [ -t 1 ]; then tput setaf $(1); fi; echo $(2); if [ -t 1 ]; then tput sgr0; fi