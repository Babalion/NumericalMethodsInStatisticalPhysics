//
// Created by chris on 06.05.21.
//

#ifndef NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CELL_H
#define NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CELL_H


class Cell {
public:
    Cell();
    //initializes with given height
    explicit Cell(unsigned int height);

    [[nodiscard]] unsigned int getHeight() const;//("Why you ask for height and don't use it?")
    void setHeight(unsigned int height_);
    //increases height by 1
    void incHeight();
    friend bool operator==(const Cell &lhs,const  Cell &rhs);

private:
    unsigned int height;
    int slopeToNeighbours;
public:
    int getSlopeToNeighbours() const;
    void setSlopeToNeighbours(int slopeToNeighbours);
};


#endif //NUMERISCHEMETHODENSTATISTISCHENPHYSIK_CELL_H
