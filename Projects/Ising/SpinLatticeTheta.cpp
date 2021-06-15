//
// Created by chris on 15.06.21.
//

#include "SpinLatticeTheta.h"

SpinLatticeTheta::SpinLatticeTheta(unsigned int sights) : J(1), sights(sights) {
    auto dist = std::uniform_real_distribution<float>(0, CV_2PI);
    auto rd = std::random_device();
    auto mt = std::mt19937(rd());

    spins.reserve(sights * sights);
    for (unsigned int i = 0; i < sights * sights; i++) {
        spins[i] = dist(mt);
    }
}

void SpinLatticeTheta::printSpins() {
    for (unsigned int i = 0; i < sights * sights; i++) {
        std::cout << spins[i];
        if ((i + 1) % sights == 0) {//right boarder
            std::cout << std::endl;
        } else {
            std::cout << "\t";
        }
    }
    std::cout << std::endl;
}

float SpinLatticeTheta::calcEnergy(unsigned int x, unsigned int y, float newSpinTheta) const {
    const unsigned int i = y + x * sights;
    float energy = 0;
    if (y > 0) {// not at left boarder
        energy += cos(newSpinTheta - spins[i - 1]);
    }
    if (y < sights - 1) {// not at right boarder
        energy += cos(newSpinTheta - spins[i + 1]);
    }
    if (x > 0) {// not at top boarder
        energy += cos(newSpinTheta - spins[i - sights]);
    }
    if (x < sights - 1) {// not at bottom boarder
        energy += cos(newSpinTheta - spins[i + sights]);
    }

    return -1.0f * static_cast<float>(J) * energy;
}

float SpinLatticeTheta::calcEnergy(unsigned int x, unsigned int y) const {
    return calcEnergy(x, y, spins[y + x * sights]);
}

float SpinLatticeTheta::calcEnergy() const {
    float energy = 0;
    for (size_t i = 0; i < sights; ++i) {
        for (size_t j = 0; j < sights; ++j) {
            energy += calcEnergy(i, j);
        }
    }
    return energy;
}

void metropolisSweep(SpinLatticeTheta &spinLattice, float temp) {
    std::uniform_real_distribution<float> u(0, 1);
    std::uniform_real_distribution<float> newThetaGenerator(0, CV_2PI);
    std::random_device rd;
    auto mt = std::mt19937(rd());
    for (size_t i = 0; i < spinLattice.getSights(); ++i) {
        for (size_t j = 0; j < spinLattice.getSights(); ++j) {
            const float newSpin = newThetaGenerator(mt);
            const float oldEnergy = spinLattice.calcEnergy(i, j);
            const float newEnergy = spinLattice.calcEnergy(i, j, newSpin);

            if (newEnergy <= oldEnergy) {
                spinLattice(i, j) = newSpin;
            } else {
                // TODO track rejection rate
                const float rand = u(mt);
                const float deltaE = newEnergy - oldEnergy;
                if (rand < exp(-1.0f * deltaE / temp)) {
                    spinLattice(i, j) = newSpin;
                }
            }
        }
    }
}

void metropolisSweep(SpinLatticeTheta &spinLattice, float temp, unsigned int iterations) {
    for (size_t i = 0; i < iterations; ++i) {
        metropolisSweep(spinLattice, temp);
    }
}