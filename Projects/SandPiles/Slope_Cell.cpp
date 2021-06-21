//
// Created by chris on 06.05.21.
//

#include "Slope_Cell.h"

Slope_Cell::Slope_Cell() {
    slope=0;
}

int Slope_Cell::getSlope() const {
    return slope;
}

void Slope_Cell::setSlope(int slope_) {
    slope = slope_;
}

Slope_Cell::Slope_Cell(int slope):slope(slope) {

}

bool operator==(const Slope_Cell &lhs, const Slope_Cell &rhs) {
    return lhs.getSlope()==rhs.getSlope();
}
