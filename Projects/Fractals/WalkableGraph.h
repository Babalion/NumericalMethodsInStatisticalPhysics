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
    std::vector<int> startLoc;
    unsigned int amountOfNeighbours;
    RNG_MT19937 rng;


protected:
    std::vector<int> loc;

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

    [[maybe_unused]] [[nodiscard]] unsigned int getAmountOfNeighbours() const {
        return amountOfNeighbours;
    }

    //let the walker walk until come back
    //returns number of steps until comeback, will break when reached maxSteps
    //returns -1, if never and sets location to start
    int walkUntilComeBack(unsigned int maxSteps) {
        for (unsigned int i = 0; i < maxSteps; ++i) {
            hopToNeighbour(rng.getRandom());
            if (loc == startLoc) {//we're back!
                return static_cast<int>(i);
            }
        }
        resetCoordinates();
        return -1;//walker escaped and has not come back
    }

    //generates a large .tsv file with number of steps until return for each random-walk
    void stepsToReturn(unsigned int maxIterations, unsigned int maxSteps) {
        file.open(filename);
        file << "steps-until-comeback\n";
        for (unsigned int i = 0; i < maxIterations; i++) {
            if ((i + 1) % 1000 == 0) {
                std::cout << "iteration: (" << i + 1 << "/" << maxIterations << ")" << std::endl;
            }
            int steps = walkUntilComeBack(maxSteps);
            if (true) {//steps is smaller 0 if walker escaped
                file << steps << std::endl;
            }
        }
        file.close();
    }
};
