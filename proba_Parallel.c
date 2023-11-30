/**
gcc -o ssp ssp.c proba.c -lm && ./ssp
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

int _NB_ITERATION = 1000;        // Number of try before giving up
int _SUBSET_SIZE = 8;            // Size of the subset
unsigned int _NTHREADS = 10;

/**
 * Print a tab
*/
void printSet_p(unsigned long * set, int size) {
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
void shuffle_p(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

bool isIn_p(unsigned int * indices, unsigned int indices_size, unsigned int target) {
    int i;
    for (i=0 ; i<indices_size ; i++){
        if (target==indices[i]) return true;
    }
    return false;
}

void getSubset_p(unsigned long * set, unsigned long * subset, unsigned int set_size, unsigned int subset_size) {
    unsigned int indices[subset_size];
    int i;
    #pragma omp parallel for private(i) num_threads(_NTHREADS)
    for (i=0 ; i<subset_size ; i++) {
        unsigned int randomIndice = rand()%set_size;
        while (isIn_p(indices, i, randomIndice)) {
            randomIndice = (randomIndice + 1)%set_size;
        }
        subset[i] = set[randomIndice];
    }
}

/**
 * Calcule "stupide" de la solution sur le sous-ensemble
*/
bool compute_p(unsigned long* subset, unsigned int subset_size, unsigned long target) {
    int computed_set_size = pow(2, subset_size);
    unsigned long* computed_set = (unsigned long*)calloc(computed_set_size, sizeof(unsigned long));
    computed_set[0] = target;
    int cs_index = 1;
    for (int i=0 ; i<subset_size ; i++){
        unsigned long w_i = subset[i];
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
bool keepGoing_p(unsigned long* set, int size, unsigned long target, int maxIter, int subset_size) {
    srand(time(NULL)); // Initialize the random seed
    int iter = 0;
    bool validate = false;
    
    // We run "maxIter" iterations (we stop befor if we find a solution)
    #pragma omp parallel for private(iter) shared(validate) num_threads(_NTHREADS)
    for (iter=0 ; iter<maxIter ; iter++) {
        if (validate) continue;
        unsigned long * subset = (unsigned long*)calloc(subset_size, sizeof(unsigned long));
        getSubset_p(set, subset, size, subset_size);
        validate = compute_p(subset, subset_size, target); // Execute the "stupid" algo on the subset
        free(subset);
    }
    if (validate) {
        // Display the solution's subset (may contain useless elements), and exit loop
        printf("Solution : ");
        printSet_p(set, subset_size);
    }
    printf("%d ? found ?\n", validate);
    return validate;
}

/**
 * Execute the probabilistic approch of the problem
*/
void execution_parallel(unsigned long * set, int set_size, unsigned long target) {
    printf("Running with : %i iterations / subset size of %i / set size of : %i\n",_NB_ITERATION,_SUBSET_SIZE,set_size); // Recap
    bool soluce = keepGoing_p(set, set_size, target, _NB_ITERATION, _SUBSET_SIZE); // execution
}

/**
 * Execute the probabilistic approch of the problem
*/
void execution_test_parallel(unsigned long * set, int set_size, unsigned long target, int nb_iteration, int subset_size) {
    printf("Running with : %i iterations / subset size of %i / set size of : %i\n",nb_iteration,subset_size,set_size); // Recap
    bool soluce = keepGoing_p(set, set_size, target, nb_iteration, subset_size); // execution
}