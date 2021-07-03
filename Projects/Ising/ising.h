//
// Created by chris on 03.07.21.
//
#pragma once

#include "SpinLattice2level.h"

#include <fstream>
#include <string>
#include <thread>

struct workerTask1 {
    workerTask1(int N, int noOfTemps, float tempStep, int numIterations)
            : N(N), noOfTemps(noOfTemps), numIterations(numIterations),
              magnetization(noOfTemps), energy(noOfTemps), susceptibility(noOfTemps),
              heatCapacity(noOfTemps) {
        for (int i = 0; i < noOfTemps; ++i) {
            // calculate temperatures
            temps.push_back(static_cast<float>(i + 1) * tempStep);

            // reserve memory for all vectors
            magnetization[i].reserve(numIterations);
            energy[i].reserve(numIterations);
            susceptibility[i].reserve(numIterations);
            heatCapacity[i].reserve(numIterations);
        }
    };
    const int N;
    //const int startTemp{};
    const int noOfTemps;
    //const float tempStep{};
    const int numIterations;
    std::vector<std::vector<float>> magnetization;
    std::vector<std::vector<float>> energy;
    std::vector<std::vector<float>> susceptibility;
    std::vector<std::vector<float>> heatCapacity;
    std::vector<SpinLattice2level> sl; //this is later the number of parallel ensembles (--> number of threads)
    [[nodiscard]] const std::vector<float> &getTemps() const {
        return temps;
    }

private:
    std::vector<float> temps;
};

/**
 * Calculates wk.energy etc. for given indices as sequential job.
 * This is a helper class for the parallel function see below.
 *
 * @param wk all data of all parallel ensembles
 * @param tempsToCalc the indices of wk.temp which should be calculated in this thread
 * @param threadId consecutive number for each thread
 */
void runWorkTask1Serial(workerTask1 &wk, const std::vector<unsigned int> &tempsToCalc, unsigned int threadId) {
    //thermalize first
    metropolisSweep(wk.sl[threadId], static_cast<float>(wk.getTemps()[0]), 100);
    for (auto &i:tempsToCalc) {
        wk.magnetization[i].reserve(wk.numIterations);
        for (int j = 0; j < wk.numIterations; ++j) {
            metropolisSweep(wk.sl[threadId], static_cast<float>(wk.getTemps()[i]), 5);
            wk.magnetization[i].push_back(
                    static_cast<float>(wk.sl[threadId].calcMagnetization()) / static_cast<float>(wk.N * wk.N));
            wk.energy[i].push_back(static_cast<float>(wk.sl[threadId].calcEnergy()) / static_cast<float>(wk.N * wk.N));
            wk.susceptibility[i].push_back(
                    static_cast<float>(wk.sl[threadId].calcSusceptibility()) / static_cast<float>(wk.N * wk.N));
            wk.heatCapacity[i].push_back(
                    static_cast<float>(wk.sl[threadId].calcHeatCapacity()) / static_cast<float>(wk.N * wk.N));
            if (j > 0 && j % 100 == 0) {
                wk.sl[threadId].initRandom();
                //thermalize again
                metropolisSweep(wk.sl[threadId], static_cast<float>(wk.getTemps()[i]), 100);
            }
        }
        wk.sl[threadId].initRandom();
#ifdef DEBUG
        std::cout << "done temp " << wk.temps[i] <<" in thread " <<threadId<< std::endl;
#endif
    }
}


/**
 * does the main work of the simulation parallelized
 */
void runWorkTask1(workerTask1 &wk) {

    // Divide amount of work
    static const unsigned int hardwareCon = std::thread::hardware_concurrency();
    static const unsigned int supportedThreads = hardwareCon == 0 ? 2 : hardwareCon;

    static const unsigned int workPerThread = wk.noOfTemps / supportedThreads;
    static const unsigned int workRemaining = wk.noOfTemps % supportedThreads;

    static unsigned int amountOfThreads;
    if (workPerThread == 0 && workRemaining > 0) //we have less work than threads
        amountOfThreads = wk.noOfTemps;
    else //we have enough work --> use all cores
        amountOfThreads = supportedThreads;
    //TODO split up remaining work to all cores, not only the last one
#ifdef DEBUG
    std::cout << amountOfThreads << " threads will be used for calculation." << std::endl;
#endif

    /// Create now data structure to divide and store work

    // initialize different ensembles
    for (unsigned int i = 0; i < amountOfThreads; ++i) {
        wk.sl.emplace_back(wk.N);
    }

    // calc to do temperature indices for every thread and one for the potential rest
    std::vector<std::vector<unsigned int>> tempIndices(amountOfThreads + 1);
    for (unsigned int i = 0; i < amountOfThreads + 1; ++i) {
        if (i == amountOfThreads) {
            for (unsigned int j = 0; j < workRemaining; ++j) {
                tempIndices[i].push_back(i * workPerThread + j);
            }
        } else {
            for (unsigned int j = 0; j < workPerThread; ++j) {
                tempIndices[i].push_back(i * workPerThread + j);
            }
        }
    }

    //start thread
    std::vector<std::thread> threads(amountOfThreads - 1);
    for (unsigned int i = 0; i < amountOfThreads - 1; i++) {
        threads[i] = std::thread([&wk, &tempIndices, i]() { runWorkTask1Serial(wk, tempIndices[i], i); });
    }

    runWorkTask1Serial(wk, tempIndices[amountOfThreads - 1], amountOfThreads - 1);
    // do the remaining work
    runWorkTask1Serial(wk, tempIndices[amountOfThreads], amountOfThreads - 1);

    for (auto &i : threads) {
        i.join();
    }


}
