//
// Created by chris on 26.04.21.
//

#include "RngTests.h"

int main(){
    runRngTests();
}

//startPoint for RNG-Tests Project. Returns 0 if no error occurs.
int runRngTests() {
    std::cout << "This is RngTests. Endless (0) or interactive Mode (1)?" << std::endl;
    int mode;
    std::cin >> mode;
    switch (mode) {
        case 0:
            endlessMode();
            break;
        case 1:
            interactiveMode();
            break;
        default:
            runRngTests();
            break;
    }

    return 0;
}

int interactiveMode() {
    unsigned long long maxN = 1E6;
    unsigned int amount = 8;
    std::cout << "Until wich Number 10^X would you like to run? 1E4-1E10 are good values." << std::endl;
    std::cin >> maxN;
    maxN = std::pow(10, maxN + 1);
    std::cout << "How many values per Number should be generated? (1-100)" << std::endl;
    std::cin >> amount;

    RNG_results finalResults;
    compareRNGs_parallel(finalResults, maxN, 10, amount);
    finalResults.saveResults();
    return 0;
}

bool stopCommandEndless = false;

//tries to get best value of pi.
//terminates if you press enter
int endlessMode() {
    RNG_results rng;
    static const unsigned int hardwareCon = std::thread::hardware_concurrency();
    static const unsigned int supportedThreads = hardwareCon == 0 ? 2 : hardwareCon;
    std::thread workerThread(
            [&]() {
                while (!stopCommandEndless)
                    compareRNGs_parallel(rng, 1E7, 1E6, 10, 50 * supportedThreads);
                rng.saveAndClearResults();
            } // end of lambda expression
    );
    workerThread.detach();
    std::cout << "Press 1 to stop program." << std::endl;
    char a = 0;
    while (a != 1) {
        std::cin >> a;
    }
    stopCommandEndless = true;
    rng.saveAndClearResults();
    return 0;
}

// a generator from <random>, here Linear congruential generator
// not the best generator
double RNGs::randomNumber_LCG() {
    // Making rng static ensures that it stays the same
    // Between different invocations of the function
    static std::linear_congruential_engine<uint_fast32_t, 16807UL, 0UL, 2147483647UL> rng;

    //return dist(rd); //this is a physical hardware RNG
    //return rand() * 1.0 / RAND_MAX; //this is one of the worst RNGs
    return dist(rng);
}

// performs a pi-Test with Linear-Congruential-Method with a list of n 2-Tuples
// this should not be the best due to small periodicity of LCG
double RNGs::piTest_LCG(unsigned long long n) {
    unsigned long long insideCircle = 0;
    for (unsigned long long i = 0; i < n; i++) {
        if (sqrt(pow(randomNumber_LCG(), 2) + pow(randomNumber_LCG(), 2)) <= 1)
            insideCircle++;
    }
    return pi - 4.0 * static_cast<long double>(insideCircle) / static_cast<long double>(n);
}


// Mersenne-Twister 19937 algorithm from <random>
// Best algorithm for production in C++
// use random device input for seed and total randomness
double RNGs::random_MT19937() {
    return dist(mt);
}

//performs a pi-Test with MT19937 with a list of n 2-Tuples
double RNGs::piTest_MT19937(unsigned long long n) {
    unsigned long long insideCircle = 0;
    for (unsigned long long i = 0; i < n; i++) {
        if (sqrt(pow(random_MT19937(), 2) + pow(random_MT19937(), 2)) <= 1)
            insideCircle++;
    }
    return pi - 4.0 * static_cast<long double>(insideCircle) / static_cast<long double>(n);
}

RNGs::RNGs() {
    mt = std::mt19937_64(rd());
    //initialize MT19937. Therefore let it generate 8E5 random numbers
    // to be sure the initial value doesnt affect randomness badly anymore.
    std::cout << "This is the first run of MT19937, init now!" << std::endl;
    for (uint_fast32_t i = 0; i < 8E5; i++) {
        dist(mt);
    }
}

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs_seq.tsv
// input: rng: a RNGResults class handling save results
//        max_N: max Number to test (min 100)
//        min_N: min Number to test (min 10)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_seq(RNG_results &rng, RNGs &rGen, unsigned long long max_N,
                     unsigned long long min_N, unsigned int step, unsigned int amount) {

    for (int i = min_N; i < max_N; i *= step) {
        for (int j = 0; j < amount; j++) {
            std::cout << "Performing pi-Test with log10(n)=" << log10(i) << "....(" << j + 1 << "/" << amount << ")"
                      << std::endl;


            std::chrono::steady_clock::time_point begin_LCG = std::chrono::steady_clock::now();
            double diff_LCG = rGen.piTest_LCG(i);
            std::chrono::steady_clock::time_point end_LCG = std::chrono::steady_clock::now();
            double time_ms_LCG = std::chrono::duration_cast<std::chrono::microseconds>(end_LCG - begin_LCG).count();


            std::chrono::steady_clock::time_point begin_MT19937 = std::chrono::steady_clock::now();
            double diff_MT19937 = rGen.piTest_MT19937(i);
            std::chrono::steady_clock::time_point end_MT19937 = std::chrono::steady_clock::now();
            double time_ms_MT19937 = std::chrono::duration_cast<std::chrono::microseconds>(
                    end_MT19937 - begin_MT19937).count();
            rng.push_results(i, diff_LCG, diff_MT19937, time_ms_LCG, time_ms_MT19937);
        }
    }
}

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs_seq.tsv
// input: rng: a RNGResults class handling save results
//        max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_seq_(RNG_results &rng, RNGs &rGen, unsigned long long max_N,
                      unsigned int step, unsigned int amount) {
    compareRNGs_seq(rng, rGen, max_N, 10, step, amount);
}

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs.tsv
// Calculation is done parallel
// input: max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_parallel(RNG_results &rng, unsigned long long max_N, unsigned long long min_N, unsigned int step,
                          unsigned int amount) {
    // Calculate amount of threads
    static const unsigned int hardwareCon = std::thread::hardware_concurrency();
    static const unsigned int supportedThreads = hardwareCon == 0 ? 2 : hardwareCon;
    static const unsigned int minWorkPerThread = 1; //at least an amount of piTest, from 10 to max_N should be done per Thread.

    static const unsigned int maxAmountOfThreads = (amount + minWorkPerThread - 1) / minWorkPerThread;
    // this is the optimal number of threads....
    static const unsigned int amountOfThreads =
            supportedThreads > maxAmountOfThreads ? maxAmountOfThreads : supportedThreads;
    std::cout << "Cool, you're using Multithreading! Number of used cores is: " << amountOfThreads << std::endl;
    static const unsigned int WorkPerThread = amount / amountOfThreads;
    //TODO optimize Workload, that last thread has less work, not most work

    std::vector<std::thread> threads(amountOfThreads - 1);
    std::vector<RNGs> rGen(amountOfThreads);

    for (int i = 0; i < threads.size(); i++) {
        threads[i] = std::thread(compareRNGs_seq, std::ref(rng), std::ref(rGen[i]), max_N, min_N, step,
                                 static_cast<int>(WorkPerThread));
    }

    compareRNGs_seq(rng, rGen[threads.size()], max_N, min_N, step, amount - WorkPerThread * (amountOfThreads-1));


    for (auto &i : threads) {
        i.join();
    }
    rng.saveResults();
}

// compares the LCG and MT19937 random generators with a pi-test and prints the result to compareRNGs.tsv
// Calculation is done parallel
// input: max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void compareRNGs_parallel(RNG_results &rng, unsigned long long max_N, unsigned int step, unsigned int amount) {
    compareRNGs_parallel(rng, max_N, 10, step, amount);
}
