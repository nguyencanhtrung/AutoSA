#include <vector>
#include <cstdlib>
#include "kernel_kernel.h"
#include "kernel_kernel_hw.h"
#include <mc_scverify.h>

#include "kernel.h"

CCS_MAIN(int argc, char **argv) {
  data_t A[I_P][K_P], B[J_P][K_P], C[I_P][J_P], C_golden[I_P][J_P]; // gemm0,3

  for (int i = 0; i < I_P; i++) 
    for (int k = 0; k < K_P; k++) {
      //A[i][k] = (data_t)rand() / RAND_MAX;
      A[i][k] = (data_t)1;
    }

  for (int j = 0; j < J_P; j++)
    for (int k = 0; k < K_P; k++) {
      //B[j][k] = (data_t)rand() / RAND_MAX;
      B[j][k] = (data_t)1;
    }

  {
    // Allocate memory in host memory
    unsigned int *dev_A_unserialized = (unsigned int *)malloc((64) * (64) * sizeof(unsigned int));
    unsigned int *dev_A = (unsigned int *)malloc(16384 * sizeof(unsigned int));
    unsigned int *dev_B_unserialized = (unsigned int *)malloc((64) * (64) * sizeof(unsigned int));
    unsigned int *dev_B = (unsigned int *)malloc(16384 * sizeof(unsigned int));
    unsigned int *dev_C_unserialized = (unsigned int *)malloc((64) * (64) * sizeof(unsigned int));
    unsigned int *dev_C = (unsigned int *)malloc(4096 * sizeof(unsigned int));

    // Initialize host buffers
    memcpy(dev_A_unserialized, A, (64) * (64) * sizeof(unsigned int));
    memcpy(dev_B_unserialized, B, (64) * (64) * sizeof(unsigned int));
    memcpy(dev_C_unserialized, C, (64) * (64) * sizeof(unsigned int));
    host_serialize_A(dev_A, dev_A_unserialized);
    host_serialize_B(dev_B, dev_B_unserialized);

    // Allocate buffers in device memory
    std::vector<A_t16 *> buffer_A;
    std::vector<B_t16 *> buffer_B;
    std::vector<C_t16 *> buffer_C;
    for (int i = 0; i < 1; i++) {
      A_t16 *buffer_A_tmp = (A_t16 *)malloc(16384 / 16 * sizeof(A_t16));
      buffer_A.push_back(buffer_A_tmp);
    }
    for (int i = 0; i < 1; i++) {
      B_t16 *buffer_B_tmp = (B_t16 *)malloc(16384 / 16 * sizeof(B_t16));
      buffer_B.push_back(buffer_B_tmp);
    }
    for (int i = 0; i < 1; i++) {
      C_t16 *buffer_C_tmp = (C_t16 *)malloc(4096 / 16 * sizeof(C_t16));
      buffer_C.push_back(buffer_C_tmp);
    }

    for (int i = 0; i < 1; i++) {
      for (int c0 = 0; c0 < 16384 / 16; c0++) {
        A_t16 tmp;
        for (int c1 = 0; c1 < 16; c1++) {
          tmp.set_slc(c1 * 32, (A_t1)dev_A[c0 * 16 + c1]);
        }
        buffer_A[i][c0] = tmp;
      }
    }

    for (int i = 0; i < 1; i++) {
      for (int c0 = 0; c0 < 16384 / 16; c0++) {
        B_t16 tmp;
        for (int c1 = 0; c1 < 16; c1++) {
          tmp.set_slc(c1 * 32, (B_t1)dev_B[c0 * 16 + c1]);
        }
        buffer_B[i][c0] = tmp;
      }
    }

    {
      // Launch the kernel
      kernel0 kernel0_inst;
      kernel0_inst.run(buffer_A[0], buffer_B[0], buffer_C[0]);
    }
    for (int i = 0; i < 1; i++) {
      for (int c0 = 0; c0 < 4096 / 16; c0++) {
        C_t16 tmp = buffer_C[i][c0];
        for (int c1 = 0; c1 < 16; c1++) {
          dev_C[c0 * 16 + c1] = (unsigned int)tmp.slc<32>(32 * c1);
        }
      }
    }

    host_deserialize_C(dev_C_unserialized, dev_C);
    // Restore data from host buffers
    memcpy(C, dev_C_unserialized, (64) * (64) * sizeof(unsigned int));

    // Clean up resources
    for (int i = 0; i < 1; i++) {
      free(buffer_A[i]);
    }
    for (int i = 0; i < 1; i++) {
      free(buffer_B[i]);
    }
    for (int i = 0; i < 1; i++) {
      free(buffer_C[i]);
    }
    free(dev_A);
    free(dev_A_unserialized);
    free(dev_B);
    free(dev_B_unserialized);
    free(dev_C);
    free(dev_C_unserialized);
  }

  for (int i = 0; i < I_P; i++)
    for (int j = 0; j < J_P; j++) {
      C_golden[i][j] = 0;
      for (int k = 0; k < K_P; k++) {
        C_golden[i][j] = C_golden[i][j] + A[i][k] * B[j][k];
      }
    }

  int err = 0;
  for (int i = 0; i < I_P; i++)
    for (int j = 0; j < J_P; j++) {
      if (fabs((float)C_golden[i][j] - (float)C[i][j]) > 0.001)
        err++;
    }

  if (err)
    printf("Failed with %d errors!\n", err);
  else
    printf("Passed!\n");

  return 0;
}
