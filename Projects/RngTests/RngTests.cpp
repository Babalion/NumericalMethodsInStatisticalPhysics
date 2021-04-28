//
// Created by chris on 26.04.21.
//

#include "RngTests.h"

int runRngTests() {
    std::cout << "This is RngTests." << std::endl;

    RNG_results finalResults;
    compareRNGs_parallel(finalResults, 1E8, 10, 30);
    finalResults.printResults();
    return 0;
}

// a generator from <random>, here Linear congruential generator
// not the best generator
double randomNumber_LCG() {
    // Making rng static ensures that it stays the same
    // Between different invocations of the function
    static std::linear_congruential_engine<uint_fast32_t, 16807UL, 0UL, 2147483647UL> rng;

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    //return rand()*1.0/RAND_MAX;
    return dist(rng);
}

// performs a pi-Test with Linear-Congruential-Method with a list of n 2-Tuples
// this should not be the best due to small periodicity of LCG
double piTest_LCG(unsigned int n) {
    int insideCircle = 0;
    for (int i = 0; i < n; i++) {
        if (sqrt(pow(randomNumber_LCG(), 2) + pow(randomNumber_LCG(), 2)) <= 1)
            insideCircle++;
    }
    return pi - 4.0 * insideCircle / n;
}


// Mersenne-Twister 19937 algorithm from <random>
// Best algorithm for production in C++
// use random device input for seed and total randomness
double random_MT19937() {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    //initialize MT19937. Therefore let it generate 8E5 random numbers
    // to be sure the initial value doesnt affect randomness badly anymore.
    static bool firstRun = true;
    if (firstRun) {
        firstRun = false;
        std::cout << "This is the first run of MT19937, init now!" << std::endl;
        for (int i = 0; i < 8E5; i++) {
            dist(mt);
        }
    }
    return dist(mt);
}

//performs a pi-Test with MT19937 with a list of n 2-Tuples
double piTest_MT19937(unsigned int n) {
    int insideCircle = 0;
    for (int i = 0; i < n; i++) {
        if (sqrt(pow(random_MT19937(), 2) + pow(random_MT19937(), 2)) <= 1)
            insideCircle++;
    }
    return pi - 4.0 * insideCircle / n;
}


// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs.tsv
// input: rng: a RNGResults class handling save results
//        max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs(RNG_results &rng, unsigned int max_N, unsigned int step, unsigned int amount) {

    for (int i = 10; i < max_N; i *= step) {
        for (int j = 0; j < amount; j++) {
            std::cout << "Performing pi-Test with log10(n)=" << log10(i) << "....(" << j + 1 << "/" << amount << ")"
                      << std::endl;


            std::chrono::steady_clock::time_point begin_LCG = std::chrono::steady_clock::now();
            double diff_LCG = piTest_LCG(i);
            std::chrono::steady_clock::time_point end_LCG = std::chrono::steady_clock::now();
            double time_ms_LCG = std::chrono::duration_cast<std::chrono::microseconds>(end_LCG - begin_LCG).count();


            std::chrono::steady_clock::time_point begin_MT19937 = std::chrono::steady_clock::now();
            double diff_MT19937 = piTest_MT19937(i);
            std::chrono::steady_clock::time_point end_MT19937 = std::chrono::steady_clock::now();
            double time_ms_MT19937 = std::chrono::duration_cast<std::chrono::microseconds>(
                    end_MT19937 - begin_MT19937).count();
            rng.push_results(i, diff_LCG, diff_MT19937, time_ms_LCG, time_ms_MT19937);
        }
    }

    rng.printResults();

}

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs.tsv
// Calculation is done parallel
// input: max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_parallel(RNG_results &rng, unsigned int max_N, unsigned int step, unsigned int amount) {
    // Calculate amount of threads
    static const unsigned int hardwareCon = std::thread::hardware_concurrency();
    static const unsigned int supportedThreads = hardwareCon == 0 ? 2 : hardwareCon;
    static const unsigned int minWorkPerThread = 1; //at least an amount of piTest, from 10 to max_N should be done per Thread.

    static const unsigned int maxAmountOfThreads = (amount + minWorkPerThread - 1) / minWorkPerThread;
    // this is the optimal number of threads....
    static const unsigned int amountOfThreads = supportedThreads > maxAmountOfThreads ? maxAmountOfThreads : supportedThreads;
    std::cout<<"Cool, you're using Multithreading! Number of used cores is: "<<amountOfThreads<<std::endl;
    static const unsigned int WorkPerThread = amount / amountOfThreads;


    std::vector<std::thread> threads(amountOfThreads-1);

    for (auto & i : threads) {
        i = std::thread(compareRNGs, std::ref(rng), max_N, step, static_cast<int>(WorkPerThread));
    }

    compareRNGs(rng,max_N,step,amount-WorkPerThread*amountOfThreads);


    for (auto & i : threads) {
        i.join();
    }


}