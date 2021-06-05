//
// Created by chris on 05.06.21.
//
//Source: https://www.geeksforgeeks.org/travelling-salesman-problem-implementation-using-backtracking/
#pragma once

#include <vector>
#include <bits/stdc++.h>

#include "Graph.h"


// Function to find the minimum weight Hamiltonian Cycle
void tsp(graphMatrix graph, std::vector<bool> &v, int currPos,
         int n, int count, int cost, int &ans) {

    // If last node is reached and it has a link
    // to the starting node i.e the source then
    // keep the minimum value out of the total cost
    // of traversal and "ans"
    // Finally return to check for more possible values
    if (count == n && graph[currPos][0]) {
        ans = std::min(ans, cost + graph[currPos][0]);
        return;
    }

    // BACKTRACKING STEP
    // Loop to traverse the adjacency list
    // of currPos node and increasing the count
    // by 1 and cost by graph[currPos][i] value
    for (int i = 0; i < n; i++) {
        if (!v[i] && graph[currPos][i]) {

            // Mark as visited
            v[i] = true;
            tsp(graph, v, i, n, count + 1,
                cost + graph[currPos][i], ans);

            // Mark ith node as unvisited
            v[i] = false;
        }
    }
};

// Driver code
int backTracking(const graphMatrix& graph) {
    int n = graph.size();


    // Boolean array to check if a node
    // has been visited or not
    std::vector<bool> v(n);
    for (int i = 0; i < n; i++)
        v[i] = false;

    // Mark 0th node as visited
    v[0] = true;
    int ans = INT_MAX;

    // Find the minimum weight Hamiltonian Cycle
    tsp(graph, v, 0, n, 1, 0, ans);

    // ans is the minimum weight Hamiltonian Cycle
    return ans;
}
