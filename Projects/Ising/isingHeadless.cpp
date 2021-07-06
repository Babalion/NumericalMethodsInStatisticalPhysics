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
    const float minTemp = 1;
    const float maxTemp = 4;
    const int numOfTemps = 2;
    const int numIterations = 100;
    const unsigned int shuffleAgainAfter = 50;
/*
    std::vector<workerTask1> wk = {workerTask1(64, 0, numOfTemps, minTemp, maxTemp, numOfIterations, shuffleAgainAfter),
                                   workerTask1(128, 0, numOfTemps, minTemp, maxTemp, numOfIterations, shuffleAgainAfter),
                                   workerTask1(256, 0, numOfTemps, minTemp, maxTemp, numOfIterations, shuffleAgainAfter),
                                   workerTask1(512, 0, numOfTemps, minTemp, maxTemp, numOfIterations, shuffleAgainAfter),
                                   workerTask1(1024, 0, numOfTemps, minTemp, maxTemp, numOfIterations, shuffleAgainAfter)};
*/
    std::vector<Simulation> Sims = {Simulation(10, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(11, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(12, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(20, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter),
                                    Simulation(35, numOfTemps, minTemp, maxTemp, numIterations, shuffleAgainAfter)};

    std::ofstream file("IsingResults.tsv");
    file << "numOfTemps:\t" << numOfTemps << std::endl;
    file << "numOfIterations:\t" << numIterations << std::endl << std::endl;
    file << "N\ttemp\tmagnetization\tenergy\tsusceptibility\theatCapacity\n";
    file << std::fixed;
    file.precision(5);
    for (auto &S:Sims) {
        std::cout << "Simulating now N=" << S.getSights() << "\n";
        S.simulate_seq();
        std::cout << "Simulation finished. Save results now...\n";

        /// Save measurements to file
        for (size_t i = 0; i < S.getNumOfTemps() * S.getNumIterations(); ++i) {
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