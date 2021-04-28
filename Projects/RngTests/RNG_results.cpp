//
// Created by chris on 27.04.21.
//

#include "RNG_results.h"
void RNG_results::saveResults() {
    std::lock_guard<std::mutex> guard(mutex_push_results);
    std::cout<<"Saving all simulated data to disk...."<<std::endl;

    // Save the results
    auto it1 = list_n.begin();
    auto it2 = list_diff_LCG.begin();
    auto it3 = list_time_ms_LCG.begin();
    auto it4 = list_diff_MT19937.begin();
    auto it5 = list_time_ms_MT19937.begin();

    for (; it1 != list_n.end() && it2 != list_diff_LCG.end() && it3 != list_time_ms_LCG.end()
           && it4 != list_diff_MT19937.end() && it5 != list_time_ms_MT19937.end();
           ++it1, ++it2, ++it3, ++it4, ++it5) {
        //std::cout << *it1 << "\t" << *it2 << "\t" << *it3 << "\t" << *it4 << "\t" << *it5 << std::endl;
        resultsFile << *it1 << "\t" << *it2 << "\t" << *it3 << "\t" << *it4 << "\t" << *it5 << std::endl;
    }
}

void RNG_results::push_results(unsigned long long n, double diff_LCG, double diff_MT19937, double time_LCG, double time_MT19937) {
    std::lock_guard<std::mutex> guard(mutex_push_results);
    list_n.push_back(n);
    list_diff_LCG.push_back(diff_LCG);
    list_diff_MT19937.push_back(diff_MT19937);
    list_time_ms_LCG.push_back(time_LCG);
    list_time_ms_MT19937.push_back(time_MT19937);
}

RNG_results::RNG_results() {
    resultsFile.open("compareRNGs.tsv");
    //std::cout << "n\tLCG-diff\tLCG-time-mus\tMT19937-diff\tMT19937-time-mus" << std::endl;
    resultsFile << "n\tLCG-diff\tLCG-time-mus\tMT19937-diff\tMT19937-time-mus" << std::endl;
}

RNG_results::~RNG_results() {
    resultsFile.close();
}

