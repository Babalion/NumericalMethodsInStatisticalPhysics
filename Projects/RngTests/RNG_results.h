//
// Created by chris on 27.04.21.
//
#pragma once

#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

//Constants
const double pi = 2*acos(0);

class RNG_results{
public:
    RNG_results();
    ~RNG_results();
    std::mutex mutex_push_results;
    void push_results(unsigned long long,double diff_LCG,double diff_MT19937,double time_LCG,double time_MT19937);
    void saveResults();
    void saveAndClearResults();
private:
    std::vector<unsigned long long> list_n;
    std::vector<double> list_diff_LCG;
    std::vector<double> list_diff_MT19937;
    std::vector<double> list_time_ms_LCG;
    std::vector<double> list_time_ms_MT19937;
    std::ofstream resultsFile;
};
