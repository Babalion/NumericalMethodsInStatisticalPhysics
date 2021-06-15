//
// Created by chris on 15.06.21.
//
#pragma once

#include "SpinLatticeTheta.h"
#include "SpinLattice2level.h"

/**
 * This is the runtime GUI that let's you watch what happens during the
 * optimization procedure
 */
class RuntimeGUI {
public:
    /**
     * Constructor
     */
    RuntimeGUI(int rows, int cols) : waitTime(25), gui(rows, cols, CV_8UC3) {
        // Open the window
        cv::namedWindow("GUI", 1);
    }

    /**
     * Destructor
     */
    virtual ~RuntimeGUI() {
        cv::destroyWindow("GUI");
    }

    /**
     * Paint the gui
     */
    void notify(const SpinLattice2level &instance);

    void notify(const SpinLatticeTheta &instance);

    /**
     * The time the GUI pauses after each update. Set to 0 to let
     * it wait for a keypress
     */
    int waitTime;

private:
    /**
     * The GUI matrix
     */
    cv::Mat gui;
};


////////////////////////////////////////////////////////////////////////////////
/// RuntimeGUI
////////////////////////////////////////////////////////////////////////////////

void RuntimeGUI::notify(const SpinLattice2level &instance) {
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

void RuntimeGUI::notify(const SpinLatticeTheta &instance) {
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
    static const float minX = 0;
    static const float minY = 0;
    const auto maxX = static_cast<float>(instance.getSights());
    const auto maxY = static_cast<float>(instance.getSights());


    // Calculate the compression factor
    float width = maxX - minX;
    float height = maxY - minY;
    float compression = static_cast<float>(statusCol - 10) / width;
    if (height * compression > static_cast<float>(gui.rows - 10)) {
        compression = static_cast<float>(gui.rows - 10) / height;
    }

    // Paint the spins
    for (size_t i = 0; i < instance.getSights(); i++) {
        for (size_t j = 0; j < instance.getSights(); ++j) {
            cv::Point p1; //origin of each point
            p1.x = static_cast<float>(i) * compression + 10;
            p1.y = static_cast<float>(j) * compression + 10;

            cv::Point p2; //origin of each point
            static const float r = 5;
            p2.x = p1.x + r * cos(instance(i, j));
            p2.y = p1.y + r * sin(instance(i, j));

            cv::arrowedLine(gui, p1, p2, cv::Scalar(212, 188, 0), 1.5);
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
template<typename T>
T max(const std::vector<T> &vec) {
    T maxVal=vec[0];
    for (int i = 1; i < vec.size(); ++i) {
        maxVal=std::max(maxVal, vec[i]);
    }
    return maxVal;
}

template<typename T>
std::vector<T> autoCorr(const std::vector<T> &vec) {
    float mean = std::accumulate(vec.begin(),vec.end(),0.0f)/vec.size();

    std::vector<float> autocorrelation(vec.size()/2);
    for (int t = 0; t < autocorrelation.size(); t ++)
    {
        float n = 0; // Numerator
        float d = 0; // Denominator
        for (int i = 0; i < vec.size()-t; i ++)
        {
            float xim = vec[i] - mean;
            n += xim * (vec[(i + t) % vec.size()] - mean);
            d += xim * xim;
        }
        autocorrelation[t] = n / d;
    }
    return autocorrelation;
}

