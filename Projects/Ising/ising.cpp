//
// Created by chris on 14.06.21.
//

#include "ising.h"

#include "SpinLattice2level.h"
#include "SpinLatticeTheta.h"
#include <CvPlot/cvplot.h>
#include "utils.h"


#ifdef DEBUG
void runTest() {
    SpinLattice2level sl(75);
    // create arrays to save measurements to
    std::vector<std::vector<float>> lowTemp;
    std::vector<std::vector<float>> lowTempHB;

    // Register the GUI
    // You can specify the dimensions of the window
    RuntimeGUI gui(1000, 1200);
    gui.waitTime = 1;
    gui.notify(sl);

    const int numEnsembles = 100;
    const int numSteps = 100;
    // reserve memory
    lowTemp.reserve(numEnsembles);
    lowTempHB.reserve(numEnsembles);/*
    for (int i = 0; i < numEnsembles; ++i) {
        lowTemp[i].reserve(numSteps);
        lowTempHB[i].reserve(numSteps);
    }*/


    /// Start simulation
    for (int i = 0; i < numEnsembles; i++) {
        lowTemp.push_back(std::vector<float>{(float) sl.calcEnergy()});
        for (int j = 0; j < numSteps; ++j) {
            metropolisSweep(sl, 2, 1);
            lowTemp[i].push_back(sl.calcEnergy());
            if (j == 0)
                lowTemp[i].reserve(numSteps);
            if (j % 1000 == 0)
                gui.notify(sl);
        }
        sl = SpinLattice2level(75);
    }

    sl = SpinLattice2level(75);
    for (int i = 0; i < numEnsembles; i++) {
        lowTempHB.push_back(std::vector<float>{(float) sl.calcEnergy()});
        for (int j = 0; j < numSteps; ++j) {
            heatBathSweep(sl, 2);
            lowTempHB[i].push_back(sl.calcEnergy());
            //if (i % 100 == 0)
            //gui.notify(sl);
        }
        sl = SpinLattice2level(75);
    }


    /// we can save a screenshot of the gui to disk (so you also can generate videos with ffmpeg)
    //gui.saveImageToDisk("2levelIsing.png", "./results/");

    /// ---------------------------------------------------------------------------------------------------------------
    /// Plot and calculate measured energy
    /// ---------------------------------------------------------------------------------------------------------------
    /*
    std::vector<float> x(lowTemp.size());
    for (size_t i = 0; i < x.size(); i++) {
        x[i] = static_cast<float>(i);
    }
    auto axes = CvPlot::makePlotAxes();
    axes.create<CvPlot::Series>(x, lowTemp, "-g").setMarkerType(CvPlot::MarkerType::Circle).setMarkerSize(
            10).setLineType(CvPlot::LineType::None);
    axes.create<CvPlot::Series>(x, lowTempHB, "-r").setMarkerType(CvPlot::MarkerType::Circle).setMarkerSize(
            10).setLineType(CvPlot::LineType::None);
*/
    std::cout << "mean(lowT)= " << mean(lowTemp) << std::endl;
    std::cout << "mean(lowT_HB)= " << mean(lowTempHB) << std::endl;

    //CvPlot::show("plotPhysics", axes);

    /// ---------------------------------------------------------------------------------------------------------------
    /// Calculate and plot autocorrelation
    /// ---------------------------------------------------------------------------------------------------------------

    auto axesAC = CvPlot::makePlotAxes();
    for (int i = 0; i < numEnsembles; ++i) {
        std::vector<float> ac_le = autoCorr(lowTemp[i]);
        normalize(ac_le);
        std::vector<float> acHB_le = autoCorr(lowTempHB[i]);
        normalize(acHB_le);

        axesAC.create<CvPlot::Series>(ac_le, "-b").setName("Metropolis (T=0.1)");
        axesAC.create<CvPlot::Series>(acHB_le, "-g").setName("Heatbath (T=0.1)");
    }
    axesAC.xLabel("simulation steps n").yLabel("normalized autocorrelation");
    axesAC.title("Autocorrelation Metropolis vs. Heatbath in 2D Ising");
    axesAC.create<Legend>()._parentAxes = &axesAC;
    CvPlot::show("autocorr", axesAC);

    cv::waitKey(0);
}
#endif

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
    const int noOfTemps = 16;
    const float tempStep = maxTemp / noOfTemps;
    const int numIterations = 500;

    std::vector<workerTask1> wk = {workerTask1(64, noOfTemps, tempStep, numIterations),
                                   workerTask1(128, noOfTemps, tempStep, numIterations),
                                   workerTask1(40, noOfTemps, tempStep, numIterations),
                                   workerTask1(20, noOfTemps, tempStep, numIterations)};

    /// Start simulation
    for (auto &w:wk) {
        std::cout << "Simulating now N=" << w.N << "\n";
        runWorkTask1(w);
    }

    /// ---------------------------------------------------------------------------------------------------------------
    /// Plot and calculate measured parameters
    /// ---------------------------------------------------------------------------------------------------------------
    auto axesMagnetization = CvPlot::makePlotAxes();
    for (size_t i = 0; i < noOfTemps; ++i) {
        std::vector<float> x(wk[0].magnetization[i].size());
        std::fill(x.begin(), x.end(), wk[0].getTemps()[i]);
        axesMagnetization.create<CvPlot::Series>(x, wk[0].magnetization[i], "-r").setLineSpec("o").setMarkerSize(2);
        axesMagnetization.create<CvPlot::Series>(x, wk[1].magnetization[i], "-g").setLineSpec("o").setMarkerSize(2);
        axesMagnetization.create<CvPlot::Series>(x, wk[2].magnetization[i], "-b").setLineSpec("o").setMarkerSize(2);
        axesMagnetization.create<CvPlot::Series>(x, wk[3].magnetization[i], "-c").setLineSpec("o").setMarkerSize(2);
    }
    axesMagnetization.xLabel("temperature T").yLabel("magnetization m");
    axesMagnetization.title("magnetization vs temperature");
    CvPlot::show("magnetization", axesMagnetization);

    auto axesEnergy = CvPlot::makePlotAxes();
    for (size_t i = 0; i < noOfTemps; ++i) {
        std::vector<float> x(wk[0].energy[i].size());
        std::fill(x.begin(), x.end(), wk[0].getTemps()[i]);
        axesEnergy.create<CvPlot::Series>(x, wk[0].energy[i], "-r").setLineSpec("o").setMarkerSize(2);
        axesEnergy.create<CvPlot::Series>(x, wk[1].energy[i], "-g").setLineSpec("o").setMarkerSize(2);
        axesEnergy.create<CvPlot::Series>(x, wk[2].energy[i], "-b").setLineSpec("o").setMarkerSize(2);
        axesEnergy.create<CvPlot::Series>(x, wk[3].energy[i], "-c").setLineSpec("o").setMarkerSize(2);
    }
    axesEnergy.xLabel("temperature T").yLabel("energy E");
    axesEnergy.title("energy vs temperature");
    CvPlot::show("energy", axesEnergy);
    /// ---------------------------------------------------------------------------------------------------------------
    /// Save measurements to file
    /// ---------------------------------------------------------------------------------------------------------------

    std::cout << "Simulation finished. Save results now...\n";
    std::ofstream file("IsingResults.tsv");
    file << "numOfTemps:\t" << noOfTemps << std::endl;
    file << "numOfIterations:\t" << numIterations << std::endl << std::endl;
    file << "N\ttemp\tmagnetization\tenergy\tsusceptibility\theatCapacity\n";
    file << std::fixed;
    file.precision(5);
    for (auto &w:wk) {
        for (int i = 0; i < w.noOfTemps; ++i) {
            for (int j = 0; j < w.numOfIterations; ++j) {
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