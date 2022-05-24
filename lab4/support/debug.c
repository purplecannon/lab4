#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"
#include <string.h>

#define N 64
static int A[N][N];
static int B[N][N];
static int AT[N][N];

/* External function from trans.c */
extern void registerFunctions();

extern trans_func_t func_list[MAX_TRANS_FUNCS];
extern int func_counter;

void printMatrix(int M[N][N]) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf("%03d ", M[i][j]);
        }
        printf("\n");
    }
}

int main() {
    registerFunctions();

    for (int i = 0; i < func_counter; i++) {

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = N*i + j;
                AT[j][i] = N*i + j;
                B[i][j] = -1;
            }
        }

        printf("running %s\n", func_list[i].description);
        (*func_list[i].func_ptr)(N, N, A, B);
        printf("validating %s\n", func_list[i].description);
        int valid = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if(B[i][j] != AT[i][j]) {
                    printf("failed at B[%d][%d]. Expected: %d but found %d. Original value was %d\n", i, j, AT[i][j], B[i][j], A[i][j]);
                    valid = 0;
                    break;
                    
                }
            }
        }
        if(valid){
            printf("success\n");
        } else {
            printf("vis:\n");
            printf("A:\n");
            printMatrix(A);
            printf("B:\n");
            printMatrix(B);
        }
        
    }
}