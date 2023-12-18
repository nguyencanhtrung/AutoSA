# 
# Compatible: Vitis 2022.2
# nguyencanhtrung@me.com 
# Please export Vitis environment before running make
# 	source <path_to_vitis_installation>/settings64.sh
#
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil

# hw | hw_emu | sw_emu
MODE := hw

# xrt | user | opencl
HOST := opencl 

#PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
PLATFORM := xilinx_u200_gen3x16_xdma_2_202110_1
NUMDEVICES := 1

CXX := g++
VPP := v++

# sources
KERNEL_SRC := src/kernel_kernel.cpp
HOST_SRC := src/kernel_host.cpp

# targets
HOST_EXE := host.exe

XOS := kernel0.$(MODE).xo
XCLBIN := kernel0.$(MODE).xclbin
EMCONFIG_FILE := emconfig.json

# Linker options to map kernel ports to DDR banks
VPP_LINK_OPTS := --config connectivity.cfg

VPP_COMMON_OPTS := -s -t $(MODE) --platform $(PLATFORM) -R2 -O3 --kernel_frequency 250 --vivado.prop=run.impl_1.STRATEGY=Performance_EarlyBlockPlacement

###################################################################
# G++ COMPILER FLAGS
###################################################################
host_CXXFLAGS += -g -std=c++11 -I$(XILINX_XRT)/include -I$(XILINX_HLS)/include

# Native-XRT host code
xrt_LDFLAGS += -L$(XILINX_XRT)/lib -lxrt_coreutil -pthread

# OpenCL host code
opencl_LDFLAGS += -L$(XILINX_XRT)/lib -lOpenCL -pthread

CXXFLAGS += $(host_CXXFLAGS)


# Case 1: XRT-native API: user-managed
# Case 2: XRT-native API: xrt-managed
# Else  : OpenCL API 
#
ifeq ($(HOST),$(filter $(HOST),user))
   LDFLAGS += $(xrt_LDFLAGS)
else ifeq ($(HOST),$(filter $(HOST),xrt))
   LDFLAGS += $(xrt_LDFLAGS)
else
   LDFLAGS += $(opencl_LDFLAGS)
endif


# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++


###################################################################
# END G++ COMPILER FLAGS
###################################################################

# run time args
EXE_OPT := kernel0.$(MODE).xclbin

# clean targets
CLEAN_TARGETS += $(EMCONFIG_FILE)
CLEAN_TARGETS += $(HOST_EXE)
CLEAN_TARGETS += $(XCLBIN)
CLEAN_TARGETS += *.xclbin
CLEAN_TARGETS += *.xo
CLEAN_TARGETS += *.log
CLEAN_TARGETS += *.info
CLEAN_TARGETS += *_summary
CLEAN_TARGETS += $(XOS)

CLEAN_TARGETS_REP += _x
CLEAN_TARGETS_REP += *_est
CLEAN_TARGETS_REP += tuning

.PHONY: help

help: 						# Show help for each of the Makefile recipes.
	@grep -E '^[a-zA-Z0-9 -]+:.*#' Makefile | while read -r l; do printf "\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:\n\t$$(echo $$l | cut -f 2- -d'#')\n"; done


###################################################################
#check the devices avaiable
########################################################################

config: 				# Check your current setting platform
ifndef PLATFORM
	$(error PLATFORM not set. Please set the PLATFORM properly and rerun. Run "make help" for more details.)
else
	$(info [Info] Your PLATFORM is $(PLATFORM))
endif


# primary build targets
.PHONY: xclbin app all

all: xclbin app 			# Generate host app and xclbin

xclbin: $(XCLBIN)			# Generate xclbin file MODE=<hw - default | hw_emu | sw_emu> 		 		
app: $(HOST_EXE) 			# Generate host app HOST= <xrt | user | opencl - default>
	
# kernel rules
$(XOS): $(KERNEL_SRC)
	$(RM) $@
	$(VPP) $(VPP_COMMON_OPTS) -c -k kernel0 -o $@ $+


$(XCLBIN): $(XOS)
	$(VPP) $(VPP_COMMON_OPTS) -l -o $@ $+ $(VPP_LINK_OPTS)

# host rules
$(HOST_EXE): $(HOST_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $+ $(LDFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE)'

$(EMCONFIG_FILE):
	$(EMCONFIGUTIL) --nd $(NUMDEVICES) --od . --platform $(PLATFORM)

check: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE) 	# Run to check implementation support both hw emulation and hw mode 
ifeq ($(MODE),$(filter $(MODE),sw_emu hw_emu))
	XCL_EMULATION_MODE=${MODE} ./$(HOST_EXE) $(EXE_OPT)
else
	./$(HOST_EXE) $(EXE_OPT)
endif

clean: 						# Clean all generated files
	-$(RM) $(CLEAN_TARGETS)
	-$(RM) -r $(CLEAN_TARGETS_REP)
#######################################################################
# RTL Kernel only supports Hardware and Hardware Emulation.
# THis line is to check that
#########################################################################
ifneq ($(MODE),$(findstring $(MODE), hw hw_emu))
$(warning WARNING:Application supports only hw hw_emu MODE. Please use the target for running the application)
endif


############################################################################
# check the VITIS environment
#############################################################################

ifndef XILINX_VITIS
$(error XILINX_VITIS variable is not set, please set correctly and rerun)
endif


#################################################################
# Enable profiling if needed
#####################################################################a

# REPORT := no
# PROFILE := no
# DEBUG := no

# #'estimate' for estimate report generation
# #'system' for system report generation
# ifneq ($(REPORT), no)
# CLFLAGS += --report estimate
# CLLDFLAGS += --report system
# endif

# #Generates profile summary report
# ifeq ($(PROFILE), yes)
# LDCLFLAGS += --profile.data all:all:all
# endif

# #Generates debug summary report
# ifeq ($(DEBUG), yes)
# CLFLAGS += --debug.protocol all
# endif