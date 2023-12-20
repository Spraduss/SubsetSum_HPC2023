
#include <time.h>
#include "proba.h"
#include "proba_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Print a tab
*/
void printSet(double * tab, int n) {
    printf("[");
    for (int i=0 ; i<n ; i++) {
        if (i==n-1) {
            printf("%f", tab[i]);
        } else {
            printf("%f, ", tab[i]);
        }
    }
    printf("]\n");
}

bool canBeFound(int* indices, size_t n, int target) {
    for (int i=0 ; i<n ; i++){
        if (indices[i]==target) return true;
    }
    return false;
}

unsigned long getTarget(unsigned long* set, size_t n, int nbOfSum) {
    int* listOfIndices = (int*)calloc(n, sizeof(int));
    unsigned long sum = 0;
    for (int i=0 ; i<n ; i++){
        int ind = rand()%n;
        while (canBeFound(listOfIndices, n, ind)) {
            ind = (ind + 1)%n;
        }
        listOfIndices[i] = ind;
        sum += set[ind];
    }
    free(listOfIndices);
    return sum;
}

int main() {
    printf("STARTING");
    srand(time(NULL));
    
    unsigned long TARGET;
    size_t SET_SIZE;
    size_t SUBSET_SIZE = 15;
    int LOOP = 10000000;
    unsigned long max = 300000000;

    int nbOfThreads = omp_get_max_threads();
    set_number_of_threads(nbOfThreads);
    
    int repetitionParExec = 1; // TODO 5
    clock_t start_p, start, end_p, end;
    double t_p, t;
    size_t evol_set[7] = {100,1000,10000,100000,1000000,500000};
    size_t evol_subset [11] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    int nbEvolSet = 1; //TODO 7
    int nbEvolSubSet = 10;
    double* parallel_time = (double*)calloc(nbEvolSet, sizeof(double));
    double* normal_time = (double*)calloc(nbEvolSet, sizeof(double));

    for (int i=0 ; i<nbEvolSet ; i++){
        printf("IN MAIN LOOP");
        SET_SIZE = evol_set[i];// Remplissage du tableau avec des nombres
        unsigned long * SET = (unsigned long*)calloc(SET_SIZE, sizeof(unsigned long));
        for (int k=0 ; k<SET_SIZE ; k++) {
            SET[k] = 2 * (rand()%max);
        }
        TARGET = getTarget(SET, SET_SIZE, 10);
        printf("Target : %li", TARGET);

        /*************************************************************/
        start_p = clock();
        for (int i=0;i<repetitionParExec;i++) {
            execution_test_p(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
        }
        end_p = clock();
        parallel_time[i] = (((double) (end_p - start_p)) / CLOCKS_PER_SEC)/nbOfThreads / repetitionParExec;
        /*************************************************************/
        start = clock();
        for (int i=0;i<repetitionParExec;i++){
            execution_test_seq(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
        }
        end = clock();
        normal_time[i] = (((double) (end - start)) / CLOCKS_PER_SEC) / repetitionParExec;
        free(SET);
    }
    printf("Parallel time : ");
    printSet(parallel_time, nbEvolSet);
    printf("Normal time : ");
    printSet(normal_time, nbEvolSet);
    
}

