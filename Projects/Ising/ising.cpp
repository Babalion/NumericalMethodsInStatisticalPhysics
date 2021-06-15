//
// Created by chris on 14.06.21.
//
#include "SpinLattice2level.h"
#include "SpinLatticeTheta.h"
#include <CvPlot/cvplot.h>

int main() {
    SpinLattice2level sl(75);
    sl.J=1;
    std::vector<float> lowEnergy;
    std::vector<float> highEnergy;
    std::vector<float> lowEnergyHB;
    std::vector<float> highEnergyHB;
    std::vector<float> entropy;

    // Register the GUI
    // You can specify the dimensions of the window
    RuntimeGUI gui(1024, 1024);
    gui.waitTime = 7;
    gui.notify(sl);

    const int runs = 10;
    lowEnergy.reserve(runs);
    entropy.reserve(runs);
    for (int i = 0; i < runs; i++) {
        metropolisSweep(sl, 0.001, 100);
        lowEnergy.push_back(sl.calcEnergy());
        gui.notify(sl);
    }
    for (int i = 0; i < runs; i++) {
        metropolisSweep(sl, 1000, 100);
        highEnergy.push_back(sl.calcEnergy());
        gui.notify(sl);
    }
    for (int i = 0; i < runs; i++) {
        heatBathSweep(sl, 0.001);
        lowEnergyHB.push_back(sl.calcEnergy());
        gui.notify(sl);
    }
    for (int i = 0; i < runs; i++) {
        heatBathSweep(sl, 1000);
        highEnergyHB.push_back(sl.calcEnergy());
        gui.notify(sl);
    }
    cv::waitKey(0);

    std::vector<float> x(lowEnergy.size());
    for (size_t i = 0; i < x.size(); i++) {
        x[i] = i;
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
    std::cout << "mean(lowE)= " << std::accumulate(lowEnergy.begin(), lowEnergy.end(), 0.0f) << std::endl;
    std::cout << "mean(highE)= " << std::accumulate(highEnergy.begin(), highEnergy.end(), 0.0f) << std::endl;
    std::cout << "mean(lowE_HB)= " << std::accumulate(lowEnergyHB.begin(), lowEnergyHB.end(), 0.0f) << std::endl;
    std::cout << "mean(highE_HB)= " << std::accumulate(highEnergyHB.begin(), highEnergyHB.end(), 0.0f) << std::endl;
    //axes.create<CvPlot::Series>(x, entropy, "-r");
    CvPlot::show("plotPhysics", axes);

}