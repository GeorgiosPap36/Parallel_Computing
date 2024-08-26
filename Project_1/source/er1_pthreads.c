#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "timer.h"

int thread_count;
long long int n;
long long int darts_in_circle;

double rand_double(unsigned int* seed) 
{
    double random_value = (double)rand_r(seed) / RAND_MAX * 2.0 - 1.0;
    return random_value;
}

void *Estimate_pi(void* rank){
    long my_rank = (long) rank;
    long long int my_darts_in = 0;
    long long int my_n = n/thread_count;
    //printf("Created thread %.2ld, with n: %d\n", my_rank, my_n);
    double x;
    double y;
    double squared_dist;
    unsigned int seed = (unsigned int)(time(NULL) + my_rank);
    for (int i = 0; i < my_n; i++){
        x = rand_double(&seed);
        y = rand_double(&seed);
        //printf("Thread: %.2lu, (%.6f, %.6f), i: %i\n", my_rank, x, y, i);
        squared_dist = x*x + y*y;
        if (squared_dist <= 1.0)
            my_darts_in += 1;
    }
    darts_in_circle += my_darts_in;
    printf("Darts in circle: %lld, rank: %.2ld \n", my_darts_in, my_rank);
}

int main(int argc, char* argv[]) {

    n = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);
    pthread_t *thread_handles = malloc(thread_count * sizeof(pthread_t));
    long thread;
    darts_in_circle = 0;

    double start, finish;
    GET_TIME(start);

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Estimate_pi, (void*) thread);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
    
    double my_pi = 4 * ((double) darts_in_circle)/((double) n);

    GET_TIME(finish);
    double runtime = finish - start;

    printf("----------------------\n");
    printf("N: %lld\n", n);
    printf("Darts in circle: %lld\n", darts_in_circle);
    printf("My pi: %f!\n", my_pi);
    printf("Runtime= %f\n", runtime);

    return 0;
}