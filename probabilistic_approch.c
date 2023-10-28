
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int SHUFFLING_PROBABILITY = 2;  // in fact the probability is (SHUFFLING_PROBABILITY)^(-1)

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
 * Shuffle the set passed as a parameter
*/
void shuffle(unsigned long* set, int size) {
    int i;
    for (i=0 ; i<size ; i++) {
        int target1 = rand()%size;
        int target2 = rand()%size;
        unsigned long tmp = set[target1];
        set[target1] = set[target2];
        set[target2] = tmp;
    }
}

bool compute(unsigned long* set, int borne, unsigned long target) {
    int computed_set_size = 64;//(int)pow(2, borne);
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
    while (~validate && iter<maxIter) {
        srand(time(NULL));
        shuffle(set, size);
        validate = compute(set, subset_size, target);
        iter++;
    }
    printf("%d, after %i iterations\n", validate, iter);
    return validate;
}


void main() {
    unsigned long target = 60;
    int n = 100;
    int subset_size = 6;
    unsigned long set[100] = {42,18,73,56,91,23,67,11,5,29,82,49,36,64,55,38,12,50,78,2,71,19,85,95,62,14,27,89,44,7,33,61,76,28,51,60,3,68,45,17,87,6,34,70,22,59,94,26,54,46,15,69,37,80,9,48,75,21,57,32,8,83,20,65,72,41,10,79,30,58,77,40,66,25,43,86,16,74,35,63,53,4,47,88,1,31,81,24,52,93};
    printf("%d\n", keepGoing(set, n, target, 20, subset_size));
}


