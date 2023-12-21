#ifndef _KERNEL_H_
#define _KERNEL_H_
#include <ac_int.h>
#include <ac_channel.h>
#include <ac_float.h>
#include <ac_std_float.h>
#include <ac_math.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef ac_int<32,false> A_t1;
typedef ac_int<32,false> B_t1;
typedef ac_int<32,false> C_t1;
typedef ac_int<512,false> A_t16;
typedef ac_int<64,false> A_t2;
typedef ac_int<512,false> B_t16;
typedef ac_int<64,false> B_t2;
typedef ac_int<512,false> C_t16;
typedef ac_int<128,false> C_t4;
/* Data Type */

/* Helper Function */
inline void host_serialize_A(unsigned int *A_to, unsigned int *A_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1)
      for (int c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // io_L3
        for (int c3 = 0; c3 <= 1; c3 += 1) {
          // io_L2
          for (int c4 = 0; c4 <= 7; c4 += 1)
            for (int c5 = 0; c5 <= 15; c5 += 1)
              A_to[cnt++] = A_from[(16 * c0 + 8 * c3 + c4) * 64 + (16 * c2 + c5)];
        }
      }
}
/* Helper Function */

/* Helper Function */
inline void host_serialize_B(unsigned int *B_to, unsigned int *B_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1)
      for (int c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // io_L3
        for (int c3 = 0; c3 <= 1; c3 += 1) {
          // io_L2
          for (int c4 = 0; c4 <= 7; c4 += 1)
            for (int c5 = 0; c5 <= 15; c5 += 1)
              B_to[cnt++] = B_from[(16 * c1 + 8 * c3 + c4) * 64 + (16 * c2 + c5)];
        }
      }
}
/* Helper Function */

/* Helper Function */
inline void host_deserialize_C(unsigned int *C_to, unsigned int *C_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1) {
      // array
      // io_L3
      for (int c3 = 0; c3 <= 1; c3 += 1) {
        // io_L2
        for (int c4 = 0; c4 <= 1; c4 += 1) {
          // io_L1
          // pe
          for (int c5 = 0; c5 <= 7; c5 += 1)
            for (int c6 = 0; c6 <= 7; c6 += 1)
              C_to[(16 * c0 + 8 * c4 + c5) * 64 + (16 * c1 + 8 * c3 + c6)] = C_from[cnt++];
        }
      }
    }
}
/* Helper Function */

#endif

