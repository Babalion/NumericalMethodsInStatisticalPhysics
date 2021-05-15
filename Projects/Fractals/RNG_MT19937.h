//
// Created by chris on 06.05.21.
//
#pragma once

#include <random>

struct RNG_MT19937 {
    RNG_MT19937(int a,int b);
//    explicit RNG_MT19937(RNG_MT19937 &rng);
//    ~RNG_MT19937();
    int getRandom();

private:
    std::uniform_int_distribution<int> dist;
    std::random_device rd;
    std::mt19937_64 mt;
};
