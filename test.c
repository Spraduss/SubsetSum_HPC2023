
#include <stdio.h>
#include <math.h>

void getPreciseSolution(unsigned long* processed_subset, int indice_of_zero, unsigned long * SUBSET) {
    printf("Precise solution : [ ");
    int nbOfConsideredBits = 0;
    while(pow(2, nbOfConsideredBits)<indice_of_zero) {
        nbOfConsideredBits += 1;
    }
    for (int i=0 ; i<nbOfConsideredBits ; i++){
        int mask =  1 << i;
        int mask_applyed = indice_of_zero & mask;
        int bit = mask_applyed >> i;
        if (bit==1) {
            printf("%lu ", SUBSET[i]);
        }
    }
    printf("]\n");
}

/**
 * Printh the k-th bit of n (start at 0)
*/
void main(){
    unsigned long SUBSET[5] = {1 ,2, 3, 4, 5};
    unsigned long computed[16] = {8, 7, 6, 5, 5, 4, 3, 2, 4, 3, 2, 1, 1, 0, 30000, 40000};
    int indice_zero = 13;
    getPreciseSolution(computed, indice_zero, SUBSET);
}