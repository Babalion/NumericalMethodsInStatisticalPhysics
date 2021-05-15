//
// Created by chris on 15.05.21.
//
#include <iostream>
#include <fstream>
#include <thread>
#include "Euclidean2DGraph.h"
#include "Euclidean3DGraph.h"

//TODO input this in WalkableGraph
/*
// Calculation is done parallel
// input: max_N: max Number to test (min 100)
//        step: multiplicative step width (min 2)
//        amount: amount of tests per number (min 1)
void runParallel(unsigned int amount){
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
*/

void runEuclidean2D() {

    const int maxIterations = 1E3;
    const int highestMaxSteps = 1E7;
    const int logInterval = 100;

    Euclidean2DGraph eu2D("euclidean2D.tsv");
    eu2D.generateDimensionData(maxIterations, highestMaxSteps, logInterval);
}

void runEuclidean3D() {

    const int maxIterations = 5E3;
    const int highestMaxSteps = 1E7;
    const int logInterval = 100;

    Euclidean3DGraph eu3D("euclidean3D.tsv");
    eu3D.generateDimensionData(maxIterations, highestMaxSteps, logInterval);
}

int main() {
    std::cout << "Welcome to fractals! You can calc the local spectral dimension of fractals with this tool."
              << std::endl;
    std::cout << "Which fractal do you want to calculate? Choose between:" << std::endl;
    std::cout << "exit (0)" << std::endl;
    std::cout << "2D-Euclidean lattice (1)" << std::endl;
    std::cout << "3D-Euclidean lattice (2)" << std::endl;
    int input;
    std::cin >> input;
    switch (input) {
        case 0:
            return 0;
        case 1:
            runEuclidean2D();
            break;
        case 2:
            runEuclidean3D();
            break;
        default:
            main();
    }
}