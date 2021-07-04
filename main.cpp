#include <iostream>
#include <complex>
#include <omp.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/types.h>
#include <string.h>

using namespace std;

unsigned long long int iterations(unsigned long long int maxIterations, complex<long double> point, long double threshold);
void mandelbrot(long double threshold, unsigned long long int maxIterations, complex<long double> center, long double scale, unsigned long long int resolution, unsigned long long int *output);

uint8_t* colorMapLoader(char* file, int* colorCount);

int main(int argc, char* argv[]) {
    if (argc != 11) {
        cout << "An error occured, wrong arguments passed. Should be in the form:" << endl;
        cout << "<threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>" << endl;
        return 1;
    }

    // Load Arguements
    char *pEnd;
    long double threshold = stold(argv[1]);
    unsigned long long int maxIterations = strtoull(argv[2], &pEnd, 10);
    std::complex<long double> center(stold(argv[3]), stold(argv[4]));
    long double initialScale = stold(argv[5]);
    long double finalScale = stold(argv[6]);
    unsigned long long int frames = strtoull(argv[7], &pEnd, 10);
    unsigned long long int resolution = strtoull(argv[8], &pEnd, 10);
    char* outputDirectory = argv[9];
    char* colorFile = argv[10];

    int* colorCount = (int*) malloc(sizeof(int));

    uint8_t* colorMap = colorMapLoader(colorFile, colorCount);

    long double scale = initialScale;

    //Writing to image
    unsigned long long int size = 2 * resolution + 1;
    unsigned long long int canvas = size * size;
    uint8_t* color;

    for (unsigned long long int frame = 0; frame < frames; frame++) {
        char* file = (char*) malloc((sizeof(char)) * (20 + (strlen(outputDirectory))));
        sprintf(file, "%s/frame%05llu.ppm", outputDirectory, frame);
        FILE *f = fopen(file, "w+");

        unsigned long long int* output = new unsigned long long int [((2 * resolution) + 1) * ((2 * resolution) + 1)];
        long double scale = (long double) exp((long double) log(initialScale) + (long double) ((long double) frame / (long double) (frames - 1) * log((long double) finalScale / (long double) initialScale)));
        //Internally parallelized
        mandelbrot(threshold, maxIterations, center, scale, resolution, output);

        uint8_t* img = new uint8_t[3 * canvas];

		if (img == NULL) {
			free(img);
			fclose(f);
			free(file);
			return 1;
		}

        #pragma omp parallel for
        for (unsigned long long int iter = 0; iter < canvas; iter++) {
            if (output[iter] == 0) {
                img[(iter * 3)] = 0;
                img[(iter * 3) + 1] = 0;
                img[(iter * 3) + 2] = 0;
            } else {
                unsigned long long int modulo = (((output[iter]) % *colorCount) - 1) % *colorCount;
                img[(iter * 3)] = colorMap[modulo * 3];
                img[(iter * 3) + 1] = colorMap[modulo * 3 + 1];
                img[(iter * 3) + 2] = colorMap[modulo * 3 + 2];
            }
        }
        fprintf(f, "P6 %llu %llu 255\n", size, size);
        fwrite(img, sizeof(uint8_t), (3 * canvas), f);
        fclose(f);
    }

    return 0;
}