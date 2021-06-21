//
// Created by chris on 06.05.21.
//

#include "Configuration.h"


Configuration::Configuration(unsigned int fieldWidth, unsigned int criticalSlope) :
        fieldWidth(fieldWidth), criticalSlope(criticalSlope), rng(RNG_MT19937(0, 50)) {
    cells = std::vector<Cell>(fieldWidth * fieldWidth);
    initRandom();
}

Configuration::Configuration(std::vector<Cell> &cells_, unsigned int criticalSlope) :
        criticalSlope(criticalSlope), rng(RNG_MT19937(0, 50)) {
    fieldWidth = sqrt(cells_.size());
    cells = cells_;
    updateSlopes();
}

//initializes cells and slope-cells
// it calculates random numbers between 0 and 8 for cells
// slope-cells are derived from cells
void Configuration::initRandom() {
    //init cells with random height
    for (auto &cell : cells) {
        cell.setHeight(rng.getRandom());
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

/// calculates the cell-slope based on the cell height of each neighbours
void Configuration::updateSlopes() {
    for (size_t i = 0; i < cells.size(); ++i) {
        //TODO refactoring

        int maxSlope = 0; //TODO the minimal slope is now 0 - Problem?
        if ((i % fieldWidth) == 0) {//left boundary
        } else {
            maxSlope = std::max(maxSlope,
                                static_cast<int>(cells[i].getHeight()) - (int) cells[i - 1].getHeight());
        }
        if ((i % fieldWidth) == (fieldWidth - 1)) {//right boundary
        } else {
            maxSlope = std::max(maxSlope,
                                static_cast<int>(cells[i].getHeight()) - (int) cells[i + 1].getHeight());
        }
        if (i < fieldWidth) {//top boundary
        } else {
            maxSlope = std::max(maxSlope,
                                static_cast<int>(cells[i].getHeight()) - (int) cells[i - fieldWidth].getHeight());
        }
        if (i >= (fieldWidth * (fieldWidth - 1))) {//bottom boundary
        } else {
            maxSlope = std::max(maxSlope,
                                static_cast<int>(cells[i].getHeight()) - (int) cells[i + fieldWidth].getHeight());
        }
        cells[i].setSlopeToNeighbours(maxSlope);
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
            criticalSlope) {// critical slope is positive, slopeToNeighbour needn't because neighbours could be higher

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
}

/**
 * adds a sand-corn to the middle of the field
 */
void Configuration::addSand() {
    //we add a corn of sand in the middle of the cells field
    cells[(fieldWidth - 1) * (fieldWidth - 1) / 2 + fieldWidth / 2].incHeight();
    updateSlopes();
}

