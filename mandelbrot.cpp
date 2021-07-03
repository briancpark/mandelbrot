#include <iostream>
#include <complex>
#include <omp.h>
using namespace std;

/* The complex number library for reference: https://en.cppreference.com/w/cpp/numeric/complex
 */

unsigned long long int iterations(unsigned long long int  maxIterations, complex<long double> point, long double threshold) {
    std::complex<long double> z(0.0, 0.0);
    std::complex<long double> prod(0.0, 0.0);
    std::complex<long double> sum(0.0, 0.0);

    for (unsigned long long int iteration = 1; iteration <= maxIterations; iteration++) {
        prod = z * z;
        z = prod + point;

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

        long double init_real = -scale;
        long double init_imag = scale;
        long double reset_real = -(2 * scale);
        long double reset_imag = 0;
        long double iter_real = real(center) + init_real;
        long double iter_imag = imag(center) + init_imag;

        long double ReIncrement = unit;
        long double ImIncrement = -unit;

        #pragma omp parallel for
        for (unsigned long long int row = 0; row < length; row++) {
            for (unsigned long long int col = 0; col < length; col++) {
                std::complex<long double> iter(iter_real, iter_imag);
                output[(row * length) + col] = iterations(maxIterations, iter, threshold);
                iter_real += ReIncrement;
            }
            iter_real += reset_real;
            iter_real -= unit;
            iter_imag += ImIncrement;
        }
    }
}

int main() {
    std::complex<long double> point(0.2, 1.2);
    cout << iterations(10023, point, 100) << endl;
    return 0;
}