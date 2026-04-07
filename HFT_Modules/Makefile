##########################
# ---- Introduction ---- #
##########################
# HFT Pipeline Makefile (NYSE XDP/Pillar)
#
# Single-module targets:
#   make ipv4_udp_port_filter.out   <- compile & run filter testbench
#   make xdp_packet_framer.out      <- compile & run framer testbench
#   make xdp_msg_demux.out          <- compile & run demux testbench
#   make skewed_hash_table.out      <- compile & run hash table testbench
#   make order_lookup.out           <- compile & run order lookup testbench
#   make order_gen_tx.out           <- compile & run TX path testbench
#
# Chain (integration) targets:
#   make xdp_chain.out              <- compile & run front-end chain TB
#                                      (filter → framer → demux)
#   make hft_pipeline.out           <- compile & run full 7-stage pipeline TB
#   make hft_pipeline_top.out       <- compile & run top-level wrapper TB
#
# Synthesis targets (individual modules only, not chain/integration TBs):
#   make synth/<module>.vg          <- synthesize a module to a gate-level netlist
#   make <module>.syn.out           <- run synthesized module against its testbench
#   make <module>.syn.pass          <- grep for @@@ Passed / Failed in syn output
#   make <module>.syn.verdi         <- open synthesized waveforms in Verdi
#   make slack                      <- grep slack status from all synthesis reports
#
# Other targets:
#   make <module>.verdi             <- open waveforms in Verdi
#   make <module>.pass              <- grep for @@@ Passed / Failed
#   make clean                      <- remove build files
#   make clean_synth                <- remove synthesis files only
#   make nuke                       <- remove build/ and synth/
#
# Directory layout:
#   verilog/sys_defs.svh            <- shared defines and typedefs
#   verilog/<module>.sv             <- RTL implementation
#   test/<module>_tb.sv             <- testbench
#   synth/470synth.tcl              <- synthesis TCL script (from EECS 470)
#   synth/<module>.vg               <- synthesized gate-level netlist (generated)

.DEFAULT_GOAL = xdp_chain.pass

######################################################
# ---- Compilation Commands and Other Variables ---- #
######################################################

export CLOCK_PERIOD = 50.0   # 250 MHz

VCS = module load vcs/2023.12-SP2-1 && \
      vcs -sverilog -xprop=tmerge +vc -Mupdate -Mdir=build/csrc \
          -line -full64 -kdb -lca -nc \
          -debug_access+all+reverse \
          $(VCS_BAD_WARNINGS) \
          +define+CLOCK_PERIOD=$(CLOCK_PERIOD) \
          +incdir+verilog

VCS_BAD_WARNINGS = +warn=noTFIPC +warn=noDEBUG_DEP +warn=noENUMASSIGN +warn=noLCA_FEATURES_ENABLED


RUN_VERDI = -gui=verdi -verdi_opts "-ultra"

# Standard cell library for synthesis linking
# LIB = /usr/caen/misc/class/eecs470/lib/verilog/lec25dscc25.v

LIB = /afs/umich.edu/class/eecs470/lib/verilog/lec25dscc25.v

# EECS 470 synthesis TCL script
TCL_SCRIPT = synth/470synth.tcl

SHELL := $(SHELL) -o pipefail

GREP = grep -E --color=auto

################################
# ---- Module Definitions ---- #
################################

# Individual modules (each has its own testbench, and can be synthesized)
MODULES = ipv4_udp_port_filter \
          xdp_packet_framer    \
          xdp_msg_demux        \
          skewed_hash_table    \
          order_lookup         \
          top_of_book          \
          index_arb_engine     \
          order_gen_tx         \
          hft_pipeline_top

# Chain targets — testbench drives multiple RTL modules directly, no wrapper RTL file.
# Simulation-only; there is nothing to synthesize here.
CHAIN_MODULES = xdp_chain hft_pipeline

# All targets that can be built and run (simulation)
ALL_TARGETS = $(MODULES) $(CHAIN_MODULES)

# ---- Per-module RTL dependencies ---- #
# These list every .sv file needed to compile each target

ipv4_udp_port_filter_DEPS = verilog/ipv4_udp_port_filter.sv

xdp_packet_framer_DEPS    = verilog/xdp_packet_framer.sv

xdp_msg_demux_DEPS        = verilog/xdp_msg_demux.sv

skewed_hash_table_DEPS    = verilog/skewed_hash_table.sv \
                            verilog/dp_ram.sv

order_lookup_DEPS         = verilog/order_lookup.sv \
                            verilog/skewed_hash_table.sv \
                            verilog/dp_ram.sv

# ... and similarly for hft_pipeline_DEPS, hft_pipeline_top_DEPS, xdp_chain_DEPS

top_of_book_DEPS          = verilog/top_of_book.sv

index_arb_engine_DEPS     = verilog/index_arb_engine.sv

order_gen_tx_DEPS         = verilog/order_gen_tx.sv

# Front-end chain TB (filter + framer + demux)
xdp_chain_DEPS            = verilog/ipv4_udp_port_filter.sv \
                             verilog/xdp_packet_framer.sv    \
                             verilog/xdp_msg_demux.sv        \
                             verilog/skewed_hash_table.sv 	\
							 verilog/dp_ram.sv

# Full pipeline integration TB (all 7 stages, no top wrapper)
hft_pipeline_DEPS         = verilog/ipv4_udp_port_filter.sv \
                             verilog/xdp_packet_framer.sv    \
                             verilog/xdp_msg_demux.sv        \
                             verilog/skewed_hash_table.sv    \
                             verilog/order_lookup.sv          \
                             verilog/top_of_book.sv           \
                             verilog/index_arb_engine.sv      \
                             verilog/order_gen_tx.sv			\
							 verilog/dp_ram.sv

# Top-level wrapper TB (hft_pipeline_top.sv + all sub-modules)
hft_pipeline_top_DEPS     = verilog/hft_pipeline_top.sv      \
                             verilog/ipv4_udp_port_filter.sv  \
                             verilog/xdp_packet_framer.sv     \
                             verilog/xdp_msg_demux.sv         \
                             verilog/skewed_hash_table.sv     \
                             verilog/order_lookup.sv           \
                             verilog/top_of_book.sv            \
                             verilog/index_arb_engine.sv       \
                             verilog/order_gen_tx.sv			\
							 verilog/dp_ram.sv

# ---- Per-module synthesis source lists ---- #
# By default a module synthesizes from its own single .sv file.
# Modules with internal sub-module dependencies (like order_lookup) need
# all their RTL listed here so dc_shell can elaborate the full hierarchy.

ipv4_udp_port_filter_SYN_SRCS = verilog/ipv4_udp_port_filter.sv

xdp_packet_framer_SYN_SRCS    = verilog/xdp_packet_framer.sv

xdp_msg_demux_SYN_SRCS        = verilog/xdp_msg_demux.sv

skewed_hash_table_SYN_SRCS    = verilog/skewed_hash_table.sv \
                                synth/dp_ram_stub.sv

order_lookup_SYN_SRCS         = verilog/order_lookup.sv \
                                verilog/skewed_hash_table.sv \
                                synth/dp_ram_stub.sv

top_of_book_SYN_SRCS          = verilog/top_of_book.sv

index_arb_engine_SYN_SRCS     = verilog/index_arb_engine.sv

order_gen_tx_SYN_SRCS         = verilog/order_gen_tx.sv

# hft_pipeline_top instantiates all 7 pipeline stages, so dc_shell needs every sub-module
hft_pipeline_top_SYN_SRCS    = verilog/hft_pipeline_top.sv      \
                                 verilog/ipv4_udp_port_filter.sv  \
                                 verilog/xdp_packet_framer.sv     \
                                 verilog/xdp_msg_demux.sv         \
                                 verilog/skewed_hash_table.sv     \
                                 synth/dp_ram_stub.sv             \
                                 verilog/order_lookup.sv          \
                                 verilog/top_of_book.sv           \
                                 verilog/index_arb_engine.sv      \
                                 verilog/order_gen_tx.sv

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

build/skewed_hash_table.simv: test/skewed_hash_table_tb.sv $(skewed_hash_table_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling skewed_hash_table...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/order_lookup.simv: test/order_lookup_tb.sv $(order_lookup_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling order_lookup...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/top_of_book.simv: test/top_of_book_tb.sv $(top_of_book_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling top_of_book...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/index_arb_engine.simv: test/index_arb_engine_tb.sv $(index_arb_engine_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling index_arb_engine...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/order_gen_tx.simv: test/order_gen_tx_tb.sv $(order_gen_tx_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling order_gen_tx...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

# ---- Chain (integration) testbenches ---- #
build/xdp_chain.simv: test/xdp_chain_tb.sv $(xdp_chain_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling xdp_chain - filter + framer + demux...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/hft_pipeline.simv: test/hft_pipeline_tb.sv $(hft_pipeline_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling hft_pipeline - full 7-stage integration...)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, done)

build/hft_pipeline_top.simv: test/hft_pipeline_top_tb.sv $(hft_pipeline_top_DEPS) | build
	@$(call PRINT_COLOR, 5, compiling hft_pipeline_top - top-level wrapper integration...)
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

############################
# ---- Synthesis ---- #
############################
# Synthesizes individual RTL modules to gate-level netlists (.vg) using
# dc_shell and the shared EECS 470 TCL script. Chain/integration targets
# are not supported here — synthesize the leaf modules individually.
#
# Each synthesized module produces:
#   synth/<module>.vg        <- gate-level netlist (linked against LIB)
#   synth/<module>.rep       <- timing report (check with 'make slack')
#   synth/<module>.res       <- area/resource report
#   synth/<module>_synth.out <- dc_shell transcript

# Explicit synthesis rules, one per module.
# A static pattern rule like "synth/%.vg: $$($$*_SYN_SRCS)" does not work because
# $* is not available during prerequisite expansion — Make would try to build a
# target literally named "$($*_SYN_SRCS)". foreach+eval generates a fully explicit
# rule for each module at parse time, when the variable names are known.
define SYNTH_RULE
synth/$(1).vg: $($(1)_SYN_SRCS) $(TCL_SCRIPT) | synth
	@$$(call PRINT_COLOR, 5, synthesizing $(1)...)
	@$$(call PRINT_COLOR, 3, this may take a while...)
	cd synth && \
	module load synopsys-synth && \
	MODULE=$(1) SOURCES="$$(addprefix ../,$($(1)_SYN_SRCS))" \
	dc_shell-t -f $$(notdir $$(TCL_SCRIPT)) | tee $(1)_synth.out
	@$$(call PRINT_COLOR, 6, done - netlist at synth/$(1).vg)
endef
$(foreach mod,$(MODULES),$(eval $(call SYNTH_RULE,$(mod))))

# Shorthand: make <module>.synth  →  make synth/<module>.vg
$(MODULES:%=%.synth): %.synth: synth/%.vg
	@$(call PRINT_COLOR, 2, synthesis complete: synth/$*.vg)
.PHONY: $(MODULES:%=%.synth)

# Grep slack from all synthesis timing reports
slack:
	$(GREP) "slack" synth/*.rep
.PHONY: slack

##################################
# ---- Synthesized Simulation ---- #
##################################
# Recompiles each testbench against the gate-level netlist (.vg) and the
# standard cell library. +define+SYNTH is passed so testbenches can gate
# any RTL-only constructs (e.g. $dumpvars, non-synthesizable checks).

# Build the synthesis simulation executable
$(MODULES:%=build/%.syn.simv): build/%.syn.simv: test/%_tb.sv synth/%.vg | build
	@$(call PRINT_COLOR, 5, compiling synthesized $* testbench...)
	$(VCS) +define+SYNTH $^ $(LIB) -o $@
	@$(call PRINT_COLOR, 6, done)

# Run the synthesized simulation
$(MODULES:%=build/%.syn.out): build/%.syn.out: build/%.syn.simv
	@$(call PRINT_COLOR, 5, running synthesized $<)
	cd build && ./$(<F) | tee $(@F)

# Shorthand: make <module>.syn.out  →  make build/<module>.syn.out
$(MODULES:%=%.syn.out): %.syn.out: build/%.syn.out
	@$(call PRINT_COLOR, 2, syn output saved to: build/$*.syn.out)
.PHONY: $(MODULES:%=%.syn.out)

# Grep for @@@ Passed / @@@ Failed in synthesized output
$(MODULES:%=%.syn.pass): %.syn.pass: build/%.syn.out
	@$(call PRINT_COLOR, 6, grepping for pass/fail in build/$*.syn.out:)
	@GREP_COLOR="01;31" $(GREP) -i '@@@ ?Failed' build/$*.syn.out || \
	 GREP_COLOR="01;32" $(GREP) -i '@@@ ?Passed' build/$*.syn.out
.PHONY: $(MODULES:%=%.syn.pass)

# Open synthesized simulation in Verdi
$(MODULES:%=%.syn.verdi): %.syn.verdi: build/%.syn.simv
	@$(call PRINT_COLOR, 5, opening verdi for synthesized $*)
	cd build && ./$(<F) $(RUN_VERDI)
.PHONY: $(MODULES:%=%.syn.verdi)

################################
# ---- Output Directories ---- #
################################

build:
	mkdir -p $@

synth:
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

# Remove synthesis outputs only (keep simulation build/)
clean_synth:
	@$(call PRINT_COLOR, 1, removing synthesis files...)
	cd synth && rm -rf *.vg *.rep *.res *.ddc *.chk *.syn *.out *.db \
	                       *.svf *.mr *.pvl *_svsim.sv command.log
.PHONY: clean_synth

# Remove everything (build/ + synth/)
nuke: clean clean_synth
	@$(call PRINT_COLOR, 1, nuked build/ and synth/)
.PHONY: nuke

######################
# ---- Printing ---- #
######################

PRINT_COLOR = if [ -t 1 ]; then tput setaf $(1); fi; echo $(2); if [ -t 1 ]; then tput sgr0; fi