//
// Created by chris on 15.05.21.
//

#include "Euclidean2DGraph.h"

Euclidean2DGraph::Euclidean2DGraph()
        : WalkableGraph(4, std::vector<int>{0, 0}) {

}

void Euclidean2DGraph::hopToNeighbour(unsigned int n) {
    switch (n) {
        case 0:
            setCoordinates(std::vector<int>{getCoordinates()[0], getCoordinates()[1] + 1});
            break;
        case 1:
            setCoordinates(std::vector<int>{getCoordinates()[0] + 1, getCoordinates()[1]});
            break;
        case 2:
            setCoordinates(std::vector<int>{getCoordinates()[0] - 1, getCoordinates()[1]});
            break;
        case 3:
            setCoordinates(std::vector<int>{getCoordinates()[0], getCoordinates()[1] - 1});
            break;
        default:
            std::cerr << "Index " << n << " of this neighbour is too high! (Euclidean2D:  0<=n<4)" << std::endl;
            exit(10);
    }
}


