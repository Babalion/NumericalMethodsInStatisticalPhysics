//
// Created by chris on 06.05.21.
//

#ifndef NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CONFIGURATION_H
#define NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CONFIGURATION_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "Cell.h"
#include "../../lib/matplotlibcpp.h"
#include "RNG_MT19937.h"


class Configuration {
public:
    //field width should be bigger or equal than 2
    Configuration(unsigned int fieldWidth, unsigned int criticalSlope);
    //creates a Configuration with a given cells-vector
    Configuration(std::vector<Cell> &cells_, unsigned int criticalSlope);

    void initRandom();
    void updateSlopes();

    void runTime();

    void doConservativePerturbation();

    void doNonConservativePerturbation();

    void plot();
    void printFig();

    [[nodiscard]] const std::vector<Cell> &getCells() const;
    [[nodiscard]] unsigned int getFieldWidth() const;

private:
    unsigned int fieldWidth;
    unsigned int criticalSlope;
    std::vector<Cell> cells;
    RNG_MT19937 rng;
    std::vector<int> cellHeightToArray();
};


#endif //NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CONFIGURATION_H
