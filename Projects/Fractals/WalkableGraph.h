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
        loc = startLoc;
    }

    //Creates a abstract graph with a given amountOfNeighbours and startLocation
    WalkableGraph(unsigned int amountOfNeighbours_, const std::vector<int> &startLoc, std::string filename) :
            startLoc(startLoc), amountOfNeighbours(amountOfNeighbours_), rng(0, amountOfNeighbours_ - 1),
            filename(std::move(filename)) {
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
    //returns number of steps until comeback, will break when reached maxSteps
    //returns -1, if never
    int walk(unsigned int maxSteps) {
        for (unsigned int i = 0; i < maxSteps; ++i) {
            hopToNeighbour(rng.getRandom());
            if (loc == startLoc) {//we're back!
                return static_cast<int>(i);
            }
        }
        return -1;//walker escaped and has not come back
    }

    //generates a large .tsv file with number of steps until return for each random-walk
    void comebacksUntilMaxStep(unsigned int maxIterations, unsigned int maxSteps) {
        file.open(filename);
        file << "steps-until-comeback\n";
        for (unsigned int i = 0; i < maxIterations; i++) {
            if((i+1)%1000==0){
                std::cout<<"iteration: ("<<i+1<<"/"<<maxIterations<<")"<<std::endl;
            }
            int steps=walk(maxSteps);
            if(steps>=0){//steps is smaller 0 if walker escaped
                file << steps << std::endl;
            }
            //TODO OPTIMIZE: we only have to reset coordinates, if we aren't at the beginning already...
            resetCoordinates();
        }
        file.close();
    }
};
