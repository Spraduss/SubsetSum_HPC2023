#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned long * SET;
unsigned long * SUBSET;
size_t SET_SIZE;
size_t SUBSET_SIZE;
unsigned long TARGET;
int LOOP;

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

/**
 * Calcule "stupide" de la solution sur le sous-ensemble
*/
bool compute() {
    int computed_set_size = pow(2, SUBSET_SIZE); // size of the set containing all the possibilities ( 2^n )
    unsigned long* computed_set = (unsigned long*)calloc(computed_set_size, sizeof(unsigned long));
    /* Execution of the algorithm presented within the article
    The computed set will contain :
        [target (t), t-w1, t-w2, t-w1-w2, t-w3, t-w1-w3, t-w1-w2-w3, .....] => Stupid solution for subset sum
    We keep filling it until we find a zero or the tab is full
    */
    computed_set[0] = TARGET;
    int cs_index = 1;
    for (int i=0 ; i<SUBSET_SIZE ; i++){
        unsigned long w_i = SET[i];
        for (int j=0 ; j<cs_index ; j++){
            computed_set[cs_index+j] = computed_set[j] - w_i;
            if (computed_set[cs_index+j]==0) {
                free(computed_set);
                return true;
            }
        }
        cs_index = 2 * cs_index;
    }
    free(computed_set);
    return false;
}


/**
 * Check if the number "target" is in the tab "indices"
*/
bool isIn(int * indices, unsigned int indices_size, int target) {
    int i;
    for (i=0 ; i<indices_size ; i++){
        if (target==indices[i]) {
            return true;
        }
    }
    return false;
}

/**
 * Randomly pick "SUBSET_SIZE" indices between 0 and "SET_SIZE", and fill the subset with the corresponding values
*/
void getSubset() {
    int * indices = (int*)calloc(SUBSET_SIZE, sizeof(int));
    int i;
    for (i=0 ; i<SUBSET_SIZE ; i++) {
        int randomIndice = rand() % SET_SIZE;
        while (isIn(indices, i, randomIndice)) {
            randomIndice = (randomIndice + 1)%SET_SIZE;
        }
        indices[i] = randomIndice;
        SUBSET[i] = SET[randomIndice];
    }
    //printSet(SUBSET, SUBSET_SIZE);
    free(indices);
}

/**
 * Execution of the probabilistic approch
*/
bool keepGoing() {
    //srand(time(0));
    int iter = 0;
    bool validate = false;
    // Impact of shuffling task
    clock_t start_shuffle, end_shuffle;
    double cpu_time_used_shuffle = ((double) (0.0)) / CLOCKS_PER_SEC;
    clock_t start_compile, end_compile;
    double cpu_time_used_compile = ((double) (0.0)) / CLOCKS_PER_SEC;
    // We run "maxIter" iterations (we stop befor if we find a solution)
    while (!validate && iter<LOOP) {

        start_shuffle = clock();
        SUBSET = (unsigned long*)calloc(SUBSET_SIZE, sizeof(unsigned long));
        getSubset();
        end_shuffle = clock();
        cpu_time_used_shuffle += ((double) (end_shuffle - start_shuffle)) / CLOCKS_PER_SEC;

        start_compile = clock();
        validate = compute(); // Execute the "stupid" algo on the subset
        end_compile = clock();
        cpu_time_used_compile += ((double) (end_compile - start_compile)) / CLOCKS_PER_SEC;

        iter++;
        free(SUBSET);
    }
    if (validate) {
        // Display the solution's subset (may contain useless elements)
        printf("Solution : ");
        printSet(SET, SUBSET_SIZE);
    }
    printf("Shuffling time : %f\n", cpu_time_used_shuffle);
    printf("Stupid time : %f\n", cpu_time_used_compile);
    printf("%d, after %i iterations\n", validate, iter);
    return validate;
}


/**
 * Execute the probabilistic approch of the problem
*/
void execution_test_seq(unsigned long * set, int set_size, unsigned long target, int loop, int subset_size) {
    SET = set;
    SET_SIZE = set_size;
    SUBSET_SIZE = subset_size;
    TARGET = target;
    LOOP = loop;
    printf("Running with : %i iterations / subset size of %li / set size of : %li\n",LOOP, SUBSET_SIZE, SET_SIZE); // Recap
    bool soluce = keepGoing(); // execution
}