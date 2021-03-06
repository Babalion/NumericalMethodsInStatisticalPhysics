//
// Created by chris on 14.06.21.
//
#include "ising.h"

/** TASK 1:
 *
 * Simulate Systems with sight-size 128,256,512,1024
 * Reconstruct for each size dependent on temperature
 *  -energy E(T)
 *  -magnetization m(T)
 *  -heat capacity C(T)
 *  -susceptibility chi(T)
 *
 *  We need at least 1E4 measurements per temperature
 *  Calculate the autocorrelations
 */
void runTask1() {
    const float TCritical = 2.0f / std::log(1.0f + std::sqrt(2.0f));
    const float minTemp = 2;
    const float maxTemp = 3;
    const int numOfTemps = 16;
    const int numIterations = 100;
    const unsigned int shuffleAgainAfter = UINT32_MAX;

    std::vector<Simulation> Sims = {Simulation(16, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(32, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(64, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(128, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(256, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter)};

    for (auto &S:Sims) {
        S.sweepsPerIteration = 1E3;
        S.thermalizeSweeps = 1E3;
    }

    std::ofstream file("IsingResultsNew4.tsv");
    file << "numOfTemps:\t" << numOfTemps << std::endl;
    file << "numOfIterations:\t" << numIterations << std::endl << std::endl;
    file << "N\ttemp\tmagnetization\tenergy\tsusceptibility\theatCapacity\n";
    file << std::fixed;
    file.precision(10);
    for (auto &S:Sims) {
        std::cout << "Simulating now N=" << S.getSights() << "\n";
        S.simulate_par();
        std::cout << "Simulation finished. Save results now...\n";

        /// Save measurements to file
        for (size_t i = 0; i < S.getNumOfTemps() * S.getNumOfIterations(); ++i) {
            file << S.getSights() << "\t" << S.getTemps()[i] << "\t"
                 << S.getMagnetization()[i] << "\t" << S.getEnergies()[i]
                 << "\t" << S.getSusceptibility()[i] << "\t" << S.getHeatCapacity()[i] << "\n";
        }
    }

    file.close();


}

int main() {
    runTask1();
}