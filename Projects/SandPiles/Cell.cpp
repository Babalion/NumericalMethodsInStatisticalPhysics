//
// Created by chris on 06.05.21.
//

#include "Cell.h"

unsigned int Cell::getHeight() const{
    return height;
}

void Cell::setHeight(unsigned int height_) {
    height = height_;
    slopeToNeighbours=0;
}

Cell::Cell(unsigned int height):height(height),slopeToNeighbours(0) {

}

Cell::Cell():height(0) {
}

bool operator==(const Cell &lhs, const Cell &rhs) {
    return (lhs.height==rhs.height)&&(lhs.getSlopeToNeighbours()==rhs.getSlopeToNeighbours());
}

int Cell::getSlopeToNeighbours() const {
    return slopeToNeighbours;
}

void Cell::setSlopeToNeighbours(int slopeToNeighbours) {
    Cell::slopeToNeighbours = slopeToNeighbours;
}

void Cell::incHeight() {
    height++;
}
