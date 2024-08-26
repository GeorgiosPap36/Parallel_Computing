#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void Get_args(int argc, char* argv[], int* r, int* c)  {
    *r = strtol(argv[1], NULL, 10);
    *c = strtol(argv[2], NULL, 10);
}

double rand_double() 
{
    double random_value = (double)rand() / RAND_MAX;
    return random_value;
}

void Gen_matrix(double* A, int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
                A[i*cols+j] = rand_double();
}

void Print_matrix(double* A, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%4.3f ", A[i*cols + j]);
        printf("\n");
    }
}

void Mat_mult(int rows, int cols, double* A, double* B, double* C){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            C[i*rows + j] = 0;

            for (int k = 0; k < cols; k++) {
                C[i*rows + j] += A[i*cols + k] * B[k*rows + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int rows, cols;

    double* A; //rows x cols matrix
    double* B; //cols x rows matrix
    double* C; //A x B = C, rows x rows matrix

    clock_t start, finish;

    Get_args(argc, argv, &rows, &cols);

    //Allocate memory for the matrices
    A = (double*) malloc(rows*cols*sizeof(double));
    B = (double*) malloc(cols*rows*sizeof(double));
    C = (double*) malloc(rows*rows*sizeof(double));

    //Set the seed, for the generation of the random matrices
    srand(time(NULL)); 

    //Generate A matrix
    Gen_matrix(A, rows, cols);
    //printf("\nMatrix A(%d x %d): \n", rows, cols);
    //Print_matrix(A, rows, cols);

    //Generate B matrix
    Gen_matrix(B, cols, rows);
    //printf("\nMatrix B(%d x %d): \n", cols, rows);
    //Print_matrix(B, cols, rows);

    start = clock(); //start timer

    Mat_mult(rows, cols, A, B, C);

    finish = clock(); //end timer

    printf("\n----------------------\n");
    //printf("\nMatrix C(%d x %d): \n", rows, rows);
    //Print_matrix(C, rows, rows);

    printf("\n----------------------\n");
    printf("Runtime = %f\n", (double)(finish - start) / CLOCKS_PER_SEC);

    return 0;
}