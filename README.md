<div align="center">
  <img src=".github/autosa_logo.png", width="200">
</div>

# Introduction

This repository is a fork of the AutoSA project originally developed by the University of California, Los Angeles (UCLA). A fork, in the context of GitHub, is a copy of a repository that allows you to experiment with changes without affecting the original project. 

AutoSA, or Polyhedral-Based Systolic Array Auto-Compilation, is a compiler that transforms high-level programming languages (specifically C) into systolic array architecture. It leverages the polyhedral model to perform a series of transformations and architecture optimizations, outputting high-level synthesis C (HLS C) code.

<strong>What's changes?</strong>

* URLs of `pet`, `isl` and `barvinok`
* Support Vitis 2022.2 with new platforms for Alveo U200 and U250
* Support Catapult HLS 2023.2 (synthesizable but still not able to simulate - C and CoSim)
* No change at autosa generator, just modify the generated output directly


# Installation (new)

Compile the source code and generate executable file `autosa`.

```bash
make first_install
```

If you want to modify `autosa` generator, run

```bash
make autosa
```

to re-compile and generate new executable file.


## Running `Getting started example`


```bash
make gstt 
```

The above command generates

* HLS code which describes the FPGA kernel
* Host code which uses OpenCL APIs to interract with FPGA kernel

### Running HW emulation

```bash
cd autosa.tmp/gstt
make all MODE=hw_emu HOST=opencl
```

This command will create Vitis project and generate `*.xclbin` (FPGA image - in hardware emulation mode) and `host.exe` (host application). Run emulation with

```bash
make check MODE=hw_emu
```

There is an example of host code using XRT native APIs instead of OpenCL APIs, at `autosa_tests/mm_getting_started/src_xrtnative/kernel_host.cpp`. At this stage, it is written mannually and not supported by `autosa generator` yet. Compiling the XRT-native host code by:

```bash
make app HOST=xrt
```

#### Running onboard

```bash
make all MODE=hw HOST=opencl
make check MODE=hw
```

It takes serveral hours to generate `*.xclbin`

---

# AutoSA: Polyhedral-Based Systolic Array Auto-Compilation

[Documentation](https://autosa.readthedocs.io/en/latest/) |
[Installation](https://autosa.readthedocs.io/en/latest/installation.html) |
[Tutorials](https://autosa.readthedocs.io/en/latest/tutorials/index.html) |
[Examples](https://autosa.readthedocs.io/en/latest/examples/index.html)

This repository includes the code for AutoSA. AutoSA is an end-to-end systolic array compiler based on the polyhedral model. It takes algorithms in high-level programming languages (C) as inputs, performs polyhedral transformation and other architecture optimizations to map algorithms to systolic array architecture. The generated designs are in HLS C.

## Quick Start
We offer a Docker image for quick start.
```bash
docker pull whbldhwj/autosa:latest
```

Let's try one small example. The input code can be found at `${AUTOSA_ROOT}/autosa_tests/mm/kernel.c`. The code region to be transformed to systolic array is annotated using a pair of pragmas `scop` and `endscop`.

1. Generating HLS C Code.

Run the following command to compile generate a systolic array.
```c
./autosa ./autosa_tests/mm/kernel.c \
--config=./autosa_config/autosa_config.json \
--target=autosa_hls_c \
--output-dir=./autosa.tmp/output \
--sa-sizes="{kernel[]->space_time[3];kernel[]->array_part[16,16,16];kernel[]->latency[8,8];kernel[]->simd[2]}" \
--simd-info=./autosa_tests/mm/simd_info.json \
--host-serialize
```
The generated code can be found in `${AUTOSA_ROOT}/autosa.tmp/output/src/`.
For detailed explaination of each AutoSA compilation option, please run
```c
./autosa --help
```
or refer to [AutoSA Compilation Options](https://autosa.readthedocs.io/en/latest/tutorials/getting_started.html#autosa-compilation-options).

2. Generating FPGA Bitstream

To generate the final bitsteam, set up your local Vitis development kit first.
Then execute the makefile to build the design.
```
cp ${AUTOSA_ROOT}/autosa_tests/mm/Makefile autosa.tmp/output/
cp ${AUTOSA_ROOT}/autosa_tests/mm/connectivity.cfg autosa.tmp/output/
cd ${AUTOSA_ROOT}/autosa.tmp/output
make all
```
**Makefile Options Descriptions**

* `MODE := hw_emu`: Set the build configuration mode to HW Emulation, other modes: sw_emu|hw
* `PLATFORM := xilinx_u250_xdma_201830_2`: Select the target platform
* `KERNEL_SRC := src/kernel_kernel.cpp`: List the kernel source files
* `HOST_SRC := src/kernel_host.cpp`: List the host source files

The `connectivity.cfg` describes the DRAM port mapping. For more details about how to change the DRAM port mapping, please refer to the Xilinx tutorials.

3. Verifying Designs Using Xilinx HLS

AutoSA also supports generate HLS projects. Add the flag
```
--hls
```
to the command when compiling the program.

```c
./autosa ./autosa_tests/mm/kernel.c \
--config=./autosa_config/autosa_config.json \
--target=autosa_hls_c \
--output-dir=./autosa.tmp/output \
--sa-sizes="{kernel[]->space_time[3];kernel[]->array_part[16,16,16];kernel[]->latency[8,8];kernel[]->simd[2]}" \
--simd-info=./autosa_tests/mm/simd_info.json \
--host-serialize \
--hls
```

AutoSA will generate an HLS host file `${AUTOSA_ROOT}/autosa.tmp/output/src/kernel_host.cpp` instead of the OpenCL host file generated in the previous step. To build the HLS project, run the following commands.
```
cp ${AUTOSA_ROOT}/autosa_scripts/hls_scripts/hls_script.tcl autosa.tmp/output/
cd ${AUTOSA_ROOT}/autosa.tmp/output
vivado_hls -f hls_script.tcl
```

For more detailed instructions on using AutoSA, please refer to the [AutoSA Documentation](https://autosa.readthedocs.io/en/latest/).

## Send Us Failure Cases and Feedback!
AutoSA is open source for research purposes, and we would like to continously improve it! Please let us know if...

1. you find any bug in the AutoSA code.
2. you find any application that fails the compilation flow of AutoSA.
3. you know how to further help improve any part of the compiler.
4. etc.

## Authors and Contributors
AutoSA is currently maintained by [Jie Wang](http://cadlab.cs.ucla.edu/~jaywang/).
Besides, we gratefully acknowledge the authors of [PPCG](https://github.com/Meinersbur/ppcg) for developing and actively maintaining PPCG as an open-source project.

## Papers
More implementation details of AutoSA are covered in [our paper](http://cadlab.cs.ucla.edu/~jaywang/papers/fpga21-autosa.pdf). If you find this project useful in your research, please consider citing:

    @inproceedings{wang2021autosa,
      title={AutoSA: A Polyhedral Compiler for High-Performance Systolic Arrays on FPGA},
      author={Wang, Jie and Guo, Licheng and Cong, Jason},
      booktitle={Proceedings of the 2021 ACM/SIGDA International Symposium on Field-Programmable Gate Arrays},
      year={2021}
    }
