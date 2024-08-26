#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <cuda.h>

void Get_args(int argc, char* argv[], int* threads, int* blocks, int* r, int* c)  {
    *r = strtol(argv[1], NULL, 10);
    *c = strtol(argv[2], NULL, 10);
    *threads = strtol(argv[3], NULL, 10);
    *blocks = strtol(argv[4], NULL, 10);
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

void Correct_multiplication(int rows, int cols, double* A, double* B, double* C){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            C[i*rows + j] = 0;

            for (int k = 0; k < cols; k++) {
                C[i*rows + j] += A[i*cols + k] * B[k*rows + j];
            }
        }
    }
}

int Check_correctness(double* C, double* C_correct, int rows){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++){
            if (abs(C[i*rows + j] - C_correct[i*rows + j]) > 0.0000000001){
                printf("\n(%d, %d), (%f, %f)\n", i, j, C[i*rows + j], C_correct[i*rows + j]);
                return 0;
            }
        }
    }
    return 1;
}

__global__ void Mult_mats(int rows, int cols, double* device_A, double* device_B, double* device_C){
    int my_el = blockDim.x * blockIdx.x + threadIdx.x;

    if (my_el < rows * rows){
        int r = my_el / rows;
        int c = my_el % rows;
        device_C[my_el] = 0;
        for (int i = 0; i < cols; i++){
            device_C[my_el] += device_A[r * cols + i] * device_B[i * rows + c];
        }
    }   
}

int main(int argc, char* argv[]) {
    int threads_per_block, block_count, rows, cols;

    double* A; //rows x cols matrix
    double* B; //cols x rows matrix
    double* C; //A x B = C, rows x rows matrix
    //double* C_correct; // A x B = C_correct, the correct result of the multiplication
    
    double* device_A;
    double* device_B;
    double* device_C;

    clock_t start, finish;

    Get_args(argc, argv, &threads_per_block, &block_count, &rows, &cols);
    
    //Allocate memory on CPU for the matrices
    A = (double*) malloc(rows*cols*sizeof(double));
    B = (double*) malloc(cols*rows*sizeof(double));
    C = (double*) malloc(rows*rows*sizeof(double));
    //C_correct = (double*) malloc(rows*rows*sizeof(double));

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

    //Allocate memory on the GPU for the matrices
    cudaMalloc((void**)&device_A, rows*cols*sizeof(double));
    cudaMalloc((void**)&device_B, cols*rows*sizeof(double));
    cudaMalloc((void**)&device_C, rows*rows*sizeof(double));

    //Copy the values of matrices A and B from the CPU to the GPU
    cudaMemcpy(device_A, A, rows*cols*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_B, B, cols*rows*sizeof(double), cudaMemcpyHostToDevice);

    start = clock(); //start timer

    Mult_mats <<<block_count, threads_per_block>>>(rows, cols, device_A, device_B, device_C); 
    cudaDeviceSynchronize();

    finish = clock(); //end timer

    //Copy the values of C from the GPU to the CPU
    cudaMemcpy(C, device_C, rows*rows*sizeof(double), cudaMemcpyDeviceToHost);

    printf("\n----------------------\n");
    //printf("\nMatrix C(%d x %d): \n", rows, rows);
    //Print_matrix(C, rows, rows);

    //Correct_multiplication(rows, cols, A, B, C_correct);
    //printf("\nMatrix C_correct(%d x %d): \n", rows, rows);
    //Print_matrix(C_correct, rows, rows);

    //printf("\nCUDA solution is: %d (0 is incorrect, 1 is correct)\n", Check_correctness(C, C_correct, rows));

    printf("\n----------------------\n");
    printf("Runtime = %f\n", (double)(finish - start) / CLOCKS_PER_SEC);

    //Free memory
    free(A);
    free(B);
    free(C);
    //free(C_correct);
    cudaFree(device_A);
    cudaFree(device_B);
    cudaFree(device_C);

    return 0;
}