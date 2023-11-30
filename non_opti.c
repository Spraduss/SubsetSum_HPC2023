/**
gcc -o ssp ssp.c proba.c -lm && ./ssp
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int NB_ITERATION = 1000;        // Number of try before giving up
int SUBSET_SIZE = 8;            // Size of the subset

/**
 * Print a tab
*/
void printSet(unsigned long * set, int size) {
    printf("[");
    for (int i=0 ; i<size ; i++) {
        if (i==size-1) {
            printf("%lu", set[i]);
        } else {
            printf("%lu, ", set[i]);
        }
    }
    printf("]\n");
}

/**
 * Arrange the N elements of ARRAY in random order.
 *  Only effective if N is much smaller than RAND_MAX;
 *  if this may not be the case, use a better random
 *  number generator.
 * source : https://benpfaff.org/writings/clc/shuffle.html
*/
void shuffle(unsigned long *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          unsigned long t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

/**
 * Calcule "stupide" de la solution sur le sous-ensemble
*/
bool compute(unsigned long* set, int borne, unsigned long target) {
    int computed_set_size = pow(2, borne); // size of the set containing all the possibilities ( 2^n )
    unsigned long* computed_set = (unsigned long*)calloc(computed_set_size, sizeof(unsigned long));
    /* Execution of the algorithm presented within the article
    The computed set will contain :
        [target (t), t-w1, t-w2, t-w1-w2, t-w3, t-w1-w3, t-w1-w2-w3, .....] => Stupid solution for subset sum
    We keep filling it until we find a zero or the tab is full
    */
    computed_set[0] = target;
    int cs_index = 1;
    for (int i=0 ; i<borne ; i++){
        unsigned long w_i = set[i];
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
 * Execution of the probabilistic approch
*/
bool keepGoing(unsigned long* set, int size, unsigned long target, int maxIter, int subset_size) {
    srand(time(NULL)); // Initialize the random seed
    int iter = 0;
    bool validate = false;
    // Impact of shuffling task
    clock_t start_shuffle, end_shuffle;
    double cpu_time_used_shuffle = ((double) (0.0)) / CLOCKS_PER_SEC;
    clock_t start_compile, end_compile;
    double cpu_time_used_compile = ((double) (0.0)) / CLOCKS_PER_SEC;
    // We run "maxIter" iterations (we stop befor if we find a solution)
    while (!validate && iter<maxIter) {
        start_shuffle = clock();
        shuffle(set, size);
        end_shuffle = clock();
        cpu_time_used_shuffle += ((double) (end_shuffle - start_shuffle)) / CLOCKS_PER_SEC;
        start_compile = clock();
        validate = compute(set, subset_size, target); // Execute the "stupid" algo on the subset
        end_compile = clock();
        cpu_time_used_compile += ((double) (end_compile - start_compile)) / CLOCKS_PER_SEC;
        iter++;
    }
    if (validate) {
        // Display the solution's subset (may contain useless elements)
        printf("Solution : ");
        printSet(set, subset_size);
    }
    printf("Shuffling time : %f\n", cpu_time_used_shuffle);
    printf("Stupid time : %f\n", cpu_time_used_compile);
    printf("%d, after %i iterations\n", validate, iter);
    return validate;
}

/**
 * Execute the probabilistic approch of the problem
*/
void execution_seq(unsigned long * set, int set_size, unsigned long target) {
    printf("Running with : %i iterations / subset size of %i / set size of : %i\n",NB_ITERATION,SUBSET_SIZE,set_size); // Recap
    bool soluce = keepGoing(set, set_size, target, NB_ITERATION, SUBSET_SIZE); // execution
}

/**
 * Execute the probabilistic approch of the problem
*/
void execution_test_nonOpti(unsigned long * set, int set_size, unsigned long target, int nb_iteration, int subset_size) {
    printf("Running with : %i iterations / subset size of %i / set size of : %i\n",nb_iteration,subset_size,set_size); // Recap
    bool soluce = keepGoing(set, set_size, target, nb_iteration, subset_size); // execution
}

