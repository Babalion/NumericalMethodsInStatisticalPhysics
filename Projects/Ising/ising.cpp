//
// Created by chris on 14.06.21.
//
#include "SpinLattice2level.h"
#include "SpinLatticeTheta.h"
#include <CvPlot/cvplot.h>
#include "utils.h"

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

/** TASK 1:
 *
 * Simulate Systems with sight-size 128,256,512,1024
 * Reconstruct for each size dependent on temperature
 *  -energy E(T)
 *  -magnetization m(T)
 *  -heat capacity C(T)
 *  -optional: susceptibility chi(T)
 *
 *  We need at least 1E4 measurements per temperature
 *  Calculate the autocorrelations
 */
void runTask1() {
    const int N = 128;// number of spins at quadratic sight
    const int maxTemp = 20;
    const int noOfTemps = 20;
    const int tempStep = maxTemp / noOfTemps;
    const int numIterations = 100;


    //Create the ising-field
    SpinLattice2level sl(N);
    // create vectors to save measurements to
    std::vector<int> temps;
    temps.reserve(noOfTemps);
    std::vector<std::vector<float>> energy(noOfTemps);
    std::vector<std::vector<float>> magnetization(noOfTemps);


    /// Start simulation
    for (int i = 0; i < noOfTemps; ++i) {
        temps.push_back(i * tempStep);
        magnetization[i].reserve(numIterations);
        for (int j = 0; j < numIterations; ++j) {
            metropolisSweep(sl, static_cast<float>(temps[i]), 10);
            magnetization[i].push_back(static_cast<float>(sl.calcMagnetization()) / N / N);
            energy[i].push_back(static_cast<float>(sl.calcEnergy()) / N / N);
        }
        sl.initRandom();
        std::cout << "temps: " << temps[i] << std::endl;
    }

    /// ---------------------------------------------------------------------------------------------------------------
    /// Plot and calculate measured parameters
    /// ---------------------------------------------------------------------------------------------------------------

    auto axes = CvPlot::makePlotAxes();
    for (size_t i = 0; i < noOfTemps; ++i) {
        std::vector<int> x(magnetization[i].size());
        std::fill(x.begin(), x.end(), temps[i]);
        axes.create<CvPlot::Series>(x, magnetization[i], "-b").setLineSpec("o").setMarkerSize(2);
    }
    axes.xLabel("temperature T").yLabel("magnetization m");
    axes.title("magnetization vs temperature");
    CvPlot::show("magnetization", axes);
}

int main() {
    runTask1();
}