// ANDREA TONELLO SM3201234

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "pgm.h"

// These functions were seen in class and are found within Lecture 16's code


// Filling the netpbm image struct
int open_image(char * path, netpbm_ptr img)
{
    img->fd = fopen(path, "r+");
    if (img->fd == NULL) {
        return -1;
    }

    struct stat sbuf;
    stat(path, &sbuf);
    img->size = sbuf.st_size;

    // Checking netpbm grey-scale format
    if (fscanf(img->fd, "P5\n%d %d\n255\n", &img->width, &img->height) != 2) {
        fclose(img->fd);
        return -2;
    }

    img->offset = ftell(img->fd);
    img->data = mmap((void *)0, img->size, PROT_WRITE, MAP_SHARED, fileno(img->fd), 0);    // we just need to write on an empty image
    if (img->data == MAP_FAILED) {
        fclose(img->fd);
        return -3;
    }
    return 0;
}


// Makes an empty image
int make_empty_image(char * path, netpbm_ptr img, int width, int height)
{
    // File creation
    FILE * fd = fopen(path, "w+");
    if (fd == NULL) {
        return -1;
    }

    int written = fprintf(fd, "P5\n%d %d\n255\n", width, height);    // requirements for a grey-scale netpbm image
    ftruncate(fileno(fd), written + width * height);                 // "Filling" the image with blank space
    fclose(fd);
    return open_image(path, img);
}


char * pixel_at(netpbm_ptr img, int x, int y)
{
    if (img == NULL) {
        return NULL;
    }
    if (x < 0 || x >= img->width) {
        return NULL;
    }
    if (y < 0 || y >= img->height) {
        return NULL;
    }
    return &img->data[y * img->width + x + img->offset];    // row-major access
}


int close_image(netpbm_ptr img)
{
    if (img == NULL) {
        return -1;
    }
    munmap(img->data, img->size);
    fclose(img->fd);
    return 0;
}