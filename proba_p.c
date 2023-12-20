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
unsigned int* SEEDS;

/*
[t, t-w1, t-w2, t-w1-w2, t-w3, t-w1-w3, t-w1-w2-w3, t-w4, t-w1-w4, t-w2-w4, t-w1-w2-w4, t-w3-w4, t-w1-w3-w4, t-w1-w2-w3-w4]
[0,    1,    2,       3,    4        5,          6,    7        8,       9,         10,      11,         12,            13]

convert index to binary (2 become 10)
The i-th bit indicate if w_i is part of the solution (1 = yes / 0 = no)

index => Solution
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
    printf("Precise solution by Thread %d: [ ", omp_get_thread_num());
    int nbOfConsideredBits = 0;
    // We count the number of useful bits
    while(pow(2, nbOfConsideredBits)<indice_of_zero) {
        nbOfConsideredBits += 1;
    }
    int i;
    // We go through all useful bits
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
 * The computed set will contain :
 *    [target (t), t-w1, t-w2, t-w1-w2, t-w3, t-w1-w3, t-w1-w2-w3, .....] => Stupid solution for subset sum
 * We keep filling it until we find a zero or the tab is full
*/
bool compute_p(unsigned long* subset_p) {
    // Initialize the tab containing all the computations (size of 2^Subset_size)
    int computed_set_size = pow(2, SUBSET_SIZE_p);
    unsigned long* computed_set = (unsigned long*)calloc(computed_set_size, sizeof(unsigned long));

    computed_set[0] = TARGET_p;
    int cs_index = 1;
    for (int i=0 ; i<SUBSET_SIZE_p ; i++){
        unsigned long w_i = subset_p[i];
        // We go back in the tab to consider every previous computations
        for (int j=0 ; j<cs_index ; j++){
            computed_set[cs_index+j] = computed_set[j] - w_i;
            if (computed_set[cs_index+j]==0) {
                // We found a solution
                #pragma omp critical
                {
                    PrintPreciseSolution_p(computed_set, (cs_index+j), subset_p);
                }
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
    int indices[SUBSET_SIZE_p];
    int i;
    int randomIndice;
    for (i=0 ; i<SUBSET_SIZE_p ; i++) {
        randomIndice = rand() % SET_SIZE_p;
        while (isIn_p(indices, i, randomIndice)) {
            randomIndice = (randomIndice + 1)%SET_SIZE_p;
        }
        indices[i] = randomIndice;
        subset_p[i] = SET_p[randomIndice];
    }
}

/**
 * Execution of the probabilistic approch
*/
bool keepGoing_p() {
    bool validate = false;
    bool tmp = false;
    // We run "maxIter" iterations (we stop befor if we find a solution)
    int iter;
    int fullIter = 0; // Count the effective number of iteration of every threads
    for (iter=0 ; iter<(LOOP_p/NTHREADS) ; iter++) {
        if (validate) break;
        #pragma omp parallel num_threads(NTHREADS) shared(validate, fullIter) private(tmp)
        {
            if (!validate) {
                unsigned long subset_p[SUBSET_SIZE_p];
                getSubset_p(subset_p);
                tmp = compute_p(subset_p); // Execute the "stupid" algo on the subset
                
                // Critical region for changing the value of validate and counting the total number of iteration
                #pragma omp critical
                {
                    validate = validate || tmp;
                    fullIter++;
                }
            }
        }
    }
    printf("%d, after %i iterations\n", validate, fullIter);
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
    printf("Running with : %i iterations / subset size of %li / set size of : %li / Nb of threads : %i\n",LOOP_p, SUBSET_SIZE_p, SET_SIZE_p, NTHREADS); // Recap
    bool soluce = keepGoing_p(); // execution
}

/**
 * Execute the probabilistic approch of the problem with default values for SUBSET_SIZE_p and LOOP_p
*/
void executiont_p(unsigned long* set, unsigned long target, size_t set_size) {
    TARGET_p = target;
    SET_p = set;
    SET_SIZE_p = set_size;
    SUBSET_SIZE_p = 10;
    LOOP_p = 10000000;
    printf("Running with : %i iterations / subset size of %li / set size of : %li / Nb of threads : %i\n",LOOP_p, SUBSET_SIZE_p, SET_SIZE_p, NTHREADS); // Recap
    bool soluce = keepGoing_p(); // execution
}

/**
 * Set the number of threads used by the programm (set at 5 by default)
*/
void set_number_of_threads(int nbThreads) {
    NTHREADS = nbThreads;
}