//
// Created by chris on 05.06.21.
//
// Source: https://www.geeksforgeeks.org/traveling-salesman-problem-tsp-implementation/
#pragma once

#include <bits/stdc++.h>
#include "Graph.h"

// implementation of traveling Salesman Problem
// The complexity is O(n!), but implementation is trivial and for small graphs it's quite fine
int tspMinPermutations(std::vector<std::vector<int>> graph) {
    static const unsigned long s = 0;
    // store all vertex apart from source vertex
    std::vector<int> vertex;
    for (unsigned long i = 0; i < graph.size(); i++) {
        if (i != s) {
            vertex.push_back(i);
        }
    }


    // store minimum weight Hamiltonian Cycle.
    int min_path = INT_MAX;
    do {

        // store current Path weight(cost)
        int currentPathWeight = 0;

        // compute current path weight
        unsigned long k = s;
        for (int i : vertex) {
            if (graph[k][i] == 0) {// if no connection exists
                currentPathWeight = INT_MAX;
                break;
            }
            currentPathWeight += graph[k][i];
            k = i;
        }
        if (currentPathWeight != INT_MAX) {
            currentPathWeight += graph[k][s];
            //std::cout << "Vertex is:\n";
            //plotVector(vertex);
            //std::cout << "Path-weight is: "<<currentPathWeight << "\n";
        }


        // update minimum
        min_path = std::min(min_path, currentPathWeight);

    } while (
            next_permutation(vertex.begin(), vertex.end()));

    return min_path;
}
