// ----------------------------------------------------------------------------
// 
// Project   : mm_getting_started
// Filename  : kernel_host
// 
// Author    : Nguyen Canh Trung
// Email     : nguyencanhtrung 'at' me 'dot' com
// Date      : 2023-12-13 14:39:41
// Last Modified : 2023-12-13 14:48:34
// Modified By   : Nguyen Canh Trung
// 
// Description: 
// 
// HISTORY:
// Date      	By	Comments
// ----------	---	---------------------------------------------------------
// 2023-12-13	NCT	Modify to comply with Vitis 2022.2
// ----------------------------------------------------------------------------
#include <iostream>
#include <cstring>

// XRT includes
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include <experimental/xrt_xclbin.h>

#include <assert.h>
#include <stdio.h>
#include "kernel_kernel.h"

// XRT mode
#include "xrt/xrt_kernel.h"

// User mode
// #include <experimental/xrt_ip.h>

// Uncomment the macro below to apply the layout transformation on array B to enable SIMD vectorization
#define LAYOUT_TRANSFORM

#include "kernel.h"

int main(int argc, char **argv) {

    std::cout << "argc = " << argc << std::endl;
    for(int i=0; i < argc; i++){
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }

    // Read settings
    std::string binaryFile = argv[1];
    //int device_index = (int)argv[2];
    int device_index = 0;

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    auto krnl = xrt::kernel(device, uuid, "kernel0", xrt::kernel::cu_access_mode::exclusive);


#ifndef LAYOUT_TRANSFORM  
    data_t A[I][K], B[K][J], C[I][J], C_golden[I][J]; 
#else  
    data_t A[I][K], B[J][K], C[I][J], C_golden[I][J];
#endif

    for (int i = 0; i < I; i++) 
        for (int k = 0; k < K; k++) {
            A[i][k] = (data_t)rand() / RAND_MAX;
        }

    for (int j = 0; j < J; j++)
        for (int k = 0; k < K; k++) {
#ifndef LAYOUT_TRANSFORM      
            B[k][j] = (data_t)rand() / RAND_MAX;
#else      
            B[j][k] = (data_t)rand() / RAND_MAX;
#endif      
        }
    /////////////////////////////////////////////////////////////////////////
    // Kernel interaction
    /////////////////////////////////////////////////////////////////////////
    {
    // Allocate memory in host memory
    float *dev_A_unserialized = (float *)malloc((64) * (64) * sizeof(float));
    float *dev_A = (float *)malloc(16384 * sizeof(float));
    float *dev_B_unserialized = (float *)malloc((64) * (64) * sizeof(float));
    float *dev_B = (float *)malloc(16384 * sizeof(float));
    float *dev_C_unserialized = (float *)malloc((64) * (64) * sizeof(float));
    float *dev_C = (float *)malloc(4096 * sizeof(float));

    // Initialize host buffers
    memcpy(dev_A_unserialized, A, (64) * (64) * sizeof(float));
    memcpy(dev_B_unserialized, B, (64) * (64) * sizeof(float));
    host_serialize_A(dev_A, dev_A_unserialized);
    host_serialize_B(dev_B, dev_B_unserialized);

    // Allocate buffers in device memory
    std::cout << "Allocate Buffer in Global Memory\n";
    auto boA = xrt::bo(device, (16384) * sizeof(float), krnl.group_id(0)); //Match kernel arguments to RTL kernel
    auto boB = xrt::bo(device, (16384) * sizeof(float), krnl.group_id(1));
    auto boC = xrt::bo(device, (4096) * sizeof(float), krnl.group_id(2));

   // Map the contents of the buffer object into host memory
    auto bo0_map = boA.map<float *>();
    auto bo1_map = boB.map<float *>();
    auto bo2_map = boC.map<float *>();

    std::fill(bo0_map, bo0_map + 16384, 0);
    std::fill(bo1_map, bo1_map + 16384, 0);
    std::fill(bo2_map, bo2_map + 4096, 0);

    // Copy test data
    memcpy(bo0_map, dev_A, (16384) * sizeof(float));
    memcpy(bo1_map, dev_B, (16384) * sizeof(float));

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";
    boA.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boB.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "Execution of the kernel\n";
    auto run = krnl(boA, boB, boC); //DATA_SIZE=size
    run.wait();

    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    boC.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    memcpy(dev_C, bo2_map, (4096) * sizeof(float));

    host_deserialize_C(dev_C_unserialized, dev_C);
    // Restore data from host buffers
    memcpy(C, dev_C_unserialized, (64) * (64) * sizeof(float));

    free(dev_A);
    free(dev_A_unserialized);
    free(dev_B);
    free(dev_B_unserialized);
    free(dev_C);
    free(dev_C_unserialized);
    }
    /////////////////////////////////////////////////////////////////////////
    // END Kernel interaction
    /////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < I; i++)
        for (int j = 0; j < J; j++) {
            C_golden[i][j] = 0;
            for (int k = 0; k < K; k++) {
#ifndef LAYOUT_TRANSFORM        
                C_golden[i][j] = C_golden[i][j] + A[i][k] * B[k][j];
#else
                C_golden[i][j] = C_golden[i][j] + A[i][k] * B[j][k];
#endif        
            }
        }

    int err = 0;
    for (int i = 0; i < I; i++)
        for (int j = 0; j < J; j++) {
            if (fabs((float)C_golden[i][j] - (float)C[i][j]) > 0.001)
            err++;
        }

    if (err)
        printf("Failed with %d errors!\n", err);
    else
        printf("Passed!\n");

    return 0;
}
