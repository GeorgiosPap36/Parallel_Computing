#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"

int loops;
int shared_counter;

void Raise_counter(){
    for (int i = 0; i < loops; i++)
        shared_counter++;
}

int main(int argc, char* argv[]) {

    loops = strtol(argv[2], NULL, 10);
    shared_counter = 0;

    double start, finish;
    GET_TIME(start);

    Raise_counter();

    GET_TIME(finish);
    double runtime = finish - start;

    printf("----------------------\n");
    printf("Shared counter: %d\n", shared_counter);
    printf("Runtime= %f\n", runtime);

    return 0;
}