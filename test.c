
#include <stdio.h>
#include <time.h>


/**
 * Printh the k-th bit of n (start at 0)
*/
void main(){
    int i;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    #pragma omp parallel for shared(i) num_threads(5)
    for(i=0; i<5; i++){
        sleep(1);
        printf("%i\n", i);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time : %f\n", cpu_time_used);
}