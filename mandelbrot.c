// ANDREA TONELLO SM3201234

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <omp.h>

#include "pgm.h"
#include "mandelbrot.h"


// function f(z) = z^2 + c
double complex func (double complex z, double complex c) {
    return (z*z) + c;
}


// calculates (z = a + ib)'s module, i.e. |z| = sqrt(a^2 + b^2)
double module (double complex c) {
    double a = creal(c);
    double b = cimag(c);
    return sqrt(a*a + b*b);
}


// To establish whether c is or isn't in the Mandelbrot set, we return the appropriate value of n
int is_mandelbrot (int M, double complex c) {

    int r = 2;
    double complex f_c = c;

// *** cannot use OpenMP with in-loop break or return statements ***
    // testing f(c), f(f(c)), f(f(f(c)))... and their respective modules
    for (int n = 1; n <= M; n++) {

        if (module(f_c) >= r) {
            return n;
        }

        // when calculating f(c) = c^2 + c, this last "+ c" term is a constant,
        // so only f_c gets updated, while c does not change
        f_c = func(f_c, c);
    }
    return M;
}


// This function updates an empty image with the appropriate grey level for every c = a + ib in the given ranges:
//     Real range: [-2, 1] -> this means that a is in [-2, 1]
//     Imaginary range: [-i, i] -> this means that b is in [-1, 1]
// Obviously "every c" is a stretch, because we are limited by the image resolution
void mandelbrot_set (netpbm_ptr img, int M) {

    // the appropriate step size is calculated using the image resolution
    double real_step_size = 3.0 / img->width;      // |1 - (-2)| = 3
    double imag_step_size = 2.0 / img->height;     // |1 - (-1)| = 2

    double log_M = log(M);

#pragma omp parallel for collapse(2)
    for (int y = 0; y < img->height; y++) {
        
        for (int x = 0; x < img->width; x++) {

            // We initialize a = -2 and b = 1; this means we are starting from the top-left corner of the image,
            // since we access it via a row-major structure
            double b = 1.0 - y*imag_step_size;
            double a = -2.0 + x*real_step_size;
            double complex c = a + b*I;
            
            int n = is_mandelbrot(M, c);           // getting the number of iterations n for current c
            char * pixel = pixel_at(img, x, y);    // preparing the associated pixel on the grid   

            if (n == M) {                          // if n == M, c is in the Mandelbrot set -> the color is set to white
                * pixel = 255;
            } else {                               // if n < M, c isn't in the Mandelbrot set -> we need to calculate the appropriate color
                int intensity = 255 * (log(n) / log_M);
                * pixel = intensity;
            }
        }
    }
}