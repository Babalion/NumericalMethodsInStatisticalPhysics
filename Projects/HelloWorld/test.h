//
// Created by chris on 07.05.21.
//
#pragma once

#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <numeric>
#include <vector>


class test {
public:
    int n;
};

/**
 * Calculates autocorrelation with FFT
 * @param vec
 * @return
 *//*
std::vector<double> ac(const std::vector<double> &vec){
    std::vector<double> fftReturn=vec;

    gsl_fft_real_wavetable *real;
    gsl_fft_halfcomplex_wavetable *hc;
    gsl_fft_real_workspace *work;

    //FFT
    work = gsl_fft_real_workspace_alloc(vec.size());
    real = gsl_fft_real_wavetable_alloc(vec.size());

    gsl_fft_real_transform(&fftReturn[0], 1, vec.size(), real, work);
    gsl_fft_real_wavetable_free(real);

    hc = gsl_fft_halfcomplex_wavetable_alloc(vec.size());

    gsl_fft_halfcomplex_inverse(&fftReturn[0], 1, vec.size(), hc, work);

    gsl_fft_halfcomplex_wavetable_free(hc);
    return fftReturn;
}*/


/**
 * @tparam T
 * @param vec
 * @return maximum element of given vector
 */
template<typename T>
T max(const std::vector<T> &vec) {
    T maxVal = vec[0];
    for (int i = 1; i < vec.size(); ++i) {
        maxVal = std::max(maxVal, vec[i]);
    }
    return maxVal;
}

/**
 * @tparam T
 * @param vec
 * @return minimum element of given vector
 */
template<typename T>
T min(const std::vector<T> &vec) {
    T minVal = vec[0];
    for (int i = 1; i < vec.size(); ++i) {
        minVal = std::min(minVal, vec[i]);
    }
    return minVal;
}

/**
 * Calculates the arithmetic mean of given vector-elements
 * @tparam T
 * @param vec
 * @return the arithmetic mean
 */
template<typename T>
T mean(const std::vector<T> &vec) {
    return std::accumulate(vec.begin(), vec.end(), 0.0)/vec.size();
}

/**
 * Calculates the arithmetic mean of given vector-of-vector-of-elements
 * @tparam T
 * @param vec vector of vector of elements (tensor of rank 2)
 * @return the arithmetic mean of all values
 */
template<typename T>
T mean(const std::vector<std::vector<T>> &vec) {
    std::vector<T> firstMeans;
    firstMeans.reserve(vec.size());
    for(auto &i:vec){
        firstMeans.push_back(mean(i));
    }
    return mean(firstMeans);
}

/**
 * Calculates the standard deviation of given vector-elements
 * @tparam T
 * @param vec
 * @return standard deviation
 */
template<typename T>
T stdev(const std::vector<T> &vec) {
    T summation=0;
    const T vec_mean=mean(vec);
    for (size_t i = 0; i < vec.size(); ++i) {
        summation+=pow(vec[i]-vec_mean,2);
    }
    return sqrt(summation*1.0/(vec.size()-1));
}

/**
 * Calculates the autocorrelation-function of given data-sample
 * @tparam T
 * @param vec of data-sample with length n
 * @return autocorrelation-function in vector with length n/2
 */
template<typename T>
std::vector<T> autoCorr(const std::vector<T> &vec) {
    float mean = std::accumulate(vec.begin(), vec.end(), 0.0f) / vec.size();

    std::vector<T> autocorrelation(vec.size() / 2);
    for (size_t t = 0; t < autocorrelation.size(); t++) {
        float n = 0; // Numerator
        float d = 0; // Denominator
        for (size_t i = 0; i < vec.size() - t; i++) {
            float xim = vec[i] - mean;
            n += xim * (vec[(i + t) % vec.size()] - mean);
            d += xim * xim;
        }
        autocorrelation[t] = n / d;
    }
    return autocorrelation;
}

/**
 * normalizes given vector call by reference
 * @tparam T
 * @param vec vector which should be normalized
 */
template<typename T>
void normalize(std::vector<T> &vec) {
    const T vec_max = max(vec);
    for (T &i : vec) {
        i /= 1.0 * vec_max;
    }
}