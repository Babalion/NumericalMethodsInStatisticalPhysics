//
// Created by chris on 15.06.21.
//
#pragma once

#include <vector>
#include <cassert>
#include <opencv2/opencv.hpp>
#include <random>

class SpinLatticeTheta {
public:
    explicit SpinLatticeTheta(unsigned int sights);


    // prints a matrix-scheme to the console
    void printSpins();


    float operator()(unsigned int x, unsigned int y) const {
        assert(x < sights && y < sights);
        return spins[y + x * sights];
    }

    float &operator()(unsigned int x, unsigned int y) {
        assert(x < sights && y < sights);
        return spins[y + x * sights];
    }

    [[nodiscard]] float calcEnergy(unsigned int x, unsigned int y, float newSpinTheta) const;

    [[nodiscard]] float calcEnergy(unsigned int x, unsigned int y) const;

    /// Calculates the energy of the whole spin-system with open boundaries
    [[nodiscard]] float calcEnergy() const;

    [[nodiscard]] unsigned int getSights() const {
        return sights;
    }

    [[nodiscard]] const std::vector<float> &getSpins() {
        return spins;
    }

    int J;


private:
    unsigned int sights;
    std::vector<float> spins;
};

void metropolisSweep(SpinLatticeTheta &spinLattice, float temp);

void metropolisSweep(SpinLatticeTheta &spinLattice, float temp, unsigned int iterations);


