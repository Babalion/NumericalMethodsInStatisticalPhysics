//
// Created by chris on 26.04.21.
//
#include <iostream>
#include "HelloWorld.h"
#include "../../lib/matplotlibcpp.h"
#include "test.h"

namespace plt = matplotlibcpp;

// Starts the Project Hello World. Returns 0 if no error occurs.
int runHelloWorld() {
    plt::plot({1, 3, 2, 4});
    plt::show();
    std::cout << "HelloWorld! Es scheint alles zu funktionieren!." << std::endl;

    test A=test();
    std::cout<<A.n<<std::endl;

    return 0;
}
