//
// Created by chris on 26.04.21.
//
#pragma once
#include "RNG_results.h"

// Starts the Project RngTests. Returns 0 if no error occurs.
int runRngTests();

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs.tsv
// input: rng: a RNGResults class handling save results
//        max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs(RNG_results &rng, int max_N, int step, int amount);

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs.tsv
// Calculation is done parallel
// input: max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
//void compareRNGs_parallel(RNG_results &rng,unsigned int max_N, unsigned int step, unsigned int amount);
