#include <stddef.h>

#ifndef PROBA_H_
#define PROBA_H_

void execution_test_seq(unsigned long* set, unsigned long target, size_t set_size, size_t subset_size, int loop);
void execution_seq(unsigned long* set, unsigned long target, size_t set_size);

#endif