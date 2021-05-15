//
// Created by chris on 15.05.21.
//
#pragma once

#include "WalkableGraph.h"
#include <iostream>

class Euclidean2DGraph : public WalkableGraph {
public:
    //generates a startNode at loc 0,0
    Euclidean2DGraph();

    //Euclidean2DGraph(unsigned int amountOfNeighbours, std::vector<int> loc);
    void hopToNeighbour(unsigned int n) override;
};