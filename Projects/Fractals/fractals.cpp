//
// Created by chris on 15.05.21.
//
#include <iostream>
#include <fstream>
#include "Euclidean2DGraph.h"


void runEuclidean2D(){
    Euclidean2DGraph eu2D;

    std::fstream file;
    file.open("./Euclidean2D.tsv");
    file<<"number-of-walks\tcome-back-rate\n";


    for(int i=10;i<1E5;i*=10){
        int cameBack=0;
        for (int j = 0; j < 100; ++j) {
            if(eu2D.walk(i)){
                cameBack++;
            }
        }
        file<<i<<"\t"<<cameBack*1.0/i<<std::endl;
    }

    file.close();
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