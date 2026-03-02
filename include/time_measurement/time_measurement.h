#ifndef TIME_MEASUREMENT_H
#define TIME_MEASUREMENT_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "pgm/pgm.h"

typedef void (*SobelFunction)(struct PGMImage *, struct PGMImage *, int);

void measure_times(SobelFunction sobel_func, struct PGMImage *input,
                   struct PGMImage *output, int threshold,
                   const char *function_name);

#endif
