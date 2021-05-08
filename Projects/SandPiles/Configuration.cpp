//
// Created by chris on 06.05.21.
//

#include "Configuration.h"

Configuration::Configuration(unsigned int fieldWidth, unsigned int criticalSlope) :
        criticalSlope(criticalSlope), fieldWidth(fieldWidth), rng(RNG_MT19937(0, 50)) {
    cells = std::vector<Cell>(fieldWidth * fieldWidth);
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

#include <iostream>

void Configuration::plot() {
    // output cells
    std::cout << "Cells:" << std::endl;
    for (int i = 0; i < cells.size(); ++i) {
        std::cout << cells[i].getHeight();
        if ((i + 1) % fieldWidth == 0 && i > 0) {
            std::cout << std::endl;
        } else {
            std::cout << "\t";
        }
    }
    std::cout << "Cells slope:" << std::endl;
    for (int i = 0; i < cells.size(); ++i) {
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

//calculates the cell-slope based on the cell height of each neighbours
void Configuration::updateSlopes() {
    for (int i = 0; i < cells.size(); ++i) {
        //TODO refactoring

        int maxSlope = 0; //TODO the minimal slope is now 0 - Problem?
        if ((i % fieldWidth) == 0) {//left boundary
        } else {
            maxSlope = std::max(maxSlope,
                                (int) cells[i].getHeight() - (int) cells[i - 1].getHeight());
        }
        if ((i % fieldWidth) == (fieldWidth - 1)) {//right boundary
        } else {
            maxSlope = std::max(maxSlope,
                                (int) cells[i].getHeight() - (int) cells[i + 1].getHeight());
        }
        if (i < fieldWidth) {//top boundary
        } else {
            maxSlope = std::max(maxSlope,
                                (int) cells[i].getHeight() - (int) cells[i - fieldWidth].getHeight());
        }
        if (i >= (fieldWidth * (fieldWidth - 1))) {//bottom boundary
        } else {
            maxSlope = std::max(maxSlope,
                                (int) cells[i].getHeight() - (int) cells[i + fieldWidth].getHeight());
        }
        cells[i].setSlopeToNeighbours(maxSlope);
    }

}

void Configuration::runTime() {
    //reduce critical cells by 4 and increase neighbour cells by 1
    for (int i = 0; i < cells.size(); ++i) {
        if (cells[i].getSlopeToNeighbours() >= criticalSlope) {
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


