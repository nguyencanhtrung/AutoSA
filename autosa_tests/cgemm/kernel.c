#include "kernel.h"

int main(int argc, char **argv) {
  data_t A1[I][K], B1[J][K],A2[I][K], B2[J][K],A3[I][K], B3[J][K],A4[I][K], B4[J][K], C[I][J], C_golden[I][J];  

  for (int i = 0; i < I; i++) 
    for (int k = 0; k < K; k++) {
      A1[i][k] = (data_t)rand() / RAND_MAX;
      A2[i][k] = A1[i][k];
      A3[i][k] = A1[i][k];
      A4[i][k] = A1[i][k];
    }

  for (int j = 0; j < J; j++)
    for (int k = 0; k < K; k++) {
      B1[j][k] = (data_t)rand() / RAND_MAX;
      B2[j][k] = B1[j][k];
      B3[j][k] = B1[j][k];
      B4[j][k] = B1[j][k];      
    }

#pragma scop
  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      //C[i][j] = 0;
      for (int k = 0; k < K; k++) {     
        C[i][j] = ((C[i][j]>>8) + ((A1[i][k]>>8) * (B1[j][k]>>8) - (A2[i][k]&0xff) * (B2[j][k]&0xff)) << 8) 
        & ((C[i][j]&0xff) + ((A3[i][k]>>8) * (B3[j][k]&0xff) + (A4[i][k]&0xff) * (B4[j][k] << 8)) & 0xff);
      }
    }
#pragma endscop

  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      C_golden[i][j] = 0;
      for (int k = 0; k < K; k++) {
        C_golden[i][j] = ((C_golden[i][j]>>8) + ((A1[i][k]>>8) * (B1[j][k]>>8) - (A1[i][k]&0xff) * (B1[j][k]&0xff)) << 8) 
        & ((C_golden[i][j]&0xff) + ((A1[i][k]>>8) * (B1[j][k]&0xff) + (A1[i][k]&0xff) * (B1[j][k] << 8)) & 0xff);
      }
    }

  int err = 0;
  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      if (C_golden[i][j] != C[i][j])
        err++;
    }

  if (err)
    printf("Failed with %d errors!\n", err);
  else
    printf("Passed!\n");

  return 0;
}
