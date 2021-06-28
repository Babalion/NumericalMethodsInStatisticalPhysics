//
// Created by chris on 26.04.21.
//
#include <CvPlot/cvplot.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <iostream>

#include "test.h"

// Starts the Project Hello World. Returns 0 if no error occurs.
int runHelloWorld() {

    std::cout << "HelloWorld! Es scheint alles zu funktionieren!." << std::endl;

    test A = test();
    std::cout << A.n << std::endl;

    return 0;
}

void testFFT() {
    int n = 3000;
    std::vector<double> data(n);

    gsl_fft_real_wavetable *real;
    gsl_fft_halfcomplex_wavetable *hc;
    gsl_fft_real_workspace *work;

    for (int i = 0; i < n; i++) {
        data[i] = ((double) (i % 500)) / 500;
    }

    auto axes = CvPlot::makePlotAxes();
    auto ac=autoCorr(data);
    normalize(ac);
    axes.create<CvPlot::Series>(ac, "-b");

    printf("Daten\n");
    for (int i = 0; i < n; i++) {
        printf("%d: %e\n", i, data[i]);
    }
    printf("\n");

    work = gsl_fft_real_workspace_alloc(n);
    real = gsl_fft_real_wavetable_alloc(n);


    gsl_fft_real_wavetable_free(real);

    hc = gsl_fft_halfcomplex_wavetable_alloc(n);

    gsl_fft_halfcomplex_inverse(&data[0], 1, n, hc, work);

    data.resize(data.size()/2);
    for (size_t i = 0; i < data.size(); ++i) {
            data[i] /= data[0];
    }

    gsl_fft_halfcomplex_wavetable_free(hc);

    printf("FFT-Daten\n");
    for (int i = 0; i < data.size(); i++) {
        printf("%d: %e\n", i, data[i]);
    }

    axes.create<CvPlot::Series>(data, "-r");//.setLineSpec("o");

    CvPlot::show("autocorr", axes);

    gsl_fft_real_workspace_free(work);
}


int main() {
    testFFT();
}
