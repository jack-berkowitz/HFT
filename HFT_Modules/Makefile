##########################
# ---- Introduction ---- #
##########################
# Softmax module testbench makefile
# Supports simulation, synthesis, and coverage for:
#   softmax, exponential, adder, multiplier, divider
#
# reference table of all make targets:
# make                        <- runs default target: softmax.pass
# make <module>.pass          <- greps for "@@@ Passed" or "@@@ Failed"
# make <module>.out           <- run the testbench (via build/<module>.simv)
# make <module>.verdi         <- run in verdi
# make build/<module>.simv    <- compile the testbench executable
# make <module>.syn.pass      <- greps pass/fail on synthesized module
# make <module>.syn.out       <- run synthesized module on testbench
# make synth/<module>.vg      <- synthesize the module
# make build/<module>.syn.simv <- compile synthesized module with testbench
# make <module>.cov           <- print coverage hierarchy report
# make <module>.cov.verdi     <- open coverage in verdi
# make slack                  <- grep slack from synthesis reports
# make clean                  <- remove build files and executables
# make nuke                   <- remove all generated files

.DEFAULT_GOAL = softmax.pass

######################################################
# ---- Compilation Commands and Other Variables ---- #
######################################################

export CLOCK_PERIOD = 15.0

VCS = module load vcs/2023.12-SP2-1 && \
      vcs -sverilog -xprop=tmerge +vc -Mupdate -Mdir=build/csrc -line -full64 -kdb -lca -nc \
      -debug_access+all+reverse $(VCS_BAD_WARNINGS) +define+CLOCK_PERIOD=$(CLOCK_PERIOD)

RUN_VERDI = -gui=verdi -verdi_opts "-ultra"

VCS_BAD_WARNINGS = +warn=noTFIPC +warn=noDEBUG_DEP +warn=noENUMASSIGN +warn=noLCA_FEATURES_ENABLED

LIB = /usr/caen/misc/class/eecs470/lib/verilog/lec25dscc25.v

TCL_SCRIPT = synth/470synth.tcl

SHELL := $(SHELL) -o pipefail

GREP = grep -E --color=auto

################################
# ---- Module Definitions ---- #
################################
# Modules to build and test
# Each module requires:
#   verilog/<module>.sv   - the implementation
#   test/<module>_test.sv - the testbench

MODULES = softmax exponential adder multiplier divider

# ---- Per-module source dependencies ---- #
# Add any extra files each module's simv/cov/synth targets depend on

SOFTMAX_FILES   = verilog/exponential.sv \
                  verilog/adder.sv       \
                  verilog/multiplier.sv  \
                  verilog/divider.sv

# EXPONENTIAL_FILES =
# ADDER_FILES       =
# MULTIPLIER_FILES  =
# DIVIDER_FILES     =

build/softmax.simv:   $(SOFTMAX_FILES)
build/softmax.cov.simv: $(SOFTMAX_FILES)
synth/softmax.vg:     $(SOFTMAX_FILES)

build/exponential.simv:   $(EXPONENTIAL_FILES)
build/exponential.cov.simv: $(EXPONENTIAL_FILES)
synth/exponential.vg:     $(EXPONENTIAL_FILES)

build/adder.simv:   $(ADDER_FILES)
build/adder.cov.simv: $(ADDER_FILES)
synth/adder.vg:     $(ADDER_FILES)

build/multiplier.simv:   $(MULTIPLIER_FILES)
build/multiplier.cov.simv: $(MULTIPLIER_FILES)
synth/multiplier.vg:     $(MULTIPLIER_FILES)

build/divider.simv:   $(DIVIDER_FILES)
build/divider.cov.simv: $(DIVIDER_FILES)
synth/divider.vg:     $(DIVIDER_FILES)

#####################
# ---- Running ---- #
#####################

# Run compiled executables
$(MODULES:%=build/%.out) $(MODULES:%=build/%.syn.out): build/%.out: build/%.simv
	@$(call PRINT_COLOR, 5, running $<)
	cd build && ./$(<F) | tee $(@F)

# Connect 'make mod.out' to 'make build/mod.out'
$(MODULES:%=./%.out) $(MODULES:%=./%.syn.out): ./%.out: build/%.out
	@$(call PRINT_COLOR, 2, Finished $* testbench output is in: $<)

# Grep for pass/fail (your testbench must $display "@@@ Passed" or "@@@ Failed")
%.pass: build/%.out
	@$(call PRINT_COLOR, 6, Grepping for pass/fail in $<:)
	@GREP_COLOR="01;31" $(GREP) -i '@@@ ?Failed' $< || \
	GREP_COLOR="01;32" $(GREP) -i '@@@ ?Passed' $
.PHONY: %.pass

# Run a module in verdi
./%.verdi: build/%.simv
	@$(call PRINT_COLOR, 5, running $< with verdi)
	cd build && ./$(<F) $(RUN_VERDI)
.PHONY: %.verdi

###############################
# ---- Compiling Verilog ---- #
###############################

# Compile simulation executable from test/<module>_test.sv and verilog/<module>.sv
$(MODULES:%=build/%.simv): build/%.simv: test/%_tb.sv verilog/%.sv | build
	@$(call PRINT_COLOR, 5, compiling simulation executable $@)
	$(VCS) $^ -o $@
	@$(call PRINT_COLOR, 6, finished compiling $@)

# Synthesize a module
synth/%.vg: verilog/%.sv $(TCL_SCRIPT) | synth
	@$(call PRINT_COLOR, 5, synthesizing the $* module)
	@$(call PRINT_COLOR, 3, this might take a while...)
	cd synth && \
	MODULE=$* SOURCES="$(filter-out $(TCL_SCRIPT),$^)" \
	dc_shell-t -f $(notdir $(TCL_SCRIPT)) | tee $*_synth.out
	@$(call PRINT_COLOR, 6, finished synthesizing $@)

# Grep slack from all synthesis reports
slack:
	$(GREP) "slack" synth/*.rep
.PHONY: slack

# Compile synthesis executable
$(MODULES:%=build/%.syn.simv): build/%.syn.simv: test/%_test.sv synth/%.vg | build
	@$(call PRINT_COLOR, 5, compiling synthesis executable $@)
	$(VCS) +define+SYNTH $^ $(LIB) -o $@
	@$(call PRINT_COLOR, 6, finished compiling $@)

# Syn pass/fail
$(MODULES:%=%.syn.pass): %.syn.pass: build/%.syn.out
	@$(call PRINT_COLOR, 6, Grepping for pass/fail in $<:)
	@GREP_COLOR="01;31" $(GREP) -i '@@@ ?Failed' $< || \
	GREP_COLOR="01;32" $(GREP) -i '@@@ ?Passed' $
.PHONY: %.syn.pass

##############################
# ---- Coverage Targets ---- #
##############################

VCS_COVG = -cm line+tgl+cond+branch

$(MODULES:%=build/%.cov.simv): build/%.cov.simv: test/%_test.sv verilog/%.sv | build
	@$(call PRINT_COLOR, 5, compiling coverage executable $@)
	$(VCS) $(VCS_COVG) $^ -o $@
	@$(call PRINT_COLOR, 6, finished compiling $@)

$(MODULES:%=build/%.cov.simv.out): %.cov.simv.out: %.cov.simv | build
	@$(call PRINT_COLOR, 5, running $<)
	cd build && ./$(<F) $(VCS_COVG) | tee $(@F)
	@$(call PRINT_COLOR, 2, created coverage dir $<.vdb and saved output to $@)

build/%.cov.simv.vdb: build/%.cov.simv.out ;

$(MODULES:%=cov_report_%): cov_report_%: build/%.cov.simv.vdb
	@$(call PRINT_COLOR, 5, outputting coverage report in $@)
	module load vcs/2023.12-SP2-1 && cd build && urg -format text -dir $*.cov.simv.vdb -report ../$@
	@$(call PRINT_COLOR, 2, coverage report is in $@)

$(MODULES:=.cov): %.cov: cov_report_%
	@$(call PRINT_COLOR, 2, printing coverage hierarchy - open '$<' for more)
	cat $</hierarchy.txt

$(MODULES:=.cov.verdi): %.cov.verdi: build/%.cov.simv
	@$(call PRINT_COLOR, 5, running verdi for $* coverage)
	cd build && ./$(<F) $(RUN_VERDI) -cov -covdir $(<F).vdb
.PHONY: %.cov %.cov.verdi

################################
# ---- Output Directories ---- #
################################

build synth:
	mkdir -p $@

#####################
# ---- Cleanup ---- #
#####################

clean: clean_exe clean_run_files
	@$(call PRINT_COLOR, 6, note: clean is split into: $^)

nuke: clean clean_synth
	@$(call PRINT_COLOR, 6, note: nuke is split into: $^)

clean_exe:
	@$(call PRINT_COLOR, 3, removing compiled executable files)
	rm -rf build/
	rm -rf *simv *.daidir csrc *.key
	rm -rf vcdplus.vpd vc_hdrs.h
	rm -rf unifiedInference.log xprop.log
	rm -rf *.cov cov_report_* cm.log
	rm -rf verdi* novas* *fsdb*

clean_run_files:
	@$(call PRINT_COLOR, 3, removing per-run output files)
	rm -rf build/*.out

clean_synth:
	@$(call PRINT_COLOR, 1, removing synthesis files)
	cd synth && rm -rf *.vg *_svsim.sv *.res *.rep *.ddc *.chk *.syn *.out *.db *.svf *.mr *.pvl command.log

.PHONY: clean nuke clean_exe clean_run_files clean_synth

######################
# ---- Printing ---- #
######################

PRINT_COLOR = if [ -t 0 ]; then tput setaf $(1) ; fi; echo $(2); if [ -t 0 ]; then tput sgr0; fi
# colors: 0:black, 1:red, 2:green, 3:yellow, 4:blue, 5:magenta, 6:cyan, 7:white
