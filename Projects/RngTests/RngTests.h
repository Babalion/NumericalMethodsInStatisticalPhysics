//
// Created by chris on 26.04.21.
//

#ifndef NUMERISCHEMETHODENSTATISTISCHENPHYSIK_RNGTESTS_H
#define NUMERISCHEMETHODENSTATISTISCHENPHYSIK_RNGTESTS_H
#include <iostream>
#include <list>
#include <random>
#include <math.h>
#include <chrono>

// Starts the Project RngTests. Returns 0 if no error occurs.
int runRngTests();

//Constants
const double pi = 2*std::acos(0);

// input: two lists of random numbers with same dimension between 0 and 1 and performs a randomness pi-Test
// output: the difference between pi and the estimated value of pi with the given lists of random values
// info: more random distribution in the lists result in a lower result
// ideal randomness for result=0
double piTest(std::list<double> randomNumbers1,std::list<double> randomNumbers2);

//test the standard rand() function
void compareRNGs();


#endif //NUMERISCHEMETHODENSTATISTISCHENPHYSIK_RNGTESTS_H
