#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

unsigned int NTHREADS = 5;

unsigned long TARGET_p;
unsigned long * SET_p;
size_t SET_SIZE_p;
size_t SUBSET_SIZE_p;
int LOOP_p;

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
void PrintPreciseSolution_p(unsigned long* processed_subset, int indice_of_zero, unsigned long* subset_p) {
    printf("Precise solution : [ ");
    int nbOfConsideredBits = 0;
    // We count the number of useful bits
    while(pow(2, nbOfConsideredBits)<indice_of_zero) {
        nbOfConsideredBits += 1;
    }
    int i;
    // We go through all useful bits
    //#pragma omp parallel for
    for (i=0 ; i<nbOfConsideredBits ; i++){
        // Creation of the mask to only consider the i-th bit of "indice_of_zero"
        int mask =  1 << i;
        int mask_applyed = indice_of_zero & mask;
        int bit = mask_applyed >> i;
        // If the bit is 1 : The i-th number of the subset is part of the solution
        if (bit==1) {
            printf("%lu ", subset_p[i]);
        }
    }
    printf("]\n");
}

/**
 * Calcule "stupide" de la solution sur le sous-ensemble
*/
bool compute_p(unsigned long* subset_p) {
    int computed_set_size = pow(2, SUBSET_SIZE_p); // size of the set containing all the possibilities ( 2^n )
    unsigned long* computed_set = (unsigned long*)calloc(computed_set_size, sizeof(unsigned long));
    /* Execution of the algorithm presented within the article
    The computed set will contain :
        [target (t), t-w1, t-w2, t-w1-w2, t-w3, t-w1-w3, t-w1-w2-w3, .....] => Stupid solution for subset sum
    We keep filling it until we find a zero or the tab is full
    */
    computed_set[0] = TARGET_p;
    int cs_index = 1;
    for (int i=0 ; i<SUBSET_SIZE_p ; i++){
        unsigned long w_i = subset_p[i];
        for (int j=0 ; j<cs_index ; j++){
            computed_set[cs_index+j] = computed_set[j] - w_i;
            if (computed_set[cs_index+j]==0) {
                PrintPreciseSolution_p(computed_set, (cs_index+j), subset_p);
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
bool isIn_p(int * indices, unsigned int indices_size, int target) {
    int i;
    bool end = false;
    #pragma omp parallel for private(i) shared(end) num_threads(NTHREADS)
    for (i=0 ; i<indices_size ; i++){
        if (end) continue;
        if (target==indices[i]) {
            end = true;
        }
    }
    return end;
}

/**
 * Randomly pick "SUBSET_SIZE" indices between 0 and "SET_SIZE", and fill the subset with the corresponding values
*/
void getSubset_p(unsigned long* subset_p) {
    int * indices = (int*)calloc(SUBSET_SIZE_p, sizeof(int));
    int i;
    for (i=0 ; i<SUBSET_SIZE_p ; i++) {
        int randomIndice = rand() % SET_SIZE_p;
        while (isIn_p(indices, i, randomIndice)) {
            randomIndice = (randomIndice + 1)%SET_SIZE_p;
        }
        indices[i] = randomIndice;
        subset_p[i] = SET_p[randomIndice];
    }
    free(indices);
}

/**
 * Execution of the probabilistic approch
*/
bool keepGoing_p() {
    bool validate = false;
    // Impact of shuffling task
    // We run "maxIter" iterations (we stop befor if we find a solution)
    int iter;
    unsigned long subset_p[SUBSET_SIZE_p];
    #pragma omp parallel for private(iter, subset_p) shared(validate) num_threads(NTHREADS)
    for (iter=0 ; iter<LOOP_p ; iter++) {
        if (validate) continue;
        getSubset_p(subset_p);
        bool tmp = compute_p(subset_p); // Execute the "stupid" algo on the subset
        
        #pragma omp critical
        {
            validate = validate || tmp;
        }

    }
    printf("%d, after %i iterations\n", validate, iter);
    return validate;
}


/**
 * Execute the probabilistic approch of the problem
*/
void execution_test_p(unsigned long* set, unsigned long target, size_t set_size, size_t subset_size, int loop) {
    TARGET_p = target;
    SET_p = set;
    SET_SIZE_p = set_size;
    SUBSET_SIZE_p = subset_size;
    LOOP_p = loop;
    printf("Running with : %i iterations / subset size of %li / set size of : %li\n",LOOP_p, SUBSET_SIZE_p, SET_SIZE_p); // Recap
    bool soluce = keepGoing_p(); // execution
}