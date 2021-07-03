//
// Created by chris on 14.06.21.
//
#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <cassert>
//#include <opencv2/opencv.hpp>

// a quadratic 2-level ising-lattice
class SpinLattice2level {
public:
    /**
     * initialize an ising-lattice with sights² spins and no external magnetic field
     * @param sights length of the quadratic lattice
     */
    explicit SpinLattice2level(unsigned int sights);

    /**
     * initialize an ising-lattice with sights² spins and extern magnetic field h
     * @param sights length of the quadratic lattice
     * @param h strength of extern magnetic field
     */
    explicit SpinLattice2level(unsigned int sights, int h);


    // prints a matrix-scheme to the console
    void printSpins();

    // Reinitialize all spins with random values
    void initRandom();


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

    [[nodiscard]] int calcEnergy(unsigned int x, unsigned int y, int newSpinVal) const;

    [[nodiscard]] int calcMagnetization() const;

    [[nodiscard]] float calcSusceptibility() const;

    [[nodiscard]] int calcHeatCapacity() const;


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
    int h;
};

void metropolisSweep(SpinLattice2level &spinLattice, float temp);
void metropolisSweep(SpinLattice2level &spinLattice, float temp, unsigned int iterations);

void heatBathSweep(SpinLattice2level &spinLattice, float temp);
void heatBathSweepRandChoice(SpinLattice2level &spinLattice, float temp);
