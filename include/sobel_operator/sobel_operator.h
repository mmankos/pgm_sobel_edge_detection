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

void sobel_edge_detection_sequential(struct PGMImage *input,
                                     struct PGMImage *output, int threshold);

#endif
