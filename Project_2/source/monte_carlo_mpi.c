#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

double rand_double(unsigned int* seed) 
{
    double random_value = (double)rand_r(seed) / RAND_MAX * 2.0 - 1.0;
    return random_value;
}

int main(int argc, char *argv[]){
    int size, rank;
    long long int n; //number of throws
    long long int process_throws; //number of throws that each process will perform
    long long int darts_in_circle;
    long long int process_darts;
    double my_pi;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        printf("Enter number of throws:");
        fflush(stdout);
        scanf("%lld", &n);
        //n = 1000000000;
        process_throws = n/size;
        darts_in_circle = 0;
        process_darts = 0;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD); //broadcast n
    MPI_Bcast(&process_throws, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD); //broadcast process_throws
    MPI_Bcast(&process_darts, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD); //broadcast process_throws
    MPI_Barrier(MPI_COMM_WORLD);

    /*//Prints, so that I know that the n distribution is correct.
    int sendI;
    char message[1000];
    MPI_Status status;
    sprintf(message, "Process %d of %d. Total throws: %lld, my throws: %lld, my darts in circle: %lld.\n", rank, size, n, process_throws, process_darts);
    MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 50, MPI_COMM_WORLD);
    if (rank == 0){
        //printf("\n");
        for (sendI = 0; sendI < size; sendI++){
            MPI_Recv(message, 100, MPI_CHAR, sendI, 50, MPI_COMM_WORLD, &status);
            printf("%s\n", message);
        }
    }
    //------//*/

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime(); //Start timer

    //Monte Carlo
    double x;
    double y;
    double squared_dist;
    unsigned int seed = (unsigned int)(time(NULL) + rank);
    for (int i = 0; i < process_throws; i++){
        x = rand_double(&seed);
        y = rand_double(&seed);
        squared_dist = x*x + y*y;
        if (squared_dist <= 1.0)
            process_darts += 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&process_darts, &darts_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    /*//Prints, so that I know that the n distribution is correct.
    sprintf(message, "Process %d of %d. Total throws: %lld, my throws: %lld, my darts in circle: %lld.\n", rank, size, n, process_throws, process_darts);
    MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 50, MPI_COMM_WORLD);
    if (rank == 0){

        printf("--------------------RESULTS--------------------\n");
        //printf("\n");
        for (sendI = 0; sendI < size; sendI++){
            MPI_Recv(message, 100, MPI_CHAR, sendI, 50, MPI_COMM_WORLD, &status);
            printf("%s\n", message);
        }
    }
    //------//*/
    
    double end = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    double runtime = end - start;
    double totalRuntime;
    MPI_Reduce(&runtime, &totalRuntime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        my_pi = 4 * darts_in_circle/(long double)n;
        printf("My pi: %f!\n", my_pi);
        printf("Runtime = %f\n", totalRuntime / size);
    }

    MPI_Finalize();

    return 0;
}