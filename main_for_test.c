
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

void launch_seq(unsigned long* SET, unsigned long TARGET, int SET_SIZE, int SUBSET_SIZE, int LOOP) {
    // Exécution séquentielle
    //execution_test_seq(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
}

void launch_p(unsigned long* SET, unsigned long TARGET, int SET_SIZE, int SUBSET_SIZE, int LOOP) {
    // Exécution parallèle 
    execution_test_p(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
}

int main() {
    unsigned long TARGET;
    size_t SET_SIZE = 10000;
    size_t SUBSET_SIZE = 10;
    int LOOP = 10;
    unsigned long max = 300000000;

    unsigned long * SET = (unsigned long*)calloc(SET_SIZE, sizeof(unsigned long));

    // Remplissage du tableau avec des nombres
    int i;
    for (i=0 ; i<SET_SIZE ; i++) {
        SET[i] = 2 * (rand()%max);
    }
    TARGET = SET[12] + SET[23] + SET[84] + SET[90] + SET[0] + SET[99];
    printf("Solution possible : %lu, %lu, %lu, %lu, %lu, %lu, => %lu\n\n", SET[12], SET[23], SET[84], SET[90], SET[0], SET[99], TARGET);

    int nbOfThreads = 5;
    set_number_of_threads(nbOfThreads);
    
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    //launch_seq(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
    launch_p(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC)/nbOfThreads;
    printf("time : %f\n\n", cpu_time_used);
    free(SET);
}

