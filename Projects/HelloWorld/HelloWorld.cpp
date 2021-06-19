//
// Created by chris on 26.04.21.
//
#include <iostream>
#include "HelloWorld.h"
#include "test.h"

int main(){
    runHelloWorld();
}

// Starts the Project Hello World. Returns 0 if no error occurs.
int runHelloWorld() {

    std::cout << "HelloWorld! Es scheint alles zu funktionieren!." << std::endl;

    test A=test();
    std::cout<<A.n<<std::endl;

    return 0;
}
