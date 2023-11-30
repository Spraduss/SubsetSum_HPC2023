
#include "proba.h"
#include "proba_parallel.h"
#include "non_opti.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    unsigned int NTHREADS = 10;
    unsigned int set_size = 1000;
    unsigned int loop = 10000000;
    unsigned int subset_size = 10;
    unsigned long max = 300000000;
    unsigned long set[set_size];
    clock_t start, end;
    double cpu_time_used;

    // Remplissage du tableau avec des nombres
    //srand(time(NULL)); // Reset the seed
    int i;
    #pragma omp parallel for private(i) num_threads(NTHREADS)
    for (i=0 ; i<1000 ; i++) {
        set[i] = rand()%max;
    }
    unsigned long target = set[12] + set[123] + set[84] + set[990] + set[0] + set[999];
    printf("Solution possible : %lu, %lu, %lu, %lu, %lu, %lu, => %lu\n\n", set[12], set[123], set[84], set[990], set[0], set[999], target);
/*
    start = clock();
    execution_test_nonOpti(set, set_size, target, loop, subset_size);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time // : %f\n\n", cpu_time_used);
/*
    start = clock();
    execution_test_parallel(set, set_size, target, loop, subset_size);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time // : %f\n\n", cpu_time_used);
*/
    start = clock();
    execution_test_seq(set, set_size, target, loop, subset_size);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time SEQ : %f\n", cpu_time_used);

}

