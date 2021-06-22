//
// Created by chris on 14.06.21.
//
#include "SpinLattice2level.h"
#include "SpinLatticeTheta.h"
#include <CvPlot/cvplot.h>
#include "utils.h"

int main() {
    SpinLattice2level sl(75);
    // create arrays to save measurements to
    std::vector<float> lowEnergy;
    std::vector<float> highEnergy;
    std::vector<float> lowEnergyHB;
    std::vector<float> highEnergyHB;

    // Register the GUI
    // You can specify the dimensions of the window
    RuntimeGUI gui(1000, 1200);
    gui.waitTime = 1;
    gui.notify(sl);

    const int runs = 50000;
    lowEnergy.reserve(runs);
    highEnergy.reserve(runs);
    lowEnergyHB.reserve(runs);
    highEnergyHB.reserve(runs);

    /// Start simulation
    for (int i = 0; i < runs; i++) {
        metropolisSweep(sl, 2, 1);
        lowEnergy.push_back(sl.calcEnergy());
        if (i % 100 == 0)
            gui.notify(sl);
        if(i%1000==0)
            sl = SpinLattice2level(75);
    }
    sl = SpinLattice2level(75);
    for (int i = 0; i < runs; i++) {
        metropolisSweep(sl, 100, 1);
        highEnergy.push_back(sl.calcEnergy());
        if (i % 100 == 0)
            gui.notify(sl);
        if(i%1000==0)
            sl = SpinLattice2level(75);
    }
    sl = SpinLattice2level(75);
    for (int i = 0; i < runs; i++) {
        heatBathSweep(sl, 2);
        lowEnergyHB.push_back(sl.calcEnergy());
        if (i % 100 == 0)
            gui.notify(sl);
        if(i%1000==0)
            sl = SpinLattice2level(75);
    }
    sl = SpinLattice2level(75);
    for (int i = 0; i < runs; i++) {
        heatBathSweep(sl, 100);
        highEnergyHB.push_back(sl.calcEnergy());
        if (i % 100 == 0)
            gui.notify(sl);
        if(i%1000==0)
            sl = SpinLattice2level(75);
    }
    //cv::waitKey(0);

    /// we can save a screenshot of the gui to disk (so you also can generate videos with ffmpeg)
    gui.saveImageToDisk("2levelIsing.png", "./results/");

    /// ---------------------------------------------------------------------------------------------------------------
    /// Plot and calculate measured energy
    /// ---------------------------------------------------------------------------------------------------------------
    std::vector<float> x(lowEnergy.size());
    for (size_t i = 0; i < x.size(); i++) {
        x[i] = static_cast<float>(i);
    }
    auto axes = CvPlot::makePlotAxes();
    axes.create<CvPlot::Series>(x, lowEnergy, "-g").setMarkerType(CvPlot::MarkerType::Circle).setMarkerSize(
            10).setLineType(CvPlot::LineType::None);
    axes.create<CvPlot::Series>(x, highEnergy, "-b").setMarkerType(CvPlot::MarkerType::Circle).setMarkerSize(
            10).setLineType(CvPlot::LineType::None);
    axes.create<CvPlot::Series>(x, lowEnergyHB, "-r").setMarkerType(CvPlot::MarkerType::Circle).setMarkerSize(
            10).setLineType(CvPlot::LineType::None);
    axes.create<CvPlot::Series>(x, highEnergyHB, "-c").setMarkerType(CvPlot::MarkerType::Circle).setMarkerSize(
            10).setLineType(CvPlot::LineType::None);

    std::cout << "mean(lowE)= " << mean(lowEnergy) << std::endl;
    std::cout << "mean(highE)= " << mean(highEnergy) << std::endl;
    std::cout << "mean(lowE_HB)= " << mean(lowEnergyHB) << std::endl;
    std::cout << "mean(highE_HB)= " << mean(highEnergyHB) << std::endl;

    CvPlot::show("plotPhysics", axes);

    /// ---------------------------------------------------------------------------------------------------------------
    /// Calculate and plot autocorrelation
    /// ---------------------------------------------------------------------------------------------------------------
    std::vector<float> ac_le = autoCorr(lowEnergy);
    normalize(ac_le);
    std::vector<float> acHB_le = autoCorr(lowEnergyHB);
    normalize(acHB_le);
    std::vector<float> ac_he = autoCorr(highEnergy);
    normalize(ac_he);
    std::vector<float> acHB_he = autoCorr(highEnergyHB);
    normalize(acHB_he);

    auto axesAC = CvPlot::makePlotAxes();
    axesAC.create<CvPlot::Series>(ac_le, "-b").setName("Metropolis (T=0.1)");
    axesAC.create<CvPlot::Series>(acHB_le, "-g").setName("Heatbath (T=0.1)");
    axesAC.create<CvPlot::Series>(ac_he, "-r").setName("Metropolis (T=100)");
    axesAC.create<CvPlot::Series>(acHB_he, "-c").setName("Heatbath (T=100)");
    axesAC.xLabel("simulation steps n").yLabel("normalized autocorrelation");


    axesAC.title("Autocorrelation Metropolis vs. Heatbath in 2D Ising");
    axesAC.create<Legend>()._parentAxes = &axesAC;
    CvPlot::show("autocorr", axesAC);

#ifdef DEBUG
    std::cout<<"E=[";
    for (size_t i = 0; i < lowEnergy.size()-1; ++i) {
        std::cout<<lowEnergy[i]<<", ";
    }
    std::cout<<lowEnergy[lowEnergy.size()-1]<<"];";
#endif

}