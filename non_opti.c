#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned long * _SUBSET;
unsigned long _TARGET;
unsigned long * _SET;
size_t _SET_SIZE = 100;
size_t _SUBSET_SIZE = 10;
int _LOOP = 10000000;

/**
 * Print a tab
*/
void _printSet(unsigned long * tab, int n) {
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
bool _compute() {
    int computed_set_size = pow(2, _SUBSET_SIZE); // size of the set containing all the possibilities ( 2^n )
    unsigned long* computed_set = (unsigned long*)calloc(computed_set_size, sizeof(unsigned long));
    /* Execution of the algorithm presented within the article
    The computed set will contain :
        [target (t), t-w1, t-w2, t-w1-w2, t-w3, t-w1-w3, t-w1-w2-w3, .....] => Stupid solution for subset sum
    We keep filling it until we find a zero or the tab is full
    */
    computed_set[0] = _TARGET;
    int cs_index = 1;
    for (int i=0 ; i<_SUBSET_SIZE ; i++){
        unsigned long w_i = _SET[i];
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
 * Execution of the probabilistic approch
*/
bool _keepGoing() {
    //srand(time(0));
    int iter = 0;
    bool validate = false;
    // Impact of shuffling task
    clock_t start_shuffle, end_shuffle;
    double cpu_time_used_shuffle = ((double) (0.0)) / CLOCKS_PER_SEC;
    clock_t start_compile, end_compile;
    double cpu_time_used_compile = ((double) (0.0)) / CLOCKS_PER_SEC;
    // We run "maxIter" iterations (we stop befor if we find a solution)
    while (!validate && iter<_LOOP) {

        start_shuffle = clock();
        shuffle(_SET, _SET_SIZE);
        end_shuffle = clock();
        cpu_time_used_shuffle += ((double) (end_shuffle - start_shuffle)) / CLOCKS_PER_SEC;

        start_compile = clock();
        validate = _compute(); // Execute the "stupid" algo on the subset
        end_compile = clock();
        cpu_time_used_compile += ((double) (end_compile - start_compile)) / CLOCKS_PER_SEC;

        iter++;
    }
    if (validate) {
        // Display the solution's subset (may contain useless elements)
        printf("Solution : ");
        _printSet(_SET, _SUBSET_SIZE);
    }
    printf("Shuffling time : %f\n", cpu_time_used_shuffle);
    printf("Stupid time : %f\n", cpu_time_used_compile);
    printf("%d, after %i iterations\n", validate, iter);
    return validate;
}


/**
 * Execute the probabilistic approch of the problem
*/
void execution_test_nonOpti(unsigned long* set, unsigned long target, size_t set_size, size_t subset_size, int loop) {
    _TARGET = target;
    _SET = set;
    _SET_SIZE = set_size;
    _SUBSET_SIZE = subset_size;
    _LOOP = loop;
    printf("Running with : %i iterations / subset size of %li / set size of : %li\n",_LOOP, _SUBSET_SIZE, _SET_SIZE); // Recap
    bool soluce = _keepGoing(); // execution
}
