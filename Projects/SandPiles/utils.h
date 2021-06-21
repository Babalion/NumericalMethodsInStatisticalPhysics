//
// Created by chris on 21.06.21.
//
#pragma once

#include "Configuration.h"

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

    // Calculate the highest height
    unsigned int maxHeight=conf.getCells()[0].getHeight();
    for (auto i : conf.getCells()) {
        maxHeight=std::max(maxHeight,i.getHeight());
    }

    // Paint the spins
    for (size_t i = 0; i < maxX * maxY; i++) {
        cv::Point p1;
        p1.x = static_cast<float>(i % maxX) * compression + 10;
        p1.y = static_cast<float>(i / maxY) * compression + 10;

        auto color=cv::Scalar();
        if (conf.getCells()[i].getHeight()>maxHeight*6/7) {
            color=cv::Scalar(106, 0, 255);
        }else if(conf.getCells()[i].getHeight()>maxHeight*5/7){
            color=cv::Scalar(132, 91, 255);
        }else if(conf.getCells()[i].getHeight()>maxHeight*4/7){
            color=cv::Scalar(157, 132, 255);
        }else if(conf.getCells()[i].getHeight()>maxHeight*3/7){
            color=cv::Scalar(182, 166, 255);
        }else if(conf.getCells()[i].getHeight()>maxHeight*2/7){
            color=cv::Scalar(206, 197, 255);
        }else if(conf.getCells()[i].getHeight()>maxHeight*1/7){
            color=cv::Scalar(230, 226, 255);
        }else{
            color=cv::Scalar(255, 255, 255);
        }
        cv::circle(gui, p1, 3, color, 3);
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
