#include "time_measurement/time_measurement.h"

void measure_times(SobelFunction sobel_func, struct PGMImage *input,
                   struct PGMImage *output, int threshold,
                   const char *function_name)
{
    struct timeval wall_start, wall_end;
    clock_t        cpu_start, cpu_end;

    gettimeofday(&wall_start, NULL);
    cpu_start = clock();

    sobel_func(input, output, threshold);

    cpu_end = clock();
    gettimeofday(&wall_end, NULL);

    double cpu_time  = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;
    double wall_time = (wall_end.tv_sec - wall_start.tv_sec) +
                       (wall_end.tv_usec - wall_start.tv_usec) / 1e6;

    printf("\n--- %s ---\n", function_name);
    printf("  CPU time:  %.4f seconds (processor time)\n", cpu_time);
    printf("  Wall time: %.4f seconds (real time)\n", wall_time);
    printf("  CPU/Wall ratio: %.2f\n", cpu_time / wall_time);
}
