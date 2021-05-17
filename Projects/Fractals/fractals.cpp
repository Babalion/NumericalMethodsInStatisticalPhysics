//
// Created by chris on 15.05.21.
//
#include <iostream>
#include <thread>
#include <string>
#include "Euclidean2DGraph.h"
#include "Euclidean3DGraph.h"
#include <cmath>


template<typename T>
void runCalcSpectralDimension(const std::string &filePrefix, const unsigned int &amountOfWalks, const unsigned int &maxSteps) {
    static const unsigned int hardwareCon = std::thread::hardware_concurrency();
    static const unsigned int supportedThreads = hardwareCon == 0 ? 2 : hardwareCon;
    static const unsigned int amountOfThreads = supportedThreads;// we use all cores!
    const unsigned int iterationsPerThread = amountOfWalks / amountOfThreads; //ignore the last modulo steps... steps are high
    std::cout<<amountOfThreads<<" threads will be used for calculation."<<std::endl;

    std::vector<std::thread> threads(amountOfThreads - 1);
    std::vector<T *> arrWalkableGraph(amountOfThreads);
    for (unsigned int i = 0; i < threads.size(); i++) {
        arrWalkableGraph[i] = new T(filePrefix + std::to_string(i) + ".tsv");
        threads[i] = std::thread(
                [=](T *walkableGraph) { walkableGraph->stepsToReturn(iterationsPerThread, maxSteps); },
                arrWalkableGraph[i]);
    }

    arrWalkableGraph[amountOfThreads - 1] = new T(filePrefix + std::to_string(amountOfThreads - 1) + ".tsv");
    arrWalkableGraph[amountOfThreads - 1]->stepsToReturn(iterationsPerThread, maxSteps);

    for (auto &i : threads) {
        i.join();
    }
}

void inputWalkParameters(unsigned int &amountOfWalks,unsigned int &maxSteps){
    std::cout<<"How many walkers should be observed at all? (Input power to 10, so 6 for 1E6 walkers):"<<std::endl;
    std::cin>>amountOfWalks;
    amountOfWalks=static_cast<unsigned int>(pow(10,amountOfWalks));
    std::cout<<"How many steps should the walkers be allowed to travel?(Input power to 10, so 6 for 1E6 steps):"<<std::endl;
    std::cin>>maxSteps;
    maxSteps=static_cast<unsigned int>(pow(10,maxSteps));
}

int main() {
    std::cout << "Welcome to fractals! You can calc the local spectral dimension of fractals with this tool."
              << std::endl;
    std::cout << "Which fractal do you want to calculate? Choose between:" << std::endl;
    std::cout << "exit (0)" << std::endl;
    std::cout << "2D-Euclidean lattice (1)" << std::endl;
    std::cout << "3D-Euclidean lattice (2)" << std::endl;
    int input;
    unsigned int amountOfWalks=0;
    unsigned int maxSteps=0;
    std::cin >> input;
    switch (input) {
        case 0:
            return 0;
        case 1:
            inputWalkParameters(amountOfWalks,maxSteps);
            runCalcSpectralDimension<Euclidean2DGraph>("euclidean2D_",amountOfWalks,maxSteps);
            break;
        case 2:
            inputWalkParameters(amountOfWalks,maxSteps);
            runCalcSpectralDimension<Euclidean3DGraph>("euclidean3D_",amountOfWalks,maxSteps);
            break;
        default:
            main();
    }
}