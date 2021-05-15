//
// Created by chris on 15.05.21.
//
#pragma once

#include <utility>
#include <vector>
#include "RNG_MT19937.h"


class WalkableGraph {
private:
    std::vector<int> loc;
    std::vector<int> startLoc;
    unsigned int amountOfNeighbours;
    RNG_MT19937 rng;

public:
    void setCoordinates(const std::vector<int> &coords) {
        WalkableGraph::loc = coords;
    }

    //Creates a abstract graph with a given amountOfNeighbours and startLocation
    WalkableGraph(unsigned int amountOfNeighbours, const std::vector<int>& startLoc) :
            amountOfNeighbours(amountOfNeighbours), startLoc(startLoc), rng(0, amountOfNeighbours - 1) {
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
        for (int i = 0; i < maxSteps; ++i) {
            hopToNeighbour(rng.getRandom());
            if (loc == startLoc) {
                return true;
            }
        }
        return false;
    }
};
