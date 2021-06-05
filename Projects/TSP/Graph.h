//
// Created by chris on 05.06.21.
//
#pragma once

#include <vector>
#include <iostream>

typedef std::vector<std::vector<int>> graphMatrix;

// prints a graphMatrix to the console
void plotGraph(const graphMatrix &graph) {
    for (unsigned long i = 0; i < graph.size(); ++i) {
        for (unsigned long j = 0; j < graph[0].size(); ++j) {
            std::cout << graph[i][j];
            if (j == graph[0].size() - 1) {
                std::cout << "\n";
            } else {
                std::cout << "\t";
            }
        }
    }
}

// prints a vector to the console
void plotVector(const std::vector<int> &vec) {
    for (int i : vec) {
        std::cout << i << std::endl;
    }
}

// input: n number of rows
// returns: a adjacent matrix of New York (a simple 2d square lattice) with dim n² x n²
graphMatrix generateNewYork(int n) {
    //MATLAB-Code from https://stackoverflow.com/questions/3277541/construct-adjacency-matrix-in-matlab
    /*
    n =2;
    diagVec1 = repmat([ones(n-1, 1); 0], n, 1)  % Make the first diagonal vector
                                                %   (for horizontal connections)
    diagVec1 = diagVec1(1:end-1)                % Remove the last value
    diagVec2 = ones(n*(n-1), 1)                 % Make the second diagonal vector
                                                %   (for vertical connections)
    adj = diag(diagVec1, 1)+diag(diagVec2, n)   % Add the diagonals to a zero matrix
    adj = adj+adj.'                             % Add the matrix to a transposed copy of
                                                %   itself to make it symmetric
     */
    std::vector<int> diagVec1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            diagVec1.push_back(1);
        }
        diagVec1.push_back(0);
    }
    diagVec1.pop_back();
    //plotVector(diagVec1);

    std::vector<int> diagVec2(n * (n - 1), 1);
    //std::cout << "diagvec2:\n";
    //plotVector(diagVec2);

    graphMatrix adjMatrix;
    for (int i = 0; i < n * n; ++i) {
        std::vector rowJ(n * n, 0);
        adjMatrix.push_back(rowJ);
    }

    for (unsigned long i = 0; i < diagVec1.size(); ++i) {
        adjMatrix[i][i + 1] = diagVec1[i];
    }
    for (unsigned long i = 0; i < diagVec2.size(); ++i) {
        adjMatrix[i][i + n] = diagVec2[i];
    }

    for (unsigned long i = 0; i < adjMatrix.size(); ++i) {
        for (unsigned long j = 0; j < adjMatrix[0].size(); ++j) {
            if (j > i) {
                adjMatrix[j][i] = adjMatrix[i][j];
            }
        }
    }

    return adjMatrix;
}