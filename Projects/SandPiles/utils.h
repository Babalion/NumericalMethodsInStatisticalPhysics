//
// Created by chris on 21.06.21.
//
#pragma once

#include "Configuration.h"

/**
 * a function for convenience creating a Scalar from HEX-colors
 * @param hexValue: a Color in HEX-Format like #FFA617, exchange # with 0X
 * @return a OpenCV Scalar-object
 */
cv::Scalar hexToCVScalar(int hexValue){
    return cv::Scalar(((hexValue) & 0xFF),((hexValue >> 8) & 0xFF) ,((hexValue >> 16) & 0xFF));
}

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
    void notify(const Configuration &conf);


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

void RuntimeGUI::notify(const Configuration &conf) {
    // The screen is split as follows:
    // 75% points
    // 25% status

    // Clear the gui
    gui = cv::Scalar(40, 40, 40);

    // Get the status marker
    int statusCol = 0.75 * gui.cols;

    // Write the status
    std::stringstream ss;
    ss << "field width = " << conf.getFieldWidth();
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 15),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    ss.str("");
    ss << "amount of sand = " << conf.amountOfSand();
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 30),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    ss.str("");
    ss << "max height = " << conf.maxHeight();
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 45),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    ss.str("");
    ss << "criticality = " << conf.getCriticality();
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 60),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
    ss.str("");
    ss << "time = " << conf.getTime();
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 75),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));

    ss.str("");/*
    ss << "epsilon =  = " << config.bestEnergy;
    cv::putText(gui,
                ss.str(),
                cv::Point(statusCol, 90),
                cv::FONT_HERSHEY_PLAIN,
                0.9,
                cv::Scalar(255, 255, 255));
*/


    // Determine the minimum and maximum X/Y
    static const unsigned int minX = 0;
    static const unsigned int minY = 0;
    const unsigned int maxX = conf.getFieldWidth();
    const unsigned int maxY = conf.getFieldWidth();


    // Calculate the compression factor
    int width = static_cast<int>(maxX) - static_cast<int>(minX);
    int height = static_cast<int>(maxY) - static_cast<int>(minY);
    float compression = static_cast<float>(statusCol - 10) / static_cast<float>(width);
    if (height * compression > gui.rows - 10) {
        compression = (gui.rows - 10) / height;
    }

    // Calculate the highest height to rescale colors
    /*
     * unsigned const static int maxHeight=conf.maxHeight()-5;
     */
    // if you dont want color rescale, comment out the following lines and uncomment lines above
    unsigned static int maxHeight = conf.maxHeight();
    unsigned static int minHeight = conf.minHeight();
    if (conf.getTime() % 100 == 0) {
        maxHeight = conf.maxHeight();
        minHeight = conf.minHeight();
    }

    // Paint the spins
    for (size_t i = 0; i < maxX * maxY; i++) {
        cv::Point p1;
        p1.x = static_cast<float>(i % maxX) * compression + 10;
        p1.y = static_cast<float>(i / maxY) * compression + 10;

        static const int numOfColors = 7;
        float range = static_cast<float>(conf.getCells()[i].getHeight() - minHeight) / static_cast<float>(maxHeight - minHeight) * numOfColors;

        auto color = cv::Scalar();
        // Color-palette from https://gka.github.io/palettes/#/7|d|ff006a,ffffff,ffffff|ffffff,ffffff,0088d7|1|1
        if (range > 6) {
            color = hexToCVScalar(0Xff006a);
        } else if (range > 5) {
            color = hexToCVScalar(0Xff849d);
        } else if (range > 4) {
            color = hexToCVScalar(0Xffc5ce);
        } else if (range > 3) {
            color = hexToCVScalar(0Xffffff);
        } else if (range > 2) {
            color = hexToCVScalar(0Xc2d5f2);
        } else if (range > 1) {
            color = hexToCVScalar(0X7faee5);
        } else {
            color = hexToCVScalar(0X0088d7);
        }
        cv::circle(gui, p1, 3, color, 3);
    }

    cv::imshow("GUI", gui);
    cv::waitKey(waitTime);
}
