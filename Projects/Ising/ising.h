//
// Created by chris on 03.07.21.
//
#pragma once

#include "SpinLattice2level.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <string>
#include <thread>

/*
struct workerTask1 {
    workerTask1(int N, int h, int noOfTemps, float tempStart, float tempEnd, int numOfIterations,
                unsigned int shuffleAgainAfter)
            : N(N), h(h), noOfTemps(noOfTemps), numOfIterations(numOfIterations), shuffleAgainAfter(shuffleAgainAfter),
              magnetization(noOfTemps), energy(noOfTemps), susceptibility(noOfTemps), heatCapacity(noOfTemps) {

        const float tempStep = (tempEnd - tempStart) / (noOfTemps - 1);
        for (int i = 0; i < noOfTemps; ++i) {
            // calculate temperatures
            temps.push_back(static_cast<float>(i) * tempStep + tempStart);

            // reserve memory for all vectors
            magnetization[i].reserve(numOfIterations);
            energy[i].reserve(numOfIterations);
            susceptibility[i].reserve(numOfIterations);
            heatCapacity[i].reserve(numOfIterations);
        }
    };
    const int N;
    const short h;
    const int noOfTemps;
    const int numOfIterations;
    const unsigned int shuffleAgainAfter;
    std::vector<std::vector<float>> magnetization;
    std::vector<std::vector<float>> energy;
    std::vector<std::vector<float>> susceptibility;
    std::vector<std::vector<float>> heatCapacity;

    // std::vector<SpinLattice2level> sl; //this is later the number of parallel ensembles (--> number of threads)
    [[nodiscard]] const std::vector<float> &getTemps() const {
        return temps;
    }

private:
    std::vector<float> temps;
};
*/
/**
 * Calculates wk.energy etc. for given indices as sequential job.
 * This is a helper class for the parallel function see below.
 *
 * @param wk all data of all parallel ensembles
 * @param tempsToCalc the indices of wk.temp which should be calculated in this thread
 * @param threadId consecutive number for each thread
 */
/*
void runWorkTask1Serial(workerTask1 &wk, const std::vector<float> &tempsToCalc, unsigned int threadId) {
   SpinLattice2level sl(wk.N, wk.h);
   for (unsigned int i = 0; i < tempsToCalc.size(); i++) {
       sl.initRandom();
       //thermalize first
       metropolisSweep(sl, tempsToCalc[i], 100);
       for (int j = 0; j < wk.numOfIterations; ++j) {
           metropolisSweep(sl, tempsToCalc[i], 50);
           wk.magnetization[i].push_back(sl.calcMagnetization());
           wk.energy[i].push_back(sl.calcEnergy());
           wk.susceptibility[i].push_back(sl.calcSusceptibility());
           wk.heatCapacity[i].push_back(sl.calcHeatCapacity());
           if (j > 0 && j % wk.shuffleAgainAfter == 0) { //initialize new
               sl.initRandom();
               //thermalize again
               metropolisSweep(sl, tempsToCalc[i], 100);
           }
       }
       auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
       std::cout << std::ctime(&time) << "done temp " << tempsToCalc[i] << " in thread " << threadId
                 << ". Remaining: "
                 << tempsToCalc.size() - i << std::endl;
   }
}*/


class Simulation {
public:
    /**
     * Creates a set for a Ising-Simulation with given parameters
     * @param sights
     * @param numOfTemps
     * @param tempStart
     * @param tempEnd
     * @param numIterations
     * @param shuffleAgainAfter
     */
    Simulation(unsigned int sights, int numOfTemps, float tempStart, float tempEnd, int numIterations,
               unsigned int shuffleAgainAfter)
            : sights(sights), tempStart(tempStart), tempEnd(tempEnd), numOfTemps(numOfTemps),
              numOfIterations(numIterations),
              shuffleAgainAfter(shuffleAgainAfter), sl(sights), isSimulated(false) {
        init();
    }

/*
    Simulation(const Simulation &other)
            : sights(other.sights), tempStart(other.tempStart), tempEnd(other.tempEnd), numOfTemps(other.numOfTemps),
              numOfIterations(other.numOfIterations),
              shuffleAgainAfter(other.shuffleAgainAfter), sl(other.sights), isSimulated(false) {
    }
*/

    void simulate_seq() {
        if (isSimulated) {
            std::cerr << "This simulation is already finished.\n";
        } else {
            for (unsigned int i = 0; i < temps.size(); i++) {
                // shuffle sl to obtain maybe a different equilibrate state
                if (i % shuffleAgainAfter == 0) {
                    sl.initRandom();
                    metropolisSweep(sl, temps[i], 1000);
                    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::cout.precision(3);
                    std::cout << std::ctime(&time) << "N="<<sights<<"\tprogress:" << 100.0f*i/temps.size() <<"%"<< std::endl;
                }
                metropolisSweep(sl, temps[i], 200);
                energies.push_back(sl.calcEnergy());
                magnetization.push_back(sl.calcMagnetization());
                susceptibility.push_back(sl.calcSusceptibility());
                heatCapacity.push_back(sl.calcHeatCapacity());
                if (i > 0 && i % numOfIterations == 0) {
                    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::cout << std::ctime(&time) << "done temp " << temps[i] << ". Remaining: "
                              << (temps.size() - i) / numOfIterations << std::endl;
                }

            }
            isSimulated = true;
        }
    }


    /**
 * sumlate the simulation parallelized
 */
    void simulate_par() {
        // -------------- Divide amount of work -------------------------------------------------
        static const unsigned int hardwareCon = std::thread::hardware_concurrency();
        static const unsigned int supportedThreads = hardwareCon == 0 ? 2 : hardwareCon;

        static const unsigned int workPerThread = numOfTemps / supportedThreads;
        static const unsigned int workRemaining = numOfTemps % supportedThreads;

        static unsigned int amountOfThreads;
        if (workPerThread == 0 && workRemaining > 0) //we have less work than threads
            amountOfThreads = numOfTemps;
        else //we have enough work --> use all cores
            amountOfThreads = supportedThreads;
        //TODO split up remaining work to all cores, not only the last one
        std::cout << amountOfThreads << " threads will be used for calculation." << std::endl;
        // --------------------------------------------------------------------------------------


        /// Create now data structure to divide and store work: initialize different ensembles
        std::vector<Simulation> Sims;
        Sims.reserve(amountOfThreads);
        float tempStep = (tempEnd - tempStart) / static_cast<float>(numOfTemps - 1);
        for (unsigned int i = 0; i < amountOfThreads; ++i) {
            float startTemp = tempStart + static_cast<float>(i * workPerThread) * tempStep;
            float endTemp;

            if (i == amountOfThreads - 1) {//this is the last case, include the rest now
                Sims.emplace_back(sights, workPerThread + workRemaining, startTemp, tempEnd, numOfIterations,
                                  shuffleAgainAfter);
            } else {
                endTemp = tempStart + static_cast<float>((i + 1) * workPerThread - 1) * tempStep;
                Sims.emplace_back(sights, workPerThread, startTemp, endTemp, numOfIterations, shuffleAgainAfter);
            }

        }
#ifdef DEBUG
        for (const auto &s:Sims) {
            std::cout.precision(3);
            std::cout << "Simulate from tS=" << s.getTempStart() << "\tuntil tE=" << s.getTempEnd() << "\twith\t"
                      << s.getNumOfTemps() << " temps, temps.size()=\t" << s.getTemps().size() << std::endl;
        }
#endif

        //start threads
        std::vector<std::thread> threads(amountOfThreads - 1);
        for (unsigned int i = 0; i < threads.size(); i++) {
            threads[i] = std::thread([&Sims, i]() { Sims[i].simulate_seq(); });
        }

        // do the remaining work
        Sims[amountOfThreads - 1].simulate_seq();

        for (auto &i : threads) {
            i.join();
        }

        for (const auto &Simulations:Sims) {
            //TODO check if measurements correspond to temps
#ifdef DEBUG
            std::cout << "Simulations.getTemps().size()=" << Simulations.getTemps().size() << std::endl;
#endif
            energies.insert(end(energies), begin(Simulations.getEnergies()), end(Simulations.getEnergies()));
            magnetization.insert(end(magnetization), begin(Simulations.getMagnetization()),
                                 end(Simulations.getMagnetization()));
            susceptibility.insert(end(susceptibility), begin(Simulations.getSusceptibility()),
                                  end(Simulations.getSusceptibility()));
            heatCapacity.insert(end(heatCapacity), begin(Simulations.getHeatCapacity()),
                                end(Simulations.getHeatCapacity()));
        }
        isSimulated = true;
#ifdef DEBUG
        if(temps.size()!=numOfTemps*numOfIterations){
            std::cerr<<"results size not matched!\n";
            exit(15);
        }
#endif

    }


    [[nodiscard]] unsigned int getSights() const {
        return sights;
    }

    [[nodiscard]] unsigned int getNumOfTemps() const {
        return numOfTemps;
    }

    [[nodiscard]] float getTempStart() const {
        return tempStart;
    }

    [[nodiscard]] float getTempEnd() const {
        return tempEnd;
    }

    [[nodiscard]] unsigned int getNumOfIterations() const {
        return numOfIterations;
    }

    [[nodiscard]] unsigned int getShuffleAgainAfter() const {
        return shuffleAgainAfter;
    }

    [[nodiscard]] const std::vector<float> &getTemps() const {
        return temps;
    }

    [[nodiscard]] const std::vector<float> &getEnergies() const {
        return energies;
    }

    [[nodiscard]] const std::vector<float> &getMagnetization() const {
        return magnetization;
    }

    [[nodiscard]] const std::vector<float> &getSusceptibility() const {
        return susceptibility;
    }

    [[nodiscard]] const std::vector<float> &getHeatCapacity() const {
        return heatCapacity;
    }

private:
    /// Parameters for simulation
    unsigned int sights;
    float tempStart;
    float tempEnd;
    unsigned int numOfTemps;
    unsigned int numOfIterations;
    unsigned int shuffleAgainAfter;

    /// Results of simulation
    std::vector<float> temps;
    std::vector<float> energies;
    std::vector<float> magnetization;
    std::vector<float> susceptibility;
    std::vector<float> heatCapacity;

    // SpinLattice for simulation
    SpinLattice2level sl;
    bool isSimulated;
private:
    void init() {
        // reserve memory for results
        temps.reserve(numOfTemps * numOfIterations);
        energies.reserve(numOfTemps * numOfIterations);
        magnetization.reserve(numOfTemps * numOfIterations);
        susceptibility.reserve(numOfTemps * numOfIterations);
        heatCapacity.reserve(numOfTemps * numOfIterations);

        // calculate temps
        for (unsigned int i = 0; i < numOfTemps; ++i) {
            float temp = tempStart + static_cast<float>(i) * (tempEnd - tempStart) / static_cast<float>(numOfTemps - 1);
            for (unsigned int j = 0; j < numOfIterations; ++j) {
                temps.push_back(temp);
            }
        }

    }
};
