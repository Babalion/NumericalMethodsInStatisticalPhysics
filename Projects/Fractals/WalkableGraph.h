//
// Created by chris on 15.05.21.
//
#pragma once

#include <utility>
#include <vector>
#include "RNG_MT19937.h"
#include <fstream>
#include <iostream>


class WalkableGraph {
private:
    std::vector<int> loc;
    std::vector<int> startLoc;
    unsigned int amountOfNeighbours;
    RNG_MT19937 rng;


protected:
    void setCoordinates(const std::vector<int> &coords) {
        WalkableGraph::loc = coords;
    }

    std::ofstream file;
    std::string filename;
public:
    void resetCoordinates() {
        loc=startLoc;
    }

    //Creates a abstract graph with a given amountOfNeighbours and startLocation
    WalkableGraph(unsigned int amountOfNeighbours_, const std::vector<int> &startLoc, std::string filename) :
            startLoc(startLoc),amountOfNeighbours(amountOfNeighbours_),rng(0, amountOfNeighbours_ - 1),filename(std::move(filename)) {
        loc = startLoc;//copy the initial position
    }

    //hops to the neighbour with index n
    // 0 <= n < amountOfNeighbours
    virtual void hopToNeighbour(unsigned int n) = 0;

    [[nodiscard]] const std::vector<int> &getCoordinates() const {
        return loc;
    }

    [[nodiscard]] unsigned int getAmountOfNeighbours() const {
        return amountOfNeighbours;
    }

    //let the walker walk
    //returns true, if walker gets back to startPoint within maxSteps
    //returns false, if not
    bool walk(unsigned int maxSteps){
        for (unsigned int i = 0; i < maxSteps; ++i) {
            hopToNeighbour(rng.getRandom());
            if (loc == startLoc) {
                return true;
            }
        }
        return false;
    }



    void generateDimensionData(int maxIterations, int highestMaxSteps,int logInterval){
        file.open(filename);
        file << "maxSteps\tnumber-of-iterations\tcome-back-rate\n";
        for (int i = 100; i < highestMaxSteps*10; i *= 10) {
            int cameBack = 0;

            for (int j = 0; j < maxIterations; ++j) {
                if ((j+1) % logInterval == 0) {
                    std::cout << "Euclidean2D maxSteps: 1E" << log10(i) << "\titeration: " << j+1 << std::endl;
                    file << i << "\t" <<logInterval<<"\t"<< cameBack*1.0/logInterval << std::endl;
                    cameBack=0;
                }
                if (walk(i)) {
                    cameBack++;
                }
                resetCoordinates();
            }
        }
        file.close();
    }
};
