//
// Created by chris on 06.05.21.
//

#include "Cell.h"

unsigned int Cell::getHeight() const{
    return height;
}

void Cell::setHeight(unsigned int height_) {
    height = height_;
}

Cell::Cell(unsigned int height):height(height) {

}

Cell::Cell():height(0) {
}

bool operator==(const Cell lhs, const Cell rhs) {
    return lhs.height==rhs.height;
}
