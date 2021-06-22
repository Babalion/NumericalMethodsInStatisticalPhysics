//
// Created by chris on 14.06.21.
//
#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <cassert>
#include <opencv2/opencv.hpp>

// a quadratic 2-level ising-lattice
class SpinLattice2level {
public:
    explicit SpinLattice2level(unsigned int sights);


    // prints a matrix-scheme to the console
    void printSpins();


    int operator()(unsigned int x, unsigned int y) const {
        assert(x < sights && y < sights);
        return spins[x + y * sights];
    }

    int &operator()(unsigned int x, unsigned int y) {
        assert(x < sights && y < sights);
        return spins[x + y * sights];
    }

    [[nodiscard]] int calcEnergy() const;

    [[nodiscard]] int calcEnergy(unsigned int x, unsigned int y) const;

    [[nodiscard]] int calcEnergy(unsigned int x, unsigned int y, int newSpinVal);

    [[nodiscard]] unsigned int getSights() const {
        return sights;
    }

    [[nodiscard]] const std::vector<int> &getSpins() const {
        return spins;
    }

    int J;

    unsigned int performedSweeps;

private:
    unsigned int sights;
    std::vector<int> spins;
};

void metropolisSweep(SpinLattice2level &spinLattice, float temp);
void metropolisSweep(SpinLattice2level &spinLattice, float temp, unsigned int iterations);

void heatBathSweep(SpinLattice2level &spinLattice, float temp);
void heatBathSweepRandChoice(SpinLattice2level &spinLattice, float temp);
