//
// Created by chris on 05.05.21.
//

#include "Configuration.h"

int main() {
    /*
    std::vector<Cell> cells(10000);
    for (int i = 0; i < 100; ++i) {
        cells.emplace_back(0);
    }*/
    Configuration conf(100, 6);
    for (int i = 0; i < 100; ++i) {
        if (i % 5 == 0) {
            conf.animate(1, i);
        } else {
            conf.runTime();
        }
    }
    for (int i = 0; i < 50000; ++i) {
        conf.addSand();
        if (i % 20 == 0) {
            conf.animate(1, i + 100);
        } else {
            conf.runTime();
        }
    }
}
