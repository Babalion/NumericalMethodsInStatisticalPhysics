//
// Created by chris on 14.06.21.
//
#include "SpinLattice2level.h"
#include "SpinLatticeTheta.h"
#include <CvPlot/cvplot.h>
#include "utils.h"

#include <thread>
#include <utility>

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

struct workerTask1 {
    workerTask1(int N, int noOfTemps, float tempStep, int numIterations)
            : N(N), noOfTemps(noOfTemps), numIterations(numIterations),
              magnetization(noOfTemps), energy(noOfTemps), susceptibility(noOfTemps),
              heatCapacity(noOfTemps) {
        for (int i = 0; i < noOfTemps; ++i) {
            // calculate temperatures
            temps.push_back(static_cast<float>(i+1) * tempStep);

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
    const int N = 128;// number of spins at quadratic sight
    const float maxTemp = 6;
    const int noOfTemps = 40;
    const float tempStep = maxTemp / noOfTemps;
    const int numIterations = 400;


    workerTask1 wk0(64, noOfTemps, tempStep, numIterations);
    workerTask1 wk1(128, noOfTemps, tempStep, numIterations);
    workerTask1 wk2(256, noOfTemps, tempStep, numIterations);
    workerTask1 wk3(512, noOfTemps, tempStep, numIterations);

    /// Start simulation
    std::cout<<"Simulating now N=64\n";
    runWorkTask1(wk0);
    std::cout<<"Simulating now N=128\n";
    runWorkTask1(wk1);
    std::cout<<"Simulating now N=256\n";
    runWorkTask1(wk2);
    std::cout<<"Simulating now N=512\n";
    runWorkTask1(wk3);

    /// ---------------------------------------------------------------------------------------------------------------
    /// Plot and calculate measured parameters
    /// ---------------------------------------------------------------------------------------------------------------

    auto axesMagnetization = CvPlot::makePlotAxes();
    for (size_t i = 0; i < noOfTemps; ++i) {
        std::vector<float> x(wk0.magnetization[i].size());
        std::fill(x.begin(), x.end(), wk0.getTemps()[i]);
        axesMagnetization.create<CvPlot::Series>(x, wk0.magnetization[i], "-r").setLineSpec("o").setMarkerSize(2);
        axesMagnetization.create<CvPlot::Series>(x, wk1.magnetization[i], "-g").setLineSpec("o").setMarkerSize(2);
        axesMagnetization.create<CvPlot::Series>(x, wk2.magnetization[i], "-b").setLineSpec("o").setMarkerSize(2);
        axesMagnetization.create<CvPlot::Series>(x, wk3.magnetization[i], "-c").setLineSpec("o").setMarkerSize(2);
    }
    axesMagnetization.xLabel("temperature T").yLabel("magnetization m");
    axesMagnetization.title("magnetization vs temperature");
    CvPlot::show("magnetization", axesMagnetization);

    auto axesEnergy = CvPlot::makePlotAxes();
    for (size_t i = 0; i < noOfTemps; ++i) {
        std::vector<float> x(wk0.energy[i].size());
        std::fill(x.begin(), x.end(), wk0.getTemps()[i]);
        axesEnergy.create<CvPlot::Series>(x, wk0.energy[i], "-r").setLineSpec("o").setMarkerSize(2);
        axesEnergy.create<CvPlot::Series>(x, wk1.energy[i], "-g").setLineSpec("o").setMarkerSize(2);
        axesEnergy.create<CvPlot::Series>(x, wk2.energy[i], "-b").setLineSpec("o").setMarkerSize(2);
        axesEnergy.create<CvPlot::Series>(x, wk3.energy[i], "-c").setLineSpec("o").setMarkerSize(2);
    }
    axesEnergy.xLabel("temperature T").yLabel("energy E");
    axesEnergy.title("energy vs temperature");
    CvPlot::show("energy", axesEnergy);
}

int main() {
    runTask1();
}