//
// Created by chris on 05.05.21.
//

#include "Configuration.h"
#include "utils.h"

int main() {
    Configuration conf(50, 4);
    // Register the GUI
    // You can specify the dimensions of the window
    RuntimeGUI gui(1000, 1200);
    gui.waitTime = 7;
    gui.notify(conf);

    static const int runs =10000;

    for (int i = 0; i < runs; i++) {
        if(i%3==0){
            conf.addSand();
        }
        conf.runTime();
        gui.notify(conf);
    }
    cv::waitKey(0);
    cv::destroyAllWindows();
}
