#include "edge_detection/edge_detection.h"

struct EdgeDetectionResult
edge_detection_pipeline(SobelFunction            sobel_function,
                        ComputeHistogramFunction compute_histogram_function,
                        const struct PGMImage   *input,
                        struct PGMImage *raw_edges, struct PGMImage *output)
{
    struct EdgeDetectionResult edge_detection_result;

    edge_detection_result.threshold = 0;
    sobel_function(input, raw_edges, edge_detection_result.threshold);

    compute_histogram_function(raw_edges, edge_detection_result.histogram);

    edge_detection_result.total_pixels = raw_edges->width * raw_edges->height;
    edge_detection_result.threshold    = otsu_threshold(
        edge_detection_result.histogram, edge_detection_result.total_pixels);

    sobel_function(input, output, edge_detection_result.threshold);

    return edge_detection_result;
}

struct PipelineResult
measured_run(SobelFunction            sobel_function,
             ComputeHistogramFunction compute_histogram_function,
             const struct PGMImage *input, struct PGMImage *raw_edges,
             struct PGMImage *output)
{
    struct PipelineResult pipeline_result;

    struct timeval wall_start, wall_end;
    clock_t        cpu_start, cpu_end;

    gettimeofday(&wall_start, NULL);
    cpu_start = clock();

    pipeline_result.edge_detection_result = edge_detection_pipeline(
        sobel_function, compute_histogram_function, input, raw_edges, output);

    cpu_end = clock();
    gettimeofday(&wall_end, NULL);

    pipeline_result.time_measurement_result.cpu_time =
        ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;
    pipeline_result.time_measurement_result.wall_time =
        (wall_end.tv_sec - wall_start.tv_sec) +
        (wall_end.tv_usec - wall_start.tv_usec) / 1e6;

    return pipeline_result;
}

void benchmark(const struct PGMImage *input, struct PGMImage *raw_edges,
               struct PGMImage *output, const int warmup_runs,
               const int measure_runs, const int verbose_mode,
               const bool is_parallel)
{
    struct PipelineResult pipeline_result;

    double total_cpu  = 0;
    double total_wall = 0;

    int num_threads = 1;

    SobelFunction sobel_function = is_parallel
                                       ? sobel_edge_detection_parallel
                                       : sobel_edge_detection_sequential;

    ComputeHistogramFunction compute_histogram_function =
        is_parallel ? compute_histogram_parallel : compute_histogram_sequential;

    for (int run = 0; run < warmup_runs; run++)
    {
        if (run == 0 && verbose_mode)
        {
            printf("WARMUP RUNS:\n");
        }

        pipeline_result =
            measured_run(sobel_function, compute_histogram_function, input,
                         raw_edges, output);

        if (verbose_mode)
        {
            printf("    Run %2d: CPU=%7.4f, Wall=%7.4f\n", run + 1,
                   pipeline_result.time_measurement_result.cpu_time,
                   pipeline_result.time_measurement_result.wall_time);
        }
    }

    for (int run = 0; run < measure_runs; run++)
    {
        if (run == 0 && verbose_mode)
        {
            printf("MEASURED RUNS:\n");
        }

        pipeline_result =
            measured_run(sobel_function, compute_histogram_function, input,
                         raw_edges, output);

        total_cpu += pipeline_result.time_measurement_result.cpu_time;
        total_wall += pipeline_result.time_measurement_result.wall_time;

        if (verbose_mode)
        {
            printf("    Run %2d: CPU=%7.4f, Wall=%7.4f\n", run + 1,
                   pipeline_result.time_measurement_result.cpu_time,
                   pipeline_result.time_measurement_result.wall_time);
        }
    }

    int pixels_above_threshold = count_pixels_above_threshold(
        pipeline_result.edge_detection_result.histogram,
        pipeline_result.edge_detection_result.threshold);

    double cpu_time_avg  = total_cpu / measure_runs;
    double wall_time_avg = total_wall / measure_runs;

    if (verbose_mode)
    {
        printf("TIME ANALYSIS:\n");
        printf("  Average (%d threads): CPU=%7.4f, Wall=%7.4f\n", num_threads,
               cpu_time_avg, wall_time_avg);
        printf("  CPU/Wall ratio: %.2f\n", cpu_time_avg / wall_time_avg);
        printf("HISTOGRAM ANALYSIS:\n");
        printf("  Total edge pixels: %zu\n",
               pipeline_result.edge_detection_result.total_pixels);
        printf("  Threshold chosen: %d\n",
               pipeline_result.edge_detection_result.threshold);
        printf("  Pixels above threshold: %d (%.1f%%)\n",
               pixels_above_threshold,
               100.0 * pixels_above_threshold /
                   pipeline_result.edge_detection_result.total_pixels);
    }
}

int count_pixels_above_threshold(const int histogram[256], const int threshold)
{
    int count = 0;
    for (int i = threshold; i < 256; i++) { count += histogram[i]; }

    return count;
}
