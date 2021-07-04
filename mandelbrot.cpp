#include <iostream>
#include <complex>
#include <omp.h>
#include <math.h>

using namespace std;

/* The complex number library for reference: https://en.cppreference.com/w/cpp/numeric/complex
 */

unsigned long long int iterations(unsigned long long int maxIterations, complex<long double> point, long double threshold) {
    std::complex<long double> z(0.0, 0.0);
    std::complex<long double> prod(0.0, 0.0);
    std::complex<long double> sum(0.0, 0.0);

    for (unsigned long long int iteration = 1; iteration <= maxIterations; iteration++) {
        z = point + (z * z);

        if (abs(z) >= threshold) {
            return iteration;
        }
    }
    return 0;
}

void mandelbrot(long double threshold, unsigned long long int maxIterations, complex<long double> center, long double scale, unsigned long long int resolution, unsigned long long int *output) {
    if (resolution == 0) {
        output[0] = iterations(maxIterations, center, threshold);
    } else {
        unsigned long long int length = 2 * resolution + 1;
        unsigned long long int* image = new unsigned long long int [length * length];
        
        long double unit = (long double) scale / (long double) resolution;
        
        *output = *image;

        long double iter_real = real(center) - scale;
        long double iter_imag = imag(center) + scale;
    
        long double *iter_reals = new long double[length];
        long double *iter_imags = new long double[length];

        for (unsigned long long int i = 0; i < length; i++) {
            iter_imags[i] = iter_imag;
            iter_imag -= unit;
        } 
        
        for (unsigned long long int i = 0; i < length; i++) {
            iter_reals[i] = iter_real;
            iter_real += unit;
        }
        
        #pragma omp parallel for
        for (unsigned long long int i = 0; i < length * length; i++) {
            output[i] = iterations(maxIterations, complex<long double>(iter_reals[i % length], iter_imags[i / length]), threshold);
        }
    }
}