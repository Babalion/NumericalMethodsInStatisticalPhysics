//
// Created by chris on 06.05.21.
//

#include "RNG_MT19937.h"

RNG_MT19937::RNG_MT19937(int a, int b):dist(std::uniform_int_distribution(a,b)) {
    mt = std::mt19937_64(rd());
    //initialize MT19937. Therefore let it generate 8E5 random numbers
    // to be sure the initial value doesnt affect randomness badly anymore.
    for (uint_fast32_t i = 0; i < 8E5; i++) {
        dist(mt);
    }
}

int RNG_MT19937::getRandom() {
    return dist(mt);
}

/*
RNG_MT19937::RNG_MT19937(RNG_MT19937 &rng) {
    dist=rng.dist;
    mt=std::mt19937_64(rd());
    for (uint_fast32_t i = 0; i < 8E5; i++) {
        dist(mt);
    }
}
*/
/*
RNG_MT19937::~RNG_MT19937() {

}
*/