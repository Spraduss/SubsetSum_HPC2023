
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int SHUFFLING_PROBABILITY = 2;  // in fact the probability is (SHUFFLING_PROBABILITY)^(-1)

/**
 * 
 * Casually print a tab
 * 
*/
void printSet(unsigned long * set, int size) {
    printf("[");
    for (int i=0 ; i<size ; i++) {
        if (i==size-1) {
            printf("%lu", set[i]);
        } else {
            printf("%lu", set[i]);
            printf(", ");
        }
    }
    printf("]\n");
}

/**
 * Shuffle the set passed as a parameter that do not work for some reasons...
*/
void shuffle_louis(unsigned long* set, int size) {
    int i;
    for (i=0 ; i<size ; i++) {
        int target1 = rand()%size;
        int target2 = rand()%size;
        unsigned long tmp = set[target1];
        set[target1] = set[target2];
        set[target2] = tmp;
    }
}

/**
 * Shuffling function from StackOverflow that works
*/
void shuffle_StackOverflow(unsigned long *array, int n) {
    if (n > 1) {
        int i;
        for (i = 0; i < n - 1; i++) {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

/**
 * 
*/
bool compute(unsigned long* set, int borne, unsigned long target) {
    int computed_set_size = pow(2, borne);
    unsigned long* computed_set = (unsigned long*)calloc(computed_set_size, sizeof(unsigned long));
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

bool keepGoing(unsigned long* set, int size, unsigned long target, int maxIter, int subset_size) {
    int iter = 0;
    bool validate = false;
    while (!validate && iter<maxIter) {
        srand(time(NULL));
        //shuffle_louis(set, size);
        shuffle_StackOverflow(set, size);
        validate = compute(set, subset_size, target);
        iter++;
    }
    if (validate) {
        printf("Solution : ");
        printSet(set, subset_size);
    }
    printf("%d, after %i iterations\n", validate, iter);
    return validate;
}

void normal_execution(unsigned long * set, int set_size, unsigned long target) {
    int nb_iteration = 100000000;
    int subset_size = 9;
    printf("Running with : %i iterations / subset size of %i / size of set : %i\n",nb_iteration,subset_size,set_size);
    bool soluce = keepGoing(set, set_size, target, nb_iteration, subset_size);
}

void personalised_execution(unsigned long * set, int set_size, unsigned long target, int nb_loop, int nb_iter, int subset_size) {
    int i;
    int success = 0;
    for (i=0 ; i<nb_loop ; i++){
        if(keepGoing(set, set_size, target, nb_iter, subset_size)){
            success++;
        }
    }
    float percent = success * 100 / nb_loop;
    printf("%i success on %i iterations : rate of %f percents\n",success, nb_loop, percent);
}
