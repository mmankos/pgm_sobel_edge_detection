#ifndef TIME_MEASUREMENT_H
#define TIME_MEASUREMENT_H

#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "pgm/pgm.h"
#include "sobel_operator/sobel_operator.h"

struct EdgeDetectionResult
{
    int    threshold;
    int    histogram[256];
    size_t total_pixels;
};

struct TimeMeasurementResult
{
    double cpu_time;
    double wall_time;
};

struct PipelineResult
{
    struct EdgeDetectionResult   edge_detection_result;
    struct TimeMeasurementResult time_measurement_result;
};

typedef void (*SobelFunction)(const struct PGMImage *, struct PGMImage *,
                              const int);
typedef void (*ComputeHistogramFunction)(const struct PGMImage *, int[256]);

struct EdgeDetectionResult
edge_detection_pipeline(SobelFunction            sobel_function,
                        ComputeHistogramFunction compute_histogram_function,
                        const struct PGMImage   *input,
                        struct PGMImage *raw_edges, struct PGMImage *output);

struct PipelineResult
measured_run(SobelFunction            sobel_function,
             ComputeHistogramFunction compute_histogram_function,
             const struct PGMImage *input, struct PGMImage *raw_edges,
             struct PGMImage *output);

void benchmark(const struct PGMImage *input, struct PGMImage *raw_edges,
               struct PGMImage *output, const int warmup_runs,
               const int measure_runs, const int verbose_mode,
               const bool is_parallel);

int count_pixels_above_threshold(const int histogram[256], const int threshold);

#endif
