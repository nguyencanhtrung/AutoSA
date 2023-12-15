# 
# Compatible: Vitis 2022.2
# nguyencanhtrung@me.com 
# Please export Vitis environment before running make
# 	source <path_to_vitis_installation>/settings64.sh
#
VPP := $(XILINX_VITIS)/bin/v++
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil
MODE := hw
#PLATFORM := xilinx_u250_xdma_201830_2
#PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
PLATFORM := xilinx_u200_gen3x16_xdma_2_202110_1


CXX := g++

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


CXXFLAGS += $(host_CXXFLAGS)
LDFLAGS += $(xrt_LDFLAGS)

# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++

# OpenCL host code
LDFLAGS += -lxilinxopencl
###################################################################
# END G++ COMPILER FLAGS
###################################################################

NUMDEVICES := 1

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



# primary build targets
.PHONY: xclbin app all

xclbin:  $(XCLBIN)
app: $(HOST_EXE)

all: xclbin app

clean:
	-$(RM) $(CLEAN_TARGETS)
	-$(RM) -r $(CLEAN_TARGETS_REP)

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

check: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
	XCL_EMULATION_MODE=${MODE} ./$(HOST_EXE) $(EXE_OPT)
