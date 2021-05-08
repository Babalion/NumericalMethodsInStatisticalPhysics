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
    std::vector<Slope_Cell> vSlopes = {Slope_Cell(0), Slope_Cell(3),
                                       Slope_Cell(2), Slope_Cell(-7),
                                       Slope_Cell(2), Slope_Cell(9)};
    std::vector<Slope_Cell> hSlopes = {Slope_Cell(4), Slope_Cell(6), Slope_Cell(-4),
                                       Slope_Cell(-4), Slope_Cell(-4), Slope_Cell(12)};
    std::vector<Slope_Cell> locSlopes = {Slope_Cell(4), Slope_Cell(-20),
                                         Slope_Cell(0), Slope_Cell(-4)};

    //std::vector<Cell> cells({Cell(),Cell()});
    Configuration conf = Configuration(cells, 4);

    assertEqual (conf.getFieldWidth() == sqrt(cells.size()));
    assertEqual (conf.getHSlopes() == hSlopes);
    assertEqual (conf.getVSlopes() == vSlopes);
    assertEqual (conf.getLocSlopes() == locSlopes);

    conf.plot();
    return err_code;
}

int testH_V_Slopes_CornerCases() {
    int err_code = 0;

    std::vector<Cell> cells_one({Cell(1)});
    std::vector<Slope_Cell> vSlopes_empty = {};
    std::vector<Slope_Cell> hSlopes_empty = {};

    //std::vector<Cell> cells({Cell(),Cell()});
    Configuration conf = Configuration(cells_one, 4);

    assertEqual (conf.getFieldWidth() == sqrt(cells_one.size()));
    assertEqual (conf.getHSlopes() == hSlopes_empty);
    assertEqual (conf.getVSlopes() == vSlopes_empty);

    return err_code;
}


int main() {
    int err_code = 0;
    err_code += testH_V_Slopes_CornerCases();
    err_code += testH_V_Slopes();
    return err_code;
}