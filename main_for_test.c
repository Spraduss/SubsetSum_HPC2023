
#include <time.h>
#include "proba.h"
#include "proba_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Lance une exécution en parallèle "repetitionParExec" fois
 * et renvoi le temps d'exécution
*/
double parallel_exe(unsigned long* SET, unsigned long TARGET, size_t SET_SIZE, 
                    size_t SUBSET_SIZE, int LOOP, int repetitionParExec) {
    clock_t start_p = clock();
    for (int i=0;i<repetitionParExec;i++) {
        execution_test_p(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
    }
    clock_t end_p = clock();
    double t_p = (((double) (end_p - start_p)) / CLOCKS_PER_SEC) / repetitionParExec;
    return t_p;
}

/**
 * Lance une exécution en séquentiel "repetitionParExec" fois
 * et renvoi le temps d'exécution
*/
double normal_exe(unsigned long* SET, unsigned long TARGET, size_t SET_SIZE, 
                    size_t SUBSET_SIZE, int LOOP, int repetitionParExec) {
    clock_t start = clock();
    for (int i=0;i<repetitionParExec;i++) {
        execution_test_seq(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP);
    }
    clock_t end = clock();
    double t = (((double) (end - start)) / CLOCKS_PER_SEC) / repetitionParExec;
    return t;
}

// Renvoi un booleen indiquant si "target" se trouve dans le tableau "indices"
bool indiceInIndices(int* indices, size_t n, int target) {
    for (int i=0 ; i<n ; i++){
        if (target==indices[i]) return true;
    }
    return false;
}

// Choisi une "target" de manière aléatoire
unsigned long getTarget(unsigned long* set, size_t n) {
    int* indices = (int*)calloc(10, sizeof(int));
    unsigned long sum = 0;
    for (int i=0 ; i<10 ; i++){
        int indice = rand()%n;
        while (indiceInIndices(indices, 10, indice)) {
            indice += 1;
        }
        indices[i] = indice;
        sum += set[indice];
    }
    return sum;
}

// Initialise le set
void setSET(unsigned long* SET, size_t size, unsigned long max, int nbOfThreads) {
    int i;
    #pragma omp parallel for private(i) num_threads(nbOfThreads)
    for (i=0 ; i<size ; i++) {
        SET[i] = 2 * (rand()%max);
    }
}

int main() {
    srand(time(NULL));
    int nbOfThreads = omp_get_max_threads();
    set_number_of_threads(nbOfThreads);

    unsigned long TARGET;
    size_t SET_SIZE = 500000;
    size_t SUBSET_SIZE = 15;
    int LOOP = 10000000;
    unsigned long max = 300000000;
    size_t evol_set[7] = {100,1000,10000,100000,500000,1000000};
    size_t evol_subset [11] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    for (int k=0 ; k<7 ; k++){
        //SUBSET_SIZE = evol_subset[k]; // change max value of k to 11
        SET_SIZE = evol_set[k]; // change max value of k to 7
        unsigned long * SET = (unsigned long*)calloc(SET_SIZE, sizeof(unsigned long));
        setSET(SET, SET_SIZE, max, nbOfThreads);
        TARGET = getTarget(SET, SET_SIZE);
        int repetitionParExec = 5;
        /*************************************************************/
        double t_p = parallel_exe(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP, repetitionParExec) / nbOfThreads;
        /*************************************************************/
        double t = normal_exe(SET, TARGET, SET_SIZE, SUBSET_SIZE, LOOP, repetitionParExec);
        /*************************************************************/
        printf("\n/*************************************************************/\nSUBSET SIZE : %li\n", SUBSET_SIZE);
        printf("time Seq: %f\n", t);
        printf("time // : %f\n/*************************************************************/\n\n", t_p);
        free(SET);
    }

    
}

