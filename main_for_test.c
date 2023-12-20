
#include <time.h>
#include "proba.h"
#include "proba_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/**
 * Print a tab
*/
void printSet(unsigned long * tab, int n) {
    printf("[");
    for (int i=0 ; i<n ; i++) {
        if (i==n-1) {
            printf("%lu", tab[i]);
        } else {
            printf("%lu, ", tab[i]);
        }
    }
    printf("]\n");
}

int main() {
    unsigned long TARGET;
    size_t SET_SIZE = 1000000;
    size_t SUBSET_SIZE = 15;
    int LOOP = 10000000;
    unsigned long max = 300000000;
    int nbOfThreads = omp_get_max_threads();

    unsigned long * SET = (unsigned long*)calloc(SET_SIZE, sizeof(unsigned long));

    // Remplissage du tableau avec des nombres
    int i;
    srand(time(NULL));
    #pragma omp parallel for private(i) num_threads(nbOfThreads)
    for (i=0 ; i<SET_SIZE ; i++) {
        SET[i] = 2 * (rand()%max);
    }
    TARGET = SET[12] + SET[23] + SET[84] + SET[90] + SET[0] + SET[99];
    printf("Solution possible : %lu, %lu, %lu, %lu, %lu, %lu, => %lu\n\n", SET[12], SET[23], SET[84], SET[90], SET[0], SET[99], TARGET);

    set_number_of_threads(nbOfThreads);
    
    int repetitionParExec = 1;
    clock_t start_p, start, end_p, end;
    double t_p, t;

    /*************************************************************/
    start_p = clock();
    for (int i=0;i<repetitionParExec;i++) {
        execution_test_p(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
    }
    end_p = clock();
    t_p = (((double) (end_p - start_p)) / CLOCKS_PER_SEC)/nbOfThreads / repetitionParExec;
    /*************************************************************/
    start = clock();
    for (int i=0;i<repetitionParExec;i++){
        execution_test_seq(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
    }
    end = clock();
    t = (((double) (end - start)) / CLOCKS_PER_SEC) / repetitionParExec;
    /*************************************************************/
    printf("\ntime Seq: %f\n", t);
    printf("time // : %f\n\n", t_p);
    free(SET);
}

