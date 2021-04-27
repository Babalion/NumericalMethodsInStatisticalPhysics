//
// Created by chris on 26.04.21.
//

#include "RngTests.h"

int runRngTests() {
    std::cout << "This is RngTests." << std::endl;
    compareRNGs();
    return 0;
}

double piTest(std::list<double> randomNumbers1, std::list<double> randomNumbers2) {
    //check if lists have same dimension
    if (randomNumbers1.size() != randomNumbers2.size()) {
        std::cerr << "given Lists aren't same size!";
        return -1;
    }

    int insideCircle = 0;

    auto it1 = randomNumbers1.begin();
    auto it2 = randomNumbers2.begin();
    // TODO simplify this expression like mentioned here: https://youtu.be/SgcHcbQ0RCQ?t=526
    for (; it1 != randomNumbers1.end() && it2 != randomNumbers2.end(); ++it1, ++it2) {
        if (sqrt((*it1) * (*it1) + (*it2) * (*it2)) <= 1)
            insideCircle++;
    }

    return abs(pi - 4.0 * insideCircle / randomNumbers1.size());
}

// a generator from <random>, here Linear congruential generator
// not the best generator
double randomNumber_LCG() {
    // Making rng static ensures that it stays the same
    // Between different invocations of the function
    static std::default_random_engine rng;

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

// performs a pi-Test with Linear-Congruential-Method with a list of n 2-Tuples
// this should not be the best due to small periodicity of LCG
double piTest_LCG(unsigned int n) {
    std::list<double> randLibList1;
    std::list<double> randLibList2;

    for (int i = 0; i < n; i++) {
        randLibList1.push_back(randomNumber_LCG());
        randLibList2.push_back(randomNumber_LCG());
    }
    return piTest(randLibList1, randLibList2);
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
    //----------------------------------------------------------------------------------------------------
    //test c++ random library with Linear congruential generator
    std::list<double> MT19937_List1;
    std::list<double> MT19937_List2;

    for (int i = 0; i < n; i++) {
        MT19937_List1.push_back(random_MT19937());
        MT19937_List2.push_back(random_MT19937());
    }
    return piTest(MT19937_List1, MT19937_List2);
}

void compareRNGs() {
    std::list<int> n;
    std::list<double> diff_LCG;
    std::list<double> diff_MT19937;
    std::list<double> time_ms_LCG;
    std::list<double> time_ms_MT19937;

    for (int i = 10; i < 1E8; i *= 5) {
        std::cout << "Performing pi-Test with log10(n)=" << log10(i) << "...." << std::endl;
        n.push_back(i);

        std::chrono::steady_clock::time_point begin_LCG = std::chrono::steady_clock::now();
        diff_LCG.push_back(piTest_LCG(i));
        std::chrono::steady_clock::time_point end_LCG = std::chrono::steady_clock::now();
        time_ms_LCG.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end_LCG - begin_LCG).count());


        std::chrono::steady_clock::time_point begin_MT19937 = std::chrono::steady_clock::now();
        diff_MT19937.push_back(piTest_MT19937(i));
        std::chrono::steady_clock::time_point end_MT19937 = std::chrono::steady_clock::now();
        time_ms_MT19937.push_back(
                std::chrono::duration_cast<std::chrono::milliseconds>(end_MT19937 - begin_MT19937).count());

    }

    auto it1 = n.begin();
    auto it2 = diff_LCG.begin();
    auto it3 = time_ms_LCG.begin();
    auto it4 = diff_MT19937.begin();
    auto it5 = time_ms_MT19937.begin();
    std::cout << "n\tLCG-diff\tLCG-time\tMT19937-diff\tMT19937-time" << std::endl;
    for (; it1 != n.end() && it2 != diff_LCG.end() && it3 != time_ms_LCG.end()
           && it4 != diff_MT19937.end() && it5 != time_ms_MT19937.end();
           ++it1, ++it2, ++it3, ++it4, ++it5) {
        std::cout << *it1 << "\t" << *it2 << "\t" << *it3 <<"\t"<< *it4 <<"\t"<< *it5 << std::endl;
    }

}
