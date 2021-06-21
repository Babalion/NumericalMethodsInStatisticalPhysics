//
// Created by chris on 06.05.21.
//
#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <vector>

#include "Cell.h"


class Configuration {
public:
    /**
     * field width should be bigger or equal than 2
     * initialization is random between 0 and 20
     */
    Configuration(unsigned int fieldWidth, unsigned int criticalSlope);

    /// creates a Configuration with a given cells-vector
    Configuration(std::vector<Cell> &cells_, unsigned int criticalSlope);

    void runTime();

    void addSand();

    void printToConsole();

    [[nodiscard]] unsigned int amountOfSand() const;

    [[nodiscard]] unsigned int maxHeight() const;

    [[nodiscard]] unsigned int minHeight() const;

    [[nodiscard]] unsigned int getCriticality() const;

    [[nodiscard]] const std::vector<Cell> &getCells() const;

    [[nodiscard]] unsigned int getFieldWidth() const;

    [[nodiscard]] unsigned int getTime() const;

private:
    void initRandom();

    void updateSlopes();

    unsigned int fieldWidth;
    unsigned int criticalSlope;
    std::vector<Cell> cells;
    unsigned int time;
};