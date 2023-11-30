#!/bin/bash

gcc $@ -fopenmp -lm
./a.out
rm ./a.out
