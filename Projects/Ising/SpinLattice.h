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
class SpinLattice {
public:
    explicit SpinLattice(unsigned int sights);


    // prints a matrix-scheme to the console
    void printSpins();


    int operator()(unsigned int x, unsigned int y) const {
        assert(x < sights && y < sights);
        return spins[y + x * sights];
    }

    int &operator()(unsigned int x, unsigned int y) {
        assert(x < sights && y < sights);
        return spins[y + x * sights];
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


private:
    unsigned int sights;
    std::vector<int> spins;
};

void metropolisSweep(SpinLattice &spinLattice, float temp);
void metropolisSweep(SpinLattice &spinLattice, float temp,unsigned int iterations);

void heatBathSweep(SpinLattice &spinLattice, float temp);


/**
 * This is the runtime GUI that let's you watch what happens during the
 * optimization procedure
 */
class RuntimeGUI {
public:
    /**
     * Constructor
     */
    RuntimeGUI(int rows, int cols) : waitTime(25), gui(rows, cols, CV_8UC3) {
        // Open the window
        cv::namedWindow("GUI", 1);
    }

    /**
     * Destructor
     */
    virtual ~RuntimeGUI() {
        cv::destroyWindow("GUI");
    }

    /**
     * Paint the gui
     */
    void notify(const SpinLattice &instance);

    /**
     * The time the GUI pauses after each update. Set to 0 to let
     * it wait for a keypress
     */
    int waitTime;

private:
    /**
     * The GUI matrix
     */
    cv::Mat gui;
};
