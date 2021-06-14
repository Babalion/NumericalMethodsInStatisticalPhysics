//
// Created by chris on 14.06.21.
//
#include "SpinLattice.h"


SpinLattice::SpinLattice(unsigned int sights) : J(1), sights(sights) {
    auto dist = std::uniform_int_distribution<int>(0, 1);
    auto rd = std::random_device();
    auto mt = std::mt19937(rd());

    spins.reserve(sights * sights);
    for (unsigned int i = 0; i < sights * sights; i++) {
        spins[i] = dist(mt) == 0 ? -1 : 1;
    }

}

void SpinLattice::printSpins() {
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

int SpinLattice::calcEnergy() const {
    int energy = 0;
    for (size_t i = 0; i < sights; ++i) {
        for (size_t j = 0; j < sights; ++j) {
            energy += calcEnergy(i, j);
        }
    }
    return energy;
}

int SpinLattice::calcEnergy(unsigned int x, unsigned int y) const {
    const int J_val = J;
    const unsigned int i = y + x * sights;
    int energy = 0;
    if (i % sights != 0) {// not at left boarder
        energy += spins[i] * spins[i - 1];
    }
    if (i % (sights - 1) != 0) {// not at right boarder
        energy += spins[i] * spins[i + 1];
    }
    if (i > sights - 1) {// not at top boarder
        energy += spins[i] * spins[i - sights];
    }
    if (i < (sights - 1) * (sights - 1) - (sights - 1)) {// not at bottom boarder
        energy += spins[i] * spins[i + sights];
    }

    return -1*J_val*energy;
}

int SpinLattice::calcEnergy(unsigned int x, unsigned int y, int newSpin) {
    auto oldSpin = spins[y + x * sights];
    spins[y + x * sights] = newSpin;
    auto energy = calcEnergy(x, y);
    spins[y + x * sights] = oldSpin;

    return energy;
}

////////////////////////////////////////////////////////////////////////////////
/// Markov Algorithms
////////////////////////////////////////////////////////////////////////////////


void metropolisSweep(SpinLattice &spinLattice, float temp) {
    std::uniform_real_distribution<float> u(0, 1);
    std::uniform_int_distribution<int> newSpinGenerator(0, 1);
    std::random_device rd;
    auto mt = std::mt19937(rd());
    for (size_t i = 0; i < spinLattice.getSights(); ++i) {
        for (size_t j = 0; j < spinLattice.getSights(); ++j) {
            const int newSpin = newSpinGenerator(mt) == 0 ? -1 : 1;
            const int oldEnergy = spinLattice.calcEnergy(i, j);
            const int newEnergy = spinLattice.calcEnergy(i, j, newSpin);

            if (newEnergy <= oldEnergy) {
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

void metropolisSweep(SpinLattice &spinLattice, float temp, unsigned int iterations) {
    for (size_t i = 0; i < iterations; ++i) {
        metropolisSweep(spinLattice, temp);
    }
}

int heatBathSumOfNeighbours(SpinLattice &sl, unsigned int i, unsigned int j) {
    const auto sights = sl.getSights();
    const auto &spins = sl.getSpins();
    int sum = 0;
    if (i % sights != 0) {// not at left boarder
        sum += spins[i - 1];
    }
    if (i % (sights - 1) != 0) {// not at right boarder
        sum += spins[i + 1];
    }
    if (i > sights - 1) {// not at top boarder
        sum += spins[i - sights];
    }
    if (i < (sights - 1) * (sights - 1) - (sights - 1)) {// not at bottom boarder
        sum += spins[i + sights];
    }

    return sum;
}

void heatBathSweep(SpinLattice &spinLattice, float temp) {
    const int J_val = spinLattice.J;
    std::uniform_real_distribution<float> u(0, 1);
    std::random_device rd;
    auto mt = std::mt19937(rd());
    for (size_t i = 0; i < spinLattice.getSights(); ++i) {
        for (size_t j = 0; j < spinLattice.getSights(); ++j) {
            const int oldEnergy = spinLattice.calcEnergy(i, j);
            const int delta = heatBathSumOfNeighbours(spinLattice, i, j);
            const float k = static_cast<float>(J_val * delta + oldEnergy) / temp;
            const float q = exp(-1.0f * k) / 2 / cosh(k);
            const float r = u(mt);
            if (r < q) {
                spinLattice(i, j) = 1;
            } else {
                spinLattice(i, j) = -1;
            }
        }
    }
}




////////////////////////////////////////////////////////////////////////////////
/// RuntimeGUI
////////////////////////////////////////////////////////////////////////////////

void RuntimeGUI::notify(const SpinLattice &instance) {
    // The screen is split as follows:
    // 75% points
    // 25% status

    // Clear the gui
    gui = cv::Scalar(40, 40, 40);

    // Get the status marker
    int statusCol = 0.75 * gui.cols;

    // Write the status
    std::stringstream ss;
    ss << "energy = " << instance.calcEnergy();
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 15),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    /*
    ss.str("");
    ss << "outer = " << config.outer;
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 30),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    ss.str("");
    ss << "inner = " << config.inner;
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 45),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    ss.str("");
    ss << "energy = " << config.energy;
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 60),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    ss.str("");
    ss << "best energy = " << config.bestEnergy;
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 75),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));

    ss.str("");
    ss << "epsilon =  = " << config.bestEnergy;
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 90),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
*/

    // Plot the charts
    // [...]

    // Plot the cities
    // Determine the minimum and maximum X/Y
    float minX = 0;
    float minY = 0;
    auto maxX = static_cast<float>(instance.getSights());
    auto maxY = static_cast<float>(instance.getSights());


    // Calculate the compression factor
    float width = maxX - minX;
    float height = maxY - minY;
    float compression = (statusCol - 10) / width;
    if (height * compression > gui.rows - 10) {
        compression = (gui.rows - 10) / height;
    }

    // Paint the spins
    for (size_t i = 0; i < instance.getSights() * instance.getSights(); i++) {
        cv::Point p1;
        p1.x = static_cast<float>((i % instance.getSights())) * compression + 10;
        p1.y = static_cast<float>((i / instance.getSights())) * compression + 10;

        if (instance.getSpins()[i] == -1) {
            //Coler Ordering BGR (blue,green,red)
            cv::circle(gui, p1, 2, cv::Scalar(212, 188, 0), 5);
        } else {
            cv::circle(gui, p1, 2, cv::Scalar(255, 0, 255), 5);
        }
    }

    cv::imshow("GUI", gui);
    cv::waitKey(waitTime);
    /*
    if (config.terminated) {
        cv::waitKey(0);
    } else {
        cv::waitKey(waitTime);
    }*/
}