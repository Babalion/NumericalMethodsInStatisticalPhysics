//
// Created by chris on 06.05.21.
//

#include "Configuration.h"

Configuration::Configuration(unsigned int fieldWidth, unsigned int criticalSlope) :
        criticalSlope(criticalSlope), fieldWidth(fieldWidth), rng(RNG_MT19937(0, 50)) {
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


namespace plt = matplotlibcpp;

void Configuration::printFig() {
    /*
    int n = 1000;
    std::vector<double> x, y, z;

    for(int i=0; i<n; i++) {
        x.push_back(i*i);
        y.push_back(sin(2*M_PI*i/360.0));
        z.push_back(log(i));

        if (i % 10 == 0) {
            // Clear previous plot
            plt::clf();
            // Plot line from given x and y data. Color is selected automatically.
            plt::plot(x, y);
            // Plot a line whose name will show up as "log(x)" in the legend.
            plt::named_plot("log(x)", x, z);

            // Set x-axis to interval [0,1000000]
            plt::xlim(0, n*n);

            // Add graph title
            plt::title("Sample figure");
            // Enable legend.
            plt::legend();
            // Display plot continuously
            plt::pause(0.01);
        }
    }
*/
    //TODO optimize with call by reference
    std::vector<std::vector<double>> x, y, z;
    for (int i = 0; i < fieldWidth; i++) {
        std::vector<double> x_row, y_row, z_row;
        for (int j = 0; j < fieldWidth; j++) {
            x_row.push_back(i);
            y_row.push_back(j);
            z_row.push_back(cells[i * fieldWidth + j].getHeight());
        }
        x.push_back(x_row);
        y.push_back(y_row);
        z.push_back(z_row);
    }
    //plt::scatter(x,y,z);
    plt::plot_surface(x, y, z);
    plt::show();
}

#include <string>

void Configuration::animate(unsigned int timeSteps, int offsetFileName) {
    std::vector<std::vector<double>> x, y, z;
    for (int t = 0; t < timeSteps; ++t) {
        for (int i = 0; i < fieldWidth; i++) {
            std::vector<double> x_row, y_row, z_row;
            for (int j = 0; j < fieldWidth; j++) {
                x_row.push_back(i);
                y_row.push_back(j);
                z_row.push_back(cells[i * fieldWidth + j].getHeight());
            }
            x.push_back(x_row);
            y.push_back(y_row);
            z.push_back(z_row);
        }
        //TODO do it like in example of matplotlib colorBar
        //plt::scatter(x,y,z);
        plt::plot_surface(x, y, z);
        plt::title("Sandpiles t=" + std::to_string(t+offsetFileName));
        //plt::pause(1);
        std::string s =
                "./sandPiles/sandPiles" + std::string(5 - std::to_string(t + offsetFileName).length(), '0') +
                std::to_string(t + offsetFileName) + ".png";
        std::cout << "saved " << s << std::endl;
        plt::save(s);
        plt::close();
        x.clear();
        y.clear();
        z.clear();
        runTime();
    }
}

void Configuration::addSand() {
    //we add a corn of sand in the middle of the cells field
    cells[(fieldWidth - 1) * (fieldWidth - 1) / 2+fieldWidth/2].incHeight();
    updateSlopes();
}

//starts at 0 to count
void Configuration::animate(unsigned int timeSteps) {
    animate(timeSteps,0);
}




