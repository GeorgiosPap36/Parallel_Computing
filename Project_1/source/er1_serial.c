#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"

int thread_count;
long long int n;
long long int darts_in_circle;

double rand_double(unsigned int* seed) 
{
    double random_value = (double)rand_r(seed) / RAND_MAX * 2.0 - 1.0;
    return random_value;
}

int main(int argc, char* argv[]) {

    n = strtol(argv[1], NULL, 10);
    darts_in_circle = 0;

    double start, finish;
    GET_TIME(start);

    double x;
    double y;
    double squared_dist;
    unsigned int seed = (unsigned int)(time(NULL));
    for (int i = 0; i < n; i++){
        x = rand_double(&seed);
        y = rand_double(&seed);
        //printf("(%.6f, %.6f)\n", x, y);
        squared_dist = x*x + y*y;
        if (squared_dist <= 1.0)
            darts_in_circle += 1;
    }

    double my_pi = 4 * ((double) darts_in_circle)/((double) n);

    GET_TIME(finish);
    double runtime = finish - start;

    printf("----------------------\n");
    printf("N: %lld\n", n);
    printf("Darts in circle: %lld\n", darts_in_circle);
    printf("My pi: %f!\n", my_pi);
    printf("Runtime = %f\n", runtime);

    return 0;
}