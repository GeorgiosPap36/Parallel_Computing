#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "timer.h"

int thread_count;
int loops;
int shared_counter;
pthread_mutex_t lock;

void *Raise_counter_locks(void* rank){
    int local_loops = loops/thread_count;
    for (int i = 0; i < local_loops; i++){
        pthread_mutex_lock(&lock);
        shared_counter++;
        pthread_mutex_unlock(&lock);
    }
}

void *Raise_counter_locks_2(void* rank){
    int local_loops = loops/thread_count;
    int local_counter = 0;
    for (int i = 0; i < local_loops; i++){
        local_counter++;
    }
    pthread_mutex_lock(&lock);
    shared_counter += local_counter;
    pthread_mutex_unlock(&lock);
}



int main(int argc, char* argv[]) {

    thread_count = strtol(argv[1], NULL, 10);
    loops = strtol(argv[2], NULL, 10);
    int opt = strtol(argv[3], NULL, 10);
    shared_counter = 0;

    pthread_t *thread_handles = malloc(thread_count * sizeof(pthread_t));
    long thread;

    double start, finish;
    GET_TIME(start);

    if (opt == 0){
        for (thread = 0; thread < thread_count; thread++)
            pthread_create(&thread_handles[thread], NULL, Raise_counter_locks, (void*) thread);
    }
    else if (opt == 1){
        for (thread = 0; thread < thread_count; thread++)
            pthread_create(&thread_handles[thread], NULL, Raise_counter_locks_2, (void*) thread);
    }

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);

    GET_TIME(finish);
    double runtime = finish - start;

    printf("----------------------\n");
    printf("Shared counter: %d\n", shared_counter);
    printf("Runtime= %f\n", runtime);

    return 0;
}