// ANDREA TONELLO SM3201234

#ifndef _MANDELBROT_H
#define _MANDELBROT_H

#include <stdio.h>
#include <complex.h>

#include "pgm.h"

double complex func(double complex c, double complex og_c);

double module (double complex c);

int is_mandelbrot (int M, double complex c);

void mandelbrot_set (netpbm_ptr, int M);

#endif