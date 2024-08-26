#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "timer.h"

void Get_args(int argc, char* argv[], int* thread_count_p, int* n_p);
void Usage(char* prog_name);
void Gen_matrix(double A[], int n);
void Gen_vector(double x[], int n);
void Print_generated(double A[], double b[], int n);
void Print_solution(double x[], double n);

void Parallel_solution(double A[], double b[], double x[], int n, int thread_count) {
    int chunk_size = n / thread_count;
    int row, col;

    for (row = n - 1; row >= 0; row--){
        x[row] = b[row];
        double temp = 0.0;
        #pragma omp parallel private(temp, col) shared(A, x, row)
        {
            #pragma omp for schedule(static, chunk_size)
            for (col = row + 1; col < n; col++){
                #pragma omp atomic
                temp -= A[row * n + col] * x[col]; 
            }
            #pragma omp atomic
            x[row] += temp;
        }
        #pragma omp single
        x[row] /= A[row * n + row];
    }
}

int main(int argc, char* argv[]) {

    int thread_count;
    int n;
    double* A;
    double* b;
    double* x;

    Get_args(argc, argv, &thread_count, &n);

    A = malloc(n*n*sizeof(double));
    b = malloc(n*sizeof(double));
    x = malloc(n*sizeof(double));
    
    Gen_matrix(A, n);
    Gen_vector(b, n);
    Print_generated(A, b, n);

    double start, finish;
    GET_TIME(start);
    Parallel_solution(A, b, x, n, thread_count);

    GET_TIME(finish);
    double runtime = finish - start;
    Print_solution(x, n);
    printf("----------------------\n");
    printf("Runtime= %f\n", runtime);

    free(A);
    free(b);
    free(x);

   return 0;
} /* main */


/*------------------------------------------------------------------
 * Function:  Get_args
 * Purpose:   Get command line args
 * In args:   argc, argv
 * Out args:  thread_count_p, n_p
 */
void Get_args(int argc, char* argv[], int* thread_count_p, int* n_p)  {
    if (argc != 3) 
        Usage(argv[0]);
    *thread_count_p = strtol(argv[1], NULL, 10);
    *n_p = strtol(argv[2], NULL, 10);
    if (*thread_count_p <= 0 || *n_p <= 0) 
        Usage(argv[0]);
}  /* Get_args */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
    fprintf(stderr, "usage: %s <thread_count> <n>\n", prog_name);
    exit(0);
}  /* Usage */


/*------------------------------------------------------------------
 * Function: Gen_matrix
 * Purpose:  Use the random number generator random to generate
 * an upper triangular matrix A.
 * In args:  n
 * Out arg:  A
 */
void Gen_matrix(double A[], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++){
            if (j >= i)
                A[i*n+j] = random()/((double) RAND_MAX);
            else
                A[i*n+j] = 0;
    }
}  /* Gen_matrix */

/*------------------------------------------------------------------
 * Function: Gen_vector
 * Purpose:  Use the random number generator random to generate
 *    the entries in x
 * In arg:   n
 * Out arg:  A
 */
void Gen_vector(double x[], int n) {
    int i;
    for (i = 0; i < n; i++)
        x[i] = random()/((double) RAND_MAX);
}  /* Gen_vector */

/*------------------------------------------------------------------
 * Function:    Print_generated
 * Purpose:     Print the matrix and the vector b
 * In args:     A, b, n
 */
void Print_generated(double A[], double b[], int n) {
    printf("Linear system: \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%4.2f ", A[i*n + j]);
        printf("%4.2f\n", b[i]);
    }
}  /* Print_generated */

/*------------------------------------------------------------------
 * Function:    Print_solution
 * Purpose:     Print vector x
 * In args:     x, n
 */
void Print_solution(double x[], double n) {
    printf("Linear system solution: \n");
    for (int i = 0; i < n; i++)
        printf("%4.2f ", x[i]);
    printf("\n");
}  /* Print_solution */
