//
// Created by chris on 08.05.21.
//
#include "assert_macro.h"
#include "../../Projects/SandPiles/Configuration.h"

int testH_V_Slopes() {
    int err_code = 0;

    std::vector<Cell> cells({Cell(1), Cell(1), Cell(4),
                             Cell(5), Cell(7), Cell(0),
                             Cell(1), Cell(3), Cell(12)});


    //std::vector<Cell> cells({Cell(),Cell()});
    Configuration conf = Configuration(cells, 10);

    assertEqual (conf.getFieldWidth() == sqrt(cells.size()));

    conf.plot();

    std::vector<Cell> cell_after1Run({Cell(1), Cell(1), Cell(4),
                                      Cell(5), Cell(7), Cell(1),
                                      Cell(1), Cell(4), Cell(8)});

    conf.runTime();
    assertEqual (conf.getCells() == cell_after1Run); //doesnt work bec Slope doesn't match
    conf.plot();
    conf.printFig();
    return err_code;
}

int testH_V_Slopes_CornerCases() {
    int err_code = 0;

    std::vector<Cell> cells_one({Cell(1)});

    //std::vector<Cell> cells({Cell(),Cell()});
    Configuration conf = Configuration(cells_one, 4);

    assertEqual (conf.getFieldWidth() == sqrt(cells_one.size()));
    return err_code;
}

int testPrintFig() {
    std::vector<Cell> cells({Cell(1), Cell(1),
                             Cell(0), Cell(0)});
    Configuration conf = Configuration(cells, 10);
    conf.printFig();
    int k=0;
    k++;
    return k;
}


int main() {
    int err_code = 0;
    err_code += testH_V_Slopes_CornerCases();
    err_code += testH_V_Slopes();
    testPrintFig();
    return err_code;
}