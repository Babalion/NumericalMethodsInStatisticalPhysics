//
// Created by chris on 15.05.21.
//
#include <iostream>
#include <thread>
#include <string>
#include "Euclidean2DGraph.h"
#include "Euclidean3DGraph.h"


void runEuclidean2D() {

    static const int maxIterations = 1E5;
    static const int highestMaxSteps = 1E5;

    Euclidean2DGraph eu2D("euclidean2D.tsv");
    eu2D.comebacksUntilMaxStep(maxIterations, highestMaxSteps);
}

void runEuclidean2D_parallel() {
    static const unsigned int amountOfThreads = 4;
    static const int maxIterations = 1E5;
    static const int highestMaxSteps = 1E7;


    std::vector<std::thread> threads(amountOfThreads - 1);
    std::vector<Euclidean2DGraph *> eu2D(amountOfThreads);
    static const std::string filename_prefix = "euclidean2D_";
    for (int i = 0; i < threads.size(); i++) {
        eu2D[i] = new Euclidean2DGraph(filename_prefix + std::to_string(i) + ".tsv");
        threads[i] = std::thread(
                [](Euclidean2DGraph *eu2dGraph) { eu2dGraph->comebacksUntilMaxStep(maxIterations, highestMaxSteps); },
                eu2D[i]);
    }

    eu2D[amountOfThreads - 1] = new Euclidean2DGraph(filename_prefix + std::to_string(amountOfThreads - 1) + ".tsv");
    eu2D[amountOfThreads - 1]->comebacksUntilMaxStep(maxIterations, highestMaxSteps);

    for (auto &i : threads) {
        i.join();
    }
}

void runEuclidean3D() {

    const int maxIterations = 1E5;
    const int highestMaxSteps = 1E5;
    //const int logInterval = 1000;

    Euclidean3DGraph eu3D("euclidean3D.tsv");
    eu3D.comebacksUntilMaxStep(maxIterations, highestMaxSteps);
}

void runEuclidean3D_parallel() {
    static const unsigned int amountOfThreads = 4;
    static const int maxIterations = 1E5;
    static const int highestMaxSteps = 1E6;


    std::vector<std::thread> threads(amountOfThreads - 1);
    std::vector<Euclidean3DGraph *> eu3D(amountOfThreads);
    static const std::string filename_prefix = "euclidean3D_";
    for (int i = 0; i < threads.size(); i++) {
        eu3D[i] = new Euclidean3DGraph(filename_prefix + std::to_string(i) + ".tsv");
        threads[i] = std::thread(
                [](Euclidean3DGraph *eu3dGraph) { eu3dGraph->comebacksUntilMaxStep(maxIterations, highestMaxSteps); },
                eu3D[i]);
    }

    eu3D[amountOfThreads - 1] = new Euclidean3DGraph(filename_prefix + std::to_string(amountOfThreads - 1) + ".tsv");
    eu3D[amountOfThreads - 1]->comebacksUntilMaxStep(maxIterations, highestMaxSteps);

    for (auto &i : threads) {
        i.join();
    }
}

int main() {
    std::cout << "Welcome to fractals! You can calc the local spectral dimension of fractals with this tool."
              << std::endl;
    std::cout << "Which fractal do you want to calculate? Choose between:" << std::endl;
    std::cout << "exit (0)" << std::endl;
    std::cout << "2D-Euclidean lattice (1)" << std::endl;
    std::cout << "3D-Euclidean lattice (2)" << std::endl;
    int input;
    std::cin >> input;
    switch (input) {
        case 0:
            return 0;
        case 1:
            runEuclidean2D_parallel();
            break;
        case 2:
            runEuclidean3D_parallel();
            break;
        default:
            main();
    }
}