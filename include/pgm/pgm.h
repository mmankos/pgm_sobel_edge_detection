#ifndef PGM_H
#define PGM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PGMImage
{
    int            width;
    int            height;
    unsigned char *data;
};

int  read_pgm_p5(const char *filename, struct PGMImage *image);
void write_pgm_p5(const char *filename, struct PGMImage *image);

#endif
