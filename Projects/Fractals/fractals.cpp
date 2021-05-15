//
// Created by chris on 15.05.21.
//
#include <iostream>
#include <fstream>
#include "Euclidean2DGraph.h"


void runEuclidean2D() {

    const int maxIterations = 1E4;
    const int highestMaxSteps = 1E5;
    const int logInterval = 1000;

    Euclidean2DGraph eu2D("euclidean2D.tsv");
    eu2D.generateDimensionData(maxIterations, highestMaxSteps, logInterval);
}


int main() {
    std::cout << "Welcome to fractals! You can calc the local spectral dimension of fractals with this tool."
              << std::endl;
    std::cout << "Which fractal do you want to calculate? Choose between:" << std::endl;
    std::cout << "exit (0)" << std::endl;
    std::cout << "2D-Euclidean lattice (1)" << std::endl;
    int input;
    std::cin >> input;
    switch (input) {
        case 0:
            return 0;
        case 1:
            runEuclidean2D();
            break;
        default:
            main();
    }
}