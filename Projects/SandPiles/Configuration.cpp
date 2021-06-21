//
// Created by chris on 06.05.21.
//

#include "Configuration.h"


Configuration::Configuration(unsigned int fieldWidth, unsigned int criticalSlope) :
        fieldWidth(fieldWidth), criticalSlope(criticalSlope), time(0) {
    cells = std::vector<Cell>(fieldWidth * fieldWidth);
    initRandom();
}


Configuration::Configuration(std::vector<Cell> &cells_, unsigned int criticalSlope) :
        criticalSlope(criticalSlope), time(0) {
    fieldWidth = sqrt(cells_.size());
    cells = cells_;
    updateSlopes();
}

/**
 * initializes cells and slope-cells
 * it calculates random numbers between 0 and 20 for cells
 * slope-cells are derived from cells
 */
void Configuration::initRandom() {
    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution u(0, 20);
    //init cells with random height
    for (auto &cell : cells) {
        cell.setHeight(u(mt));
    }
    updateSlopes();
}

/**
 * Prints out heights of each cell to the console
 */
void Configuration::printToConsole() {
    // output cells
    std::cout << "Cells:" << std::endl;
    for (size_t i = 0; i < cells.size(); ++i) {
        std::cout << cells[i].getHeight();
        if ((i + 1) % fieldWidth == 0 && i > 0) {
            std::cout << std::endl;
        } else {
            std::cout << "\t";
        }
    }
    std::cout << "Cells slope:" << std::endl;
    for (size_t i = 0; i < cells.size(); ++i) {
        std::cout << cells[i].getSlopeToNeighbours();
        if ((i + 1) % fieldWidth == 0 && i > 0) {
            std::cout << std::endl;
        } else {
            std::cout << "\t";
        }
    }
}

const std::vector<Cell> &Configuration::getCells() const {
    return cells;
}

unsigned int Configuration::getFieldWidth() const {
    return fieldWidth;
}

/**
 * Calculates the slopeToNeighbours based on the actual cell height and
 */
void Configuration::updateSlopes() {
    for (size_t x = 0; x < fieldWidth; ++x) {
        for (size_t y = 0; y < fieldWidth; ++y) {
            int maxSlope = -1 * static_cast<int>(criticalSlope); //this is not a problem.
            // Maybe maxSlope is smaller but then also nothing happens

            const unsigned int i = y * fieldWidth + x;

            if (x > 0) {// not at left boundary
                maxSlope = std::max(maxSlope,
                                    static_cast<int>(cells[i].getHeight()) - (int) cells[i - 1].getHeight());
            }
            if (y > 0) {// not at top boundary
                maxSlope = std::max(maxSlope,
                                    static_cast<int>(cells[i].getHeight()) - (int) cells[i - fieldWidth].getHeight());
            }
            if (x < fieldWidth - 1) {// not at right boundary
                maxSlope = std::max(maxSlope,
                                    static_cast<int>(cells[i].getHeight()) - (int) cells[i + 1].getHeight());
            }
            if (y < fieldWidth - 1) {// not at bottom boundary
                maxSlope = std::max(maxSlope,
                                    static_cast<int>(cells[i].getHeight()) - (int) cells[i + fieldWidth].getHeight());
            }
            cells[i].setSlopeToNeighbours(maxSlope);
        }
    }
}

/**
 * Performs a timeStep. Therefore updates each cell and increases neighbours if critical slope of cell is reached.
 * Also updates slopes.
 * Doesn't add sand!
 */
void Configuration::runTime() {
    //reduce critical cells by 4 and increase neighbour cells by 1
    for (size_t i = 0; i < cells.size(); ++i) {
        if (cells[i].getSlopeToNeighbours() >=
            static_cast<int>(criticalSlope)) {// critical slope is positive, slopeToNeighbour needn't because neighbours could be higher

            cells[i].setHeight(cells[i].getHeight() - 4);

            //increase neighbour cells by 1
            if ((i % fieldWidth) != 0) {//not at left boundary
                cells[i - 1].incHeight();
            }
            if ((i % fieldWidth) != (fieldWidth - 1)) {//not at right boundary
                cells[i + 1].incHeight();
            }
            if (i >= fieldWidth) {//not at top boundary
                cells[i - fieldWidth].incHeight();
            }
            if (i < (fieldWidth * (fieldWidth - 1))) {//not at bottom boundary
                cells[i + fieldWidth].incHeight();
            }
        }
    }
    // update the slopes now
    updateSlopes();
    time++;
}

/**
 * adds a sand-corn to the middle of the field
 */
void Configuration::addSand() {
    //we add a corn of sand in the middle of the cells field
    cells[(fieldWidth - 1) * (fieldWidth - 1) / 2 + fieldWidth / 2].incHeight();
    updateSlopes();
}

/**
 * @return sum over all cell-heights
 */
unsigned int Configuration::amountOfSand() const {
    unsigned int amountOfSand = 0;
    for (auto &cell : cells) {
        amountOfSand += cell.getHeight();
    }
    return amountOfSand;
}

/**
 * @return the maximum height of all cells
 */
unsigned int Configuration::maxHeight() const {
    unsigned int maxHeight = cells[0].getHeight();

    for (auto &cell:cells) {
        maxHeight = std::max(maxHeight, cell.getHeight());
    }
    return maxHeight;
}

/**
 * @return the minimum height of all cells
 */
unsigned int Configuration::minHeight() const {
    unsigned int minHeight = cells[0].getHeight();

    for (auto &cell:cells) {
        minHeight = std::min(minHeight, cell.getHeight());
    }
    return minHeight;
}

/**
 * sums up all slopesToNeighbours from all cells which are greater than 0
 * thus all slopes from mountains downwards, not from valleys upwards
 * @return criticality greater or equal 0
 */
unsigned int Configuration::getCriticality() const {
    unsigned int criticality = 0;
    for (auto &cell:cells) {
        criticality += std::max(0, cell.getSlopeToNeighbours());
    }
    return criticality;
}

unsigned int Configuration::getTime() const {
    return time;
}

