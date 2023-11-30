#ifndef PROBA_PARALLEL_H_
#define PROBA_PARALLEL_H_

void execution_parallel(unsigned long * set, int set_size, unsigned long target);

void execution_test_parallel(unsigned long * set, int set_size, unsigned long target, int nb_iteration, int subset_size);

#endif