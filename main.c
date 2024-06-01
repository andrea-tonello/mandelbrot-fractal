// ANDREA TONELLO SM3201234

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "pgm.h"
#include "mandelbrot.h"

int main(int argc, char * argv[])
{

// Initializing user input
if (argc != 4) {
    printf("Must enter filename, max iterations and vertical resolution!\n");
    return 1;
}
char * filename = argv[1];
int M = atoi(argv[2]);
int nrows = atoi(argv[3]);
int ncols = 1.5*nrows;;

// File extension
strcat(filename, ".pgm");

// checking for strictly positive integer
if (M <= 0) {
    printf("The number of iterations must be a strictly positive integer!\n");
    return 2;
}
if (nrows <= 0) {
    printf("The vertical resolution must be a strictly positive integer!\n");
    return 2;
}

// other resolution checks
if (nrows >= 10000 && nrows <= 20000)
    printf("Warning: with the selected resolution, the final image will take a considerable amount of time to process and space to be stored.\n");
if (nrows > 20000) {
    printf("Selected vertical resolution is too big; it must be <= 20000.\n");
    return 3;
}

printf("\nFile name: %s, with resolution %d x %d and precision given by M = %d\n", filename, ncols, nrows, M);


// Image initialization
netpbm image;

int img_err = make_empty_image(filename, &image, ncols, nrows);
if (img_err != 0) {
    printf("Unable to make the image: %d\n", -img_err);
    return 3;
}

mandelbrot_set(&image, M);

close_image(&image);

return 0;
}