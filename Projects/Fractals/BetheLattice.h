//
// Created by chris on 17.05.21.
//
#pragma once

#include "WalkableGraph.h"

// a bethe-lattice is a tree like fractal structure like described here: https://en.wikipedia.org/wiki/Bethe_lattice
// as coordinates this class uses only one variable which represents the depth of the node relative to the start-node
// there are always two nodes which lay one deeper and always one node which lays one higher.
// The start-node only has 3 deeper laying neighbours
class BetheLattice : public WalkableGraph {
public:
    //generates a startNode at loc 0
    explicit BetheLattice(std::string filename);

    void hopToNeighbour(unsigned int n) override;

};

BetheLattice::BetheLattice(std::string filename)
        : WalkableGraph(3, std::vector<int>{0}, std::move(filename)) {
}

void BetheLattice::hopToNeighbour(unsigned int n) {
    if (loc[0] == 0) {//we are at start-node
        loc[0]++;
    } else {
        if ((n == 0) || (n == 1)) {
            //probability 2/3 to descend in lattice --> increase depth
            loc[0]++;
        } else if (n == 2) {
            //probability 1/3 to ascend in lattice --> decrease depth
            loc[0]--;
        } else {
            std::cerr << "Index " << n << " of this neighbour is too high! (BetheLattice:  0<=n<3)" << std::endl;
            exit(10);
        }
    }
}

