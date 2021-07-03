//
// Created by chris on 14.06.21.
//
#include "SpinLattice2level.h"


SpinLattice2level::SpinLattice2level(unsigned int sights) : J(1), performedSweeps(0), sights(sights),
                                                            spins(sights * sights) {
    initRandom();
}

void SpinLattice2level::printSpins() {
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

void SpinLattice2level::initRandom() {
    auto dist = std::uniform_int_distribution<int>(0, 1);
    auto rd = std::random_device();
    auto mt = std::mt19937(rd());

    for (int & spin : spins) {
        spin = dist(mt) == 0 ? -1 : 1;
    }
}

int SpinLattice2level::calcEnergy() const {
    int energy = 0;
    for (size_t i = 0; i < sights; ++i) {
        for (size_t j = 0; j < sights; ++j) {
            energy += calcEnergy(i, j);
        }
    }
    return energy;
}

int SpinLattice2level::calcEnergy(unsigned int x, unsigned int y) const {
    return calcEnergy(x, y, spins[x + y * sights]);
}

int SpinLattice2level::calcEnergy(unsigned int x, unsigned int y, int newSpin) const {
    const int J_val = J;
    const unsigned int i = x + y * sights;
    int energy = 0;
    if (x > 0) {// not at left boarder
        energy += newSpin * spins[i - 1];
    }
    if (x < sights - 1) {// not at right boarder
        energy += newSpin * spins[i + 1];
    }
    if (y > 0) {// not at top boarder
        energy += newSpin * spins[i - sights];
    }
    if (y < sights - 1) {// not at bottom boarder
        energy += newSpin * spins[i + sights];
    }

    return -1 * J_val * energy;
}

/**
 *
 * @return Magnetization, which is the sum over all spins
 */
int SpinLattice2level::calcMagnetization() const {
    int magnet = 0;
    for (int i = 0; i < spins.size(); ++i) {
        magnet += spins[i];
    }
    return magnet;
}

////////////////////////////////////////////////////////////////////////////////
/// Markov Algorithms
////////////////////////////////////////////////////////////////////////////////


void metropolisSweep(SpinLattice2level &spinLattice, float temp) {
    std::uniform_real_distribution<float> u(0, 1);
    std::uniform_int_distribution<int> newSpinGenerator(0, 1);
    std::random_device rd;
    auto mt = std::mt19937(rd());
    for (size_t i = 0; i < spinLattice.getSights(); ++i) {
        for (size_t j = 0; j < spinLattice.getSights(); ++j) {
            const int newSpin = newSpinGenerator(mt) == 0 ? -1 : 1;
            const int oldEnergy = spinLattice.calcEnergy(i, j);
            const int newEnergy = spinLattice.calcEnergy(i, j, newSpin);

            if (newEnergy < oldEnergy) {
                spinLattice(i, j) = newSpin;
            } else {
                const float rand = u(mt);
                const int deltaE = newEnergy - oldEnergy;
                if (rand < exp(static_cast<float>(-1 * deltaE) / temp)) {
                    spinLattice(i, j) = newSpin;
                }
            }
        }
    }
}

void metropolisSweep(SpinLattice2level &spinLattice, float temp, unsigned int iterations) {
    for (size_t i = 0; i < iterations; ++i) {
        metropolisSweep(spinLattice, temp);
    }
    spinLattice.performedSweeps++;
}

int heatBathSumOfNeighbours(SpinLattice2level &sl, unsigned int x, unsigned int y) {
    const auto sights = sl.getSights();
    const auto &spins = sl.getSpins();
    const unsigned int i = x + y * sights;
    int sum = 0;
    if (x > 0) {// not at left boarder
        sum += spins[i - 1];
    }
    if (x < sl.getSights() - 1) {// not at right boarder
        sum += spins[i + 1];
    }
    if (y > 0) {// not at top boarder
        sum += spins[i - sights];
    }
    if (y < sl.getSights() - 1) {// not at bottom boarder
        sum += spins[i + sights];
    }

    return sum;
}

void heatBathSweep(SpinLattice2level &spinLattice, float temp) {
    const auto J_val = spinLattice.J;
    std::uniform_real_distribution<float> u(0, 1);
    std::random_device rd;
    auto mt = std::mt19937(rd());
    for (size_t i = 0; i < spinLattice.getSights(); i += 1) {
        for (size_t j = 0; j < spinLattice.getSights(); j += 1) {
            const int delta = heatBathSumOfNeighbours(spinLattice, i, j);
            const float k = static_cast<float>(-1 * J_val * delta) / temp;
            const float q = exp(-1.0f * k) / 2.0f / cosh(k);
            const float r = u(mt);
            if (r < q) {
                spinLattice(i, j) = 1;
            } else {
                spinLattice(i, j) = -1;
            }
        }
    }
    spinLattice.performedSweeps++;
}

void heatBathSweepRandChoice(SpinLattice2level &spinLattice, float temp) {
    const auto J_val = spinLattice.J;
    std::uniform_real_distribution<float> u(0, 1);
    std::uniform_int_distribution<unsigned int> loc(0, spinLattice.getSights() - 1);
    std::random_device rd;
    auto mt = std::mt19937(rd());

    for (size_t i = 0; i < spinLattice.getSights() * spinLattice.getSights(); i++) {
        const unsigned int x = loc(mt);
        const unsigned int y = loc(mt);

        const int delta = heatBathSumOfNeighbours(spinLattice, x, y);
        const float k = static_cast<float>(-1 * J_val * delta) / temp;
        const float q = exp(-1.0f * k) / 2.0f / cosh(k);
        const float r = u(mt);
        if (r < q) {
            spinLattice(x, y) = 1;
        } else {
            spinLattice(x, y) = -1;
        }
    }
    spinLattice.performedSweeps++;
}
