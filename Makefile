# ----------------------------------------------------------------------------
# 
# Project   : 
# Filename  : Makefile
# 
# Author    : Nguyen Canh Trung
# Email     : nguyencanhtrung 'at' me 'dot' com
# Date      : 2023-12-15 16:13:21
# Last Modified : 2023-12-16 10:50:51
# Modified By   : Nguyen Canh Trung
# 
# Description: 
# 		Makefile for AutoSA flow
# HISTORY:
# Date      	By	Comments
# ----------	---	---------------------------------------------------------
# 2023-12-16	NCT	File created.
# ----------------------------------------------------------------------------
# Makefile for AutoSA flow
CC := autosa

DIR_SRC := $(shell pwd)/autosa_tests
DIR_SCRIPTS := $(shell pwd)/autosa_scripts/vitis_scripts
CONFIG 	:= $(shell pwd)/autosa_config/autosa_config.json
SIMD_INFO := $(shell pwd)/autosa_tests/mm/simd_info.json
DIR_OUTPUT 	:= $(shell pwd)/autosa.tmp
# autosa_catapult_c | autosa_hls_c
TARGET  := autosa_hls_c

DEV=mqnic0
PCI_ID=$(subst ../,,$(shell readlink /sys/class/misc/$(DEV)/device))

.PHONY: help

help: 						# Show help for each of the Makefile recipes
	@grep -E '^[a-zA-Z0-9 -_]+:.*#' Makefile | while read -r l; do printf "\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:\n\t$$(echo $$l | cut -f 2- -d'#')\n"; done

# package and repos
install-deps:
	sudo apt update
	sudo apt install automake autoconf libtool pkg-config libgmp3-dev libyaml-dev libclang-dev llvm
	sudo apt-get install ocl-icd-libopencl1
	sudo apt-get install opencl-headers
	sudo apt-get install ocl-icd-opencl-dev

install-ntl:
	wget https://libntl.org/ntl-11.5.1.tar.gz -O ntl-11.5.1.tar.gz
	gunzip ntl-11.5.1.tar.gz
	tar xf ntl-11.5.1.tar
	cd ntl-11.5.1/src && ./configure NTL_GMP_LIP=on && make && sudo make install
	rm -rf ntl-11.5.1.tar.gz ntl-11.5.1.tar ntl-11.5.1

first_install: install-deps install-ntl  	# The first installation: all dependencies and generate executable autosa
	./install.sh
	mkdir autosa.tmp && cd autosa.tmp && mkdir output optimizer && cd output && mkdir src latency_est resource_est tuning
	sudo cp -f autosa /usr/local/bin/

autosa: 					# Re-generate executable autosa 
	./install.sh	
	sudo cp -f autosa /usr/local/bin/

# HLS code generation
gstt: 						# Running getting started example
	@mkdir -p $(DIR_OUTPUT)/$@/src
	@mkdir -p $(DIR_OUTPUT)/$@/latency_est
	@mkdir -p $(DIR_OUTPUT)/$@/resource_est
	@mkdir -p $(DIR_OUTPUT)/$@/tuning
	$(CC) $(DIR_SRC)/mm_getting_started/kernel.c \
	--config=$(CONFIG) \
	--target=$(TARGET) \
	--output-dir=$(DIR_OUTPUT)/$@ \
	--sa-sizes="{kernel[]->space_time[3];kernel[]->array_part[16,16,16];kernel[]->latency[8,8];kernel[]->simd[2]}" \
	--simd-info=$(SIMD_INFO) \
	--host-serialize
	@cp $(DIR_SCRIPTS)/Makefile $(DIR_OUTPUT)/$@/
	@cp $(DIR_SCRIPTS)/connectivity.cfg $(DIR_OUTPUT)/$@/
	@echo "HLS generation completed!"
	cd $(DIR_OUTPUT)/$@/

cgemm: 						# Running cgemm example
	@mkdir -p $(DIR_OUTPUT)/$@/src
	@mkdir -p $(DIR_OUTPUT)/$@/latency_est
	@mkdir -p $(DIR_OUTPUT)/$@/resource_est
	@mkdir -p $(DIR_OUTPUT)/$@/tuning
	$(CC) $(DIR_SRC)/cgemm/kernel.c \
	--config=$(CONFIG) \
	--target=$(TARGET) \
	--output-dir=$(DIR_OUTPUT)/$@ \
	--sa-sizes="{kernel[]->space_time[3];kernel[]->array_part[16,16,16];kernel[]->latency[8,8];kernel[]->simd[2]}" \
	--simd-info=$(SIMD_INFO) \
	--host-serialize
	@cp $(DIR_SCRIPTS)/Makefile $(DIR_OUTPUT)/$@/
	@cp $(DIR_SRC)/$@/connectivity.cfg $(DIR_OUTPUT)/$@/
	@echo "HLS generation completed!"
	cd $(DIR_OUTPUT)/$@/

mm_catapult: 				# Running catapult example
	@mkdir -p $(DIR_OUTPUT)/$@/src
	@mkdir -p $(DIR_OUTPUT)/$@/latency_est
	@mkdir -p $(DIR_OUTPUT)/$@/resource_est
	@mkdir -p $(DIR_OUTPUT)/$@/tuning
	$(CC) $(DIR_SRC)/$@/kernel.c \
	--config=$(CONFIG) \
	--target=autosa_catapult_c \
	--output-dir=$(DIR_OUTPUT)/$@ \
	--sa-sizes="{kernel[]->space_time[3];kernel[]->array_part[16,16,16];kernel[]->latency[8,8];kernel[]->simd[2]}" \
	--simd-info=$(SIMD_INFO) \
	--host-serialize

SRC_GSTT := $(DIR_OUTPUT)/gstt/src/kernel_kernel.cpp
SRC_CGEMM := $(DIR_OUTPUT)/cgemm/src/kernel_kernel.cpp

# XCLBIN generation
gstt_bin: $(SRC_GSTT) 			# Generate XCLBIN for getting started example MODE = hw_emu
	make -C $(DIR_OUTPUT)/gstt/ all MODE=hw_emu HOST=opencl

cgemm_bin: $(SRC_CGEMM) 		# Generate XCLBIN for cgemm example MODE = hw_emu
	make -C $(DIR_OUTPUT)/cgemm/ all MODE=hw_emu HOST=opencl


# RTL IP generation with Catapult HLS
mm_catapult_rtl: $(DIR_OUTPUT)/mm_catapult/src/kernel_kernel_hw.h 		# Generate RTL IP with catapult HLS
	cd $(DIR_OUTPUT)/mm_catapult/src && catapult -shell -file ./kernel_directives.tcl



reset_all: 						# Reset all PCIe devices
	sudo sh -c "lspci -d 10ee:5000 | cut -d ' ' -f 1 | xargs -L 1 ./pcie_hot_reset.sh"

reset_dev: 						# Reset card with PCI_ID
	sudo ./pcie_hot_reset.sh $(PCI_ID)