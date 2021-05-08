//
// Created by chris on 06.05.21.
//

#include "Configuration.h"

Configuration::Configuration(unsigned int fieldWidth, unsigned int criticalSlope) :
        criticalSlope(criticalSlope), fieldWidth(fieldWidth), rng(RNG_MT19937(0, 50)) {
    cells = std::vector<Cell>(fieldWidth * fieldWidth);
    hSlopes = std::vector<Slope_Cell>(fieldWidth * (fieldWidth - 1));
    vSlopes = std::vector<Slope_Cell>(fieldWidth * (fieldWidth - 1));
    locSlopes = std::vector<Slope_Cell>((fieldWidth - 1) * (fieldWidth - 1));
}

Configuration::Configuration(std::vector<Cell> &cells_, unsigned int criticalSlope) :
        criticalSlope(criticalSlope), rng(RNG_MT19937(0, 50)) {
    fieldWidth = sqrt(cells_.size());
    cells = cells_;
    hSlopes = std::vector<Slope_Cell>(fieldWidth * (fieldWidth - 1));
    vSlopes = std::vector<Slope_Cell>(fieldWidth * (fieldWidth - 1));
    locSlopes = std::vector<Slope_Cell>((fieldWidth - 1) * (fieldWidth - 1));
    updateLocSlopes();
}

//initializes cells and slope-cells
// it calculates random numbers between 0 and 8 for cells
// slope-cells are derived from cells
void Configuration::initRandom() {
    //init cells with random height
    for (auto &cell : cells) {
        cell.setHeight(rng.getRandom());
    }
    updateLocSlopes();
}

void Configuration::updateVSlopes() {
    // calculate now vertical slopes
    for (int row = 0; row < fieldWidth; ++row) {
        for (int col = 0; col < fieldWidth - 1; ++col) {
            int v_index = row * (fieldWidth - 1) + col;
            int setSlope = cells[row * fieldWidth + col + 1].getHeight() - cells[row * fieldWidth + col].getHeight();
            vSlopes[v_index].setSlope(setSlope);
        }
    }
}

void Configuration::updateHSlopes() {
    // calculate now horizontal slopes
    for (int row = 0; row < fieldWidth - 1; ++row) {
        for (int col = 0; col < fieldWidth; ++col) {
            int h_index = row * fieldWidth + col;
            int setSlope = cells[(row+1) * fieldWidth + col].getHeight() - cells[row * fieldWidth + col].getHeight();
            hSlopes[h_index].setSlope(setSlope);
        }
    }
}

void Configuration::updateLocSlopes() {
    updateVSlopes();
    updateHSlopes();
    for (int row = 0; row < fieldWidth - 1; ++row) {
        for (int col = 0; col < fieldWidth-1; ++col) {
            int loc_index = row * (fieldWidth-1) + col;
            int setLocalSlope = hSlopes[loc_index+(fieldWidth-1)].getSlope() - hSlopes[loc_index].getSlope()
                    +vSlopes[loc_index].getSlope()-vSlopes[loc_index-1].getSlope();
            locSlopes[loc_index].setSlope(setLocalSlope);
        }
    }
}

#include <iostream>

void Configuration::plot() {
    // output cells
    std::cout << "Cells:" << std::endl;
    for (int row = 0; row < fieldWidth; ++row) {
        for (int col = 0; col < fieldWidth - 1; ++col) {
            std::cout << cells[row * fieldWidth + col].getHeight() << " ";
        }
        std::cout << cells[row * fieldWidth + fieldWidth - 1].getHeight() << std::endl;
    }
    //output vSlopes
    std::cout << "vSlopes:" << std::endl;
    for (int i = 0; i < vSlopes.size(); ++i) {
        std::cout << vSlopes[i].getSlope();
        if ((i + 1) % (fieldWidth - 1) == 0 && i > 0) {
            std::cout << std::endl;
        } else {
            std::cout << "\t";
        }
    }
    //output hSlopes
    std::cout << "hSlopes:" << std::endl;
    for (int i = 0; i < hSlopes.size(); ++i) {
        std::cout << hSlopes[i].getSlope();
        if ((i + 1) % fieldWidth == 0 && i > 0) {
            std::cout << std::endl;
        } else {
            std::cout << "\t";
        }
    }
    //output locSlopes
    std::cout << "locSlopes:" << std::endl;
    for (int i = 0; i < locSlopes.size(); ++i) {
        std::cout << locSlopes[i].getSlope();
        if ((i + 1) % (fieldWidth-1) == 0 && i > 0) {
            std::cout << std::endl;
        } else {
            std::cout << "\t";
        }
    }

}

const std::vector<Cell> &Configuration::getCells() const {
    return cells;
}

const std::vector<Slope_Cell> &Configuration::getVSlopes() const {
    return vSlopes;
}

const std::vector<Slope_Cell> &Configuration::getHSlopes() const {
    return hSlopes;
}

const std::vector<Slope_Cell> &Configuration::getLocSlopes() const {
    return locSlopes;
}

unsigned int Configuration::getFieldWidth() const {
    return fieldWidth;
}


