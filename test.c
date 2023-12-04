
#include <stdio.h>
#include <time.h>

int print() {
    return 1;
}

/**
 * Printh the k-th bit of n (start at 0)
*/
void main(){
    int add = 0;
    int add1, add2, add3, add4, add5;
    #pragma omp parallel sections shared(add, add1, add2, add3, add4, add5) num_threads(5)
    {
        #pragma omp section
            add1 = print();
        #pragma omp section
            add2 = print();
        #pragma omp section
            add3 = print();
        #pragma omp section
            add4 = print();
        #pragma omp section
            add5 = print();
    }
    add = add1+add2+add3+add4+add5;
    printf("%i\n", add);
}