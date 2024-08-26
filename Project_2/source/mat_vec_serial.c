#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "timer.h"

double rand_double(unsigned int* seed) 
{
    double random_value = (double)rand_r(seed) / RAND_MAX;
    return random_value;
}

void Gen_matrix(double A[], int rows, int cols) {
    unsigned int seed = (unsigned int)(time(NULL));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
                A[i*cols+j] = rand_double(&seed);
}

void Gen_vector(double b[], int n) {
    unsigned int seed = (unsigned int)(2*time(NULL));
    for (int i = 0; i < n; i++)
            b[i] = rand_double(&seed);
}

void Init_vector(double b[], int n, double val){
    for (int i = 0; i < n; i++)
        b[i] = val;
}

void Print_matrix(double A[], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%4.3f ", A[i*cols + j]);
    printf("\n");
    }
}

void Print_vector(double b[], int n) {
    for (int i = 0; i < n; i++)
        printf("%4.3f ", b[i]);
    printf("\n");
}

int main(int argc, char *argv[]){
    int n;
    double* A; //n x n matrix
    double* b; //n x 1 vector
    double* x; //A * b = x, n x n vector

    double start, finish, runtime;

    printf("Enter number of rows/cols: ");
    fflush(stdout);
    scanf("%d", &n);
    //n = 15000;
    A = malloc(n*n*sizeof(double));
    b = malloc(n*sizeof(double));
    x = malloc(n*sizeof(double));
    Gen_matrix(A, n, n);
    Gen_vector(b, n);
    Init_vector(x, n, 0.0);

    ////START TIMER////
    GET_TIME(start);

    //A and b multiplication
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            x[i] += A[i*n+j] * b[j];
        }
        //printf("x[%d] = %f\n", i, x[i]);
    }

    GET_TIME(finish);
    ////END TIMER////

    runtime = finish - start;

    /*printf("A: \n");
    Print_matrix(A, n, n);
    printf("---------------------\n");
    printf("b: \n");
    Print_vector(b, n);
    printf("---------------------\n");
    printf("x: \n");
    Print_vector(x, n);
    printf("---------------------\n");*/
    printf("Runtime = %f\n", runtime);

    return 0;
}