//
// Created by chris on 05.06.21.
//

// CPP program to implement traveling salesman
// problem using naive approach.
#include "minimizePermutations.h"
#include "BackTracking.h"
#include "Graph.h"
#include "gist.h"


// Driver Code
int main() {
    // smallest path: 80
    std::vector<std::vector<int>> fullyConnected = {{0,  10, 15, 20},
                                                    {10, 0,  35, 25},
                                                    {15, 35, 0,  30},
                                                    {20, 25, 30, 0}};

    std::vector<std::vector<int>> newYork = generateNewYork(4);

    std::cout << "fully connected: " << tspMinPermutations(fullyConnected) << std::endl;
    plotGraph(fullyConnected);
    std::cout << "\nnewYork: " << gists(newYork) << std::endl;
    plotGraph(newYork);
    return 0;
}