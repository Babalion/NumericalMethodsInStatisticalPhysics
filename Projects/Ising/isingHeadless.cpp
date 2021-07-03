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
    const float maxTemp = 6;
    const int noOfTemps = 12;
    const float tempStep = maxTemp / noOfTemps;
    const int numIterations = 500;

    std::vector<workerTask1> wk = {workerTask1(64, noOfTemps, tempStep, numIterations),
                                   workerTask1(128, noOfTemps, tempStep, numIterations),
                                   workerTask1(40, noOfTemps, tempStep, numIterations),
                                   workerTask1(20, noOfTemps, tempStep, numIterations)};

    /// Start simulation
    for (auto &w:wk) {
        std::cout << "Simulating now N="<<w.N<<"\n";
        runWorkTask1(w);
    }

    /// ---------------------------------------------------------------------------------------------------------------
    /// Save measurements to file
    /// ---------------------------------------------------------------------------------------------------------------

    std::cout << "Simulation finished. Save results now...\n";
    std::ofstream file("IsingResults.tsv");
    file<<"numOfTemps:\t"<<noOfTemps<<std::endl;
    file<<"numOfIterations:\t"<<numIterations<<std::endl<<std::endl;
    file << "N\ttemp\tmagnetization\tenergy\tsusceptibility\theatCapacity\n";
    file << std::fixed;
    file.precision(5);
    for (auto &w:wk) {
        for (int i = 0; i < w.noOfTemps; ++i) {
            for (int j = 0; j < w.numIterations; ++j) {
                file << w.N << "\t" << w.getTemps()[i] << "\t"
                     << w.magnetization[i][j] << "\t" << w.energy[i][j]
                     << "\t" << w.susceptibility[i][j] << "\t" << w.heatCapacity[i][j] << "\n";
            }
        }
    }
    file.close();


}

int main() {
    runTask1();
}