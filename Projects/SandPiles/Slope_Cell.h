//
// Created by chris on 06.05.21.
//

#ifndef NUMERISCHEMETHODENSTATISTISCHENPHYSIK_SLOPE_CELL_H
#define NUMERISCHEMETHODENSTATISTISCHENPHYSIK_SLOPE_CELL_H


class Slope_Cell {
public:
    //initializes slope with 0
    Slope_Cell();
    explicit Slope_Cell(int slope);
    [[nodiscard]] int getSlope() const;//("Why you aks for slope and don't use it?")
    void setSlope(int slope);
    friend bool operator==(const Slope_Cell &lhs,const  Slope_Cell &rhs);
private:
    int slope;

};


#endif //NUMERISCHEMETHODENSTATISTISCHENPHYSIK_SLOPE_CELL_H
