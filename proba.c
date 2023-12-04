#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

unsigned long * SUBSET;
unsigned long TARGET;
unsigned long * SET;
size_t SET_SIZE;
size_t SUBSET_SIZE;
int LOOP;

/*
[t, t-w1, t-w2, t-w1-w2, t-w3, t-w1-w3, t-w1-w2-w3, t-w4, t-w1-w4, t-w2-w4, t-w1-w2-w4, t-w3-w4, t-w1-w3-w4, t-w1-w2-w3-w4]
[0,    1,    2,       3,    4        5,          6,    7        8,       9,         10,      11,         12,            13]

indice vers le binaire
Le i-ème bit indique si wi est inclu dans la solution (1 = oui / 0 = non)

0000 /
0001 => w1 / (1)
0010 => w2 / (2)
0011 => W1 & w2 / (1+2)
0100 => w3 / (3)
0101 => w1 & w3 / (1+3)
0110 => w2 & w3 / (2+3)
0111 => w1 & w2 & w3 / (1+2+3)
1000 => w4 / (4)
1001 => w1 & w4 / (1+4)
1010 => w2 & w4 / (2+4)
1011 => w1 & w2 & w4 / (1+2+4)
1100 => w3 & w4 / (3+4)
1101 => w1 & w3 & w4 / (1+3+4)
1111 => w1 & w2 & w3 & w4 / (1+2+3+4)
*/
void PrintPreciseSolution(unsigned long* processed_subset, int indice_of_zero) {
    printf("Precise solution : [ ");
    int nbOfConsideredBits = 0;
    // We count the number of useful bits
    while(pow(2, nbOfConsideredBits)<indice_of_zero) {
        nbOfConsideredBits += 1;
    }
    // We go through all useful bits
    for (int i=0 ; i<nbOfConsideredBits ; i++){
        // Creation of the mask to only consider the i-th bit of "indice_of_zero"
        int mask =  1 << i;
        int mask_applyed = indice_of_zero & mask;
        int bit = mask_applyed >> i;
        // If the bit is 1 : The i-th number of the subset is part of the solution
        if (bit==1) {
            printf("%lu ", SUBSET[i]);
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
        unsigned long w_i = SUBSET[i];
        for (int j=0 ; j<cs_index ; j++){
            computed_set[cs_index+j] = computed_set[j] - w_i;
            if (computed_set[cs_index+j]==0) {
                PrintPreciseSolution(computed_set, (cs_index+j));
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
    free(indices);
}

/**
 * Execution of the probabilistic approch
*/
bool keepGoing() {
    int iter = 0;
    bool validate = false;
    // We run "maxIter" iterations (we stop befor if we find a solution)
    while (!validate && iter<LOOP) {
        SUBSET = (unsigned long*)calloc(SUBSET_SIZE, sizeof(unsigned long));
        getSubset();
        validate = compute(); // Execute the "stupid" algo on the subset
        iter++;
        free(SUBSET);
    }
    printf("%d, after %i iterations\n", validate, iter);
    return validate;
}


/**
 * Execute the probabilistic approch of the problem
*/
void execution_test_seq(unsigned long* set, unsigned long target, size_t set_size, size_t subset_size, int loop) {
    TARGET = target;
    SET = set;
    SET_SIZE = set_size;
    SUBSET_SIZE = subset_size;
    LOOP = loop;
    printf("Running with : %i iterations / subset size of %li / set size of : %li\n",LOOP, SUBSET_SIZE, SET_SIZE); // Recap
    bool soluce = keepGoing(); // execution
}

/**
 * Execute the probabilistic approch of the problem
*/
void execution_seq(unsigned long* set, unsigned long target, size_t set_size) {
    TARGET = target;
    SET = set;
    SET_SIZE = set_size;
    SUBSET_SIZE = 20;
    LOOP = 10000000;
    printf("Running with : %i iterations / subset size of %li / set size of : %li\n", LOOP, SUBSET_SIZE, SET_SIZE); // Recap
    bool soluce = keepGoing(); // execution
}
