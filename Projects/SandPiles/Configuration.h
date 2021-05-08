//
// Created by chris on 06.05.21.
//

#ifndef NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CONFIGURATION_H
#define NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CONFIGURATION_H

#include <vector>
#include "Cell.h"
#include "Slope_Cell.h"
#include "../../lib/matplotlibcpp.h"
#include "RNG_MT19937.h"


class Configuration {
public:
    //field width should be bigger or equal than 2
    Configuration(unsigned int fieldWidth, unsigned int criticalSlope);
    //creates a Configuration with a given cells-vector
    Configuration(std::vector<Cell> &cells_, unsigned int criticalSlope);

    void initRandom();

    void runTime();

    void doConservativePerturbation();

    void doNonConservativePerturbation();

    void plot();

    [[nodiscard]] const std::vector<Cell> &getCells() const;

    [[nodiscard]]const std::vector<Slope_Cell> &getVSlopes() const;

    [[nodiscard]]const std::vector<Slope_Cell> &getHSlopes() const;

    [[nodiscard]]const std::vector<Slope_Cell> &getLocSlopes() const;

private:
    unsigned int fieldWidth;
public:
    unsigned int getFieldWidth() const;

private:
    unsigned int criticalSlope;
    std::vector<Cell> cells;

    std::vector<Slope_Cell> vSlopes;

    void updateVSlopes();

    std::vector<Slope_Cell> hSlopes;

    void updateHSlopes();

    std::vector<Slope_Cell> locSlopes;

    void updateLocSlopes();

    RNG_MT19937 rng;
};


#endif //NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CONFIGURATION_H
