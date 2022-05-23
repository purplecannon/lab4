/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s): Alicia W., Robert Stevens
 * NetID(s): aliciaw0, rsteve2
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);


/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
#define BLOCK_SIZE 8 // 2^5=32 bytes is 8 ints
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M]) {
  int i, j, k, l;
  for (i = 0; i < M; i += BLOCK_SIZE) {
    for (j = 0; j < N; j += BLOCK_SIZE) {
      for (k = 0; k < BLOCK_SIZE; k++) {
        for (l = 0; l < BLOCK_SIZE; l++) {
          B[i+k][j+l] = A[j+l][i+k];
        }
      }
    }
  }
}

//Some simple change made to the above function
char trans_simple_desc[] = "Similar to Robert's 1gen";
void trans_simple(int M, int N, int A[M][N], int B[N][M]) {
  int i, j, ii, jj;

  for(i = 0; i < M; i += BLOCK_SIZE){
    for(j = 0; j < N; j += BLOCK_SIZE){
      for(ii = i; ii < i + BLOCK_SIZE; ii++){
        for(jj = j; jj < j + BLOCK_SIZE; jj++){
          B[jj][ii] = A[ii][jj];
        }  
      }
    }   
  }
}

// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M]) {
  int i, j, tmp;

  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      tmp = A[i][j];
      B[j][i] = tmp;
    }
  }

}

char trans_Alicia32_desc[] = "Alicia32N";
void trans_Alicia32(int M, int N, int A[M][N], int B[N][M]) {
  int temp[8];
  int i, j, ii, jj;

  for(i = 0; i < M; i += BLOCK_SIZE){
    for(j = 0; j < N; j += BLOCK_SIZE){
      for(ii = i; ii < i + BLOCK_SIZE; ii++){
        for(jj = 0; jj < 8; jj++){
          temp[jj] = A[ii][jj + j];
        }
        for(jj = 0; jj < 8; jj++){
          B[jj + j][ii] = temp[jj];
        }
      }
    }
  }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
  /* Register your solution function */
  registerTransFunction(transpose_submit, transpose_submit_desc);

  /* Register any additional transpose functions */
  registerTransFunction(trans, trans_desc);

  /* Register alternetaive functions */
  registerTransFunction(trans_simple, trans_simple_desc);

  //Alicia32
  registerTransFunction(trans_Alicia32, trans_Alicia32_desc);
}


/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M]) {
  int i, j;

  for (i = 0; i < M; i++) {
    for (j = 0; j < N; ++j) {
      if (A[i][j] != B[j][i]) {
        return 0;
      }
    }
  }
  return 1;
}
