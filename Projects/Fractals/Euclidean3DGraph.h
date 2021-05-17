//
// Created by chris on 15.05.21.
//
#pragma once

class Euclidean3DGraph : public WalkableGraph {
public:
    //generates a startNode at loc 0,0,0
    explicit Euclidean3DGraph(std::string filename);

    //Euclidean2DGraph(unsigned int amountOfNeighbours, std::vector<int> loc);
    void hopToNeighbour(unsigned int n) override;

};

Euclidean3DGraph::Euclidean3DGraph(std::string filename) :
        WalkableGraph(6, std::vector<int>{0, 0, 0}, std::move(filename)) {
}

void Euclidean3DGraph::hopToNeighbour(unsigned int n) {
    switch (n) {
        case 0:
            loc[0]++;
            break;
        case 1:
            loc[1]++;
            break;
        case 2:
            loc[2]++;
            break;
        case 3:
            loc[0]--;
            break;
        case 4:
            loc[1]--;
            break;
        case 5:
            loc[2]--;
            break;
        default:
            std::cerr << "Index " << n << " of this neighbour is too high! (Euclidean3D:  0<=n<6)" << std::endl;
            exit(10);
    }
};
