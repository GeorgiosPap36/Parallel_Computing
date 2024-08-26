#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "mpi.h"
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
    int n, rank, size;
    double* A; //n x n matrix
    double* b; //n x 1 vector
    double* x; //A * b = x, n x n vector

    int process_cols; //number of cols for each process
    double *process_A; //n x process_cols sub matrix of A
    double *process_b; //process_cols x 1 sub vector of b
    double *process_x; //process_A * process_b = process_x, n x n local vector

    double start, finish, runtime, totalRuntime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        printf("Enter number of rows/cols: ");
        fflush(stdout);
        scanf("%d", &n);
        A = malloc(n*n*sizeof(double));
        b = malloc(n*sizeof(double));
        x = malloc(n*sizeof(double));
        Gen_matrix(A, n, n);
        Gen_vector(b, n);
        Init_vector(x, n, 0.0);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); //broadcast n
    process_cols = n/size;

    MPI_Datatype col_send, col_recv, send_cols_type, recv_cols_type;
    
    //Send data types
    MPI_Type_vector(n, 1, n, MPI_DOUBLE, &col_send);
    MPI_Type_commit(&col_send);
    MPI_Type_create_resized(col_send, 0, 1*sizeof(double), &send_cols_type);
    MPI_Type_commit(&send_cols_type);

    //Receive data types
    MPI_Type_vector(n, 1, n/size, MPI_DOUBLE, &col_recv);
    MPI_Type_commit(&col_recv);
    MPI_Type_create_resized(col_recv, 0, 1*sizeof(double), &recv_cols_type);
    MPI_Type_commit(&recv_cols_type);

    ////START TIMER////
    MPI_Barrier(MPI_COMM_WORLD);
    //start = MPI_Wtime(); //Start timer
    GET_TIME(start);

    process_x = malloc(n*sizeof(double));
    Init_vector(process_x, n, 0.0);

    process_A = malloc(process_cols*n*sizeof(double)); 
    process_b = malloc(process_cols*sizeof(double));
    //MPI_Barrier(MPI_COMM_WORLD);

    MPI_Scatter(b, process_cols, MPI_DOUBLE, process_b, process_cols, MPI_DOUBLE, 0, MPI_COMM_WORLD); //scatter b to all the processes
    MPI_Scatter(A, process_cols, send_cols_type, process_A, process_cols, recv_cols_type, 0, MPI_COMM_WORLD); //scatter A to all the processes

    /*if (rank == 1){
        printf("my_A: \n");
        Print_matrix(process_A, n, process_cols);       
        printf("---------------------\n");
        printf("my_b: \n");
        Print_vector(process_b, process_cols);
        printf("---------------------\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);*/

    //Local A and local b multiplication
    for (int i = 0; i < n; i++){
        for (int j = 0; j < process_cols; j++){
            process_x[i] += process_A[i*process_cols+j] * process_b[j];
        }
    }

    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(process_x, x, n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 

    GET_TIME(finish);
    //finish = MPI_Wtime(); //End timer
    MPI_Barrier(MPI_COMM_WORLD);
    ////END TIMER////

    runtime = finish - start;
    MPI_Reduce(&runtime, &totalRuntime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); //Average the runtime of all the processes

    if (rank == 0) {
        /*printf("A: \n");
        Print_matrix(A, n, n);
        printf("---------------------\n");
        printf("b: \n");
        Print_vector(b, n);
        printf("---------------------\n");
        printf("x: \n");
        Print_vector(x, n);
        printf("---------------------\n");*/
        printf("Runtime = %f\n", totalRuntime / size);
    }

    MPI_Type_free(&col_send);
    MPI_Type_free(&col_recv);
    MPI_Type_free(&send_cols_type);
    MPI_Type_free(&recv_cols_type);

    MPI_Finalize();

    return 0;
}