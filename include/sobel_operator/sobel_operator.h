#ifndef SOBEL_OPERATOR_H
#define SOBEL_OPERATOR_H

#include <stdlib.h>
#include <string.h>

#include "pgm/pgm.h"

static const int GX[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

static const int GY[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    { 1,  2,  1}
};

void sobel_edge_detection_sequential(const struct PGMImage *input,
                                     struct PGMImage       *output,
                                     const int              threshold);
void sobel_edge_detection_parallel(const struct PGMImage *input,
                                   struct PGMImage       *output,
                                   const int              threshold);

void compute_histogram_sequential(const struct PGMImage *image,
                                  int                    histogram[256]);
void compute_histogram_parallel(const struct PGMImage *image,
                                int                    histogram[256]);

int otsu_threshold(const int histogram[256], const size_t total_pixels);

#endif
