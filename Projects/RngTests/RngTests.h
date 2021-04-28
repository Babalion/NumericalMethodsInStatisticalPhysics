//
// Created by chris on 26.04.21.
//
#pragma once

#include "RNG_results.h"

//startPoint for RNG-Tests Project. Returns 0 if no error occurs.
int runRngTests();
int interactiveMode();
int endlessMode();

class RNGs {
public:
    RNGs();

    // performs a pi-Test with Linear-Congruential-Method with a list of n 2-Tuples
    // this should not be the best due to small periodicity of LCG
    double piTest_LCG(unsigned long long n);

    //performs a pi-Test with MT19937 with a list of n 2-Tuples
    double piTest_MT19937(unsigned long long n);

private:
    // Mersenne-Twister 19937 algorithm from <random>
    // Best algorithm for production in C++
    // use random device input for seed and total randomness
    double random_MT19937();

    // a generator from <random>, here Linear congruential generator
    // not the best generator
    double randomNumber_LCG();

    std::uniform_real_distribution<double> dist;
    std::random_device rd;
    std::mt19937_64 mt;
};

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs_seq.tsv
// input: rng: a RNGResults class handling save results
//        max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_seq_(RNG_results &rng, RNGs &rGen, unsigned long long max_N, unsigned int step, unsigned int amount);

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs_seq.tsv
// input: rng: a RNGResults class handling save results
//        max_N: max Number to test (min 100)
//        min_N: min Number to test (min 10)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_seq(RNG_results &rng, RNGs &rGen, unsigned long long max_N,
                     unsigned long long min_N, unsigned int step, unsigned int amount);

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs_seq.tsv
// Calculation is done parallel
// input: max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_parallel(RNG_results &rng, unsigned long long max_N, unsigned int step, unsigned int amount);

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs.tsv
// Calculation is done parallel
// input: max_N: max Number to test (min 100)
//        min_N: min Number to test (min 10)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_parallel(RNG_results &rng, unsigned long long max_N,unsigned long long min_N, unsigned int step, unsigned int amount);