# mandelbrot

An attempt to speed up the computation of the Mandelbrot set using OpenMP in C++. Code mass produces bit map images/frames in `.ppm` format and stitches frames using ffmpeg to create movies and gifs!

Code is highly parallelized through the internal Mandelbrot set calculation, as well as in making an single image! Higher precision is enabled through the use of `unsigned long long int`s and `long double`s. SIMD vectorization was also attempted, but it was very hard to try to figure out how to vectorize complex number computation, so it was not implemneted. No benchmarks were performed, but runs quite fast when disabling `#pragma omp parallel for` lines. 

## Compilation and Example Usage:

```sh
mkdir build
cd build
cmake ..
make
./mandelbrot 2 1750 0.37280535024 -0.2551419600000041 2 1e-15 500 50 output/partB defaultcolormap.txt
```

Program argument upon compilation are:
```sh
./mandelbrot <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>
```

```
./mandelbrot 2 1750 0.013438870532012129028364919004019686867528573314565492885548699 0.655614218769465062251320027664617466691295975864786403994151735 2 1e-18 1000 100 output defaultcolormap.txt
```
