#include "edge_detection/edge_detection.h"

#include <unistd.h>

struct ProgramConfig
{
    char *input_file;
    char *output_file;
    int   warmup_runs;
    int   measure_runs;
    int   verbose_mode;
    bool  use_parallel;
    int   num_threads;
};

void print_usage(const char *program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -i FILE    Input PGM file\n");
    printf("  -o FILE    Output PGM file\n");
    printf("  -w N       Number of warmup runs (default: 0)\n");
    printf("  -m N       Number of measurement runs (default: 1)\n");
    printf("  -t N       Number of OpenMP threads (default: all available "
           "cores)\n");
    printf("  -q         Quiet mode (no logging, default is verbose)\n");
    printf("  -p         Use parallel version (default)\n");
    printf("  -s         Use sequential version\n");
    printf("  -h         Show this help\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s -i input.pgm -o output.pgm                     # Parallel, "
           "all cores\n",
           program_name);
    printf("  %s -i input.pgm -o output.pgm -t 4                # Parallel, 4 "
           "threads\n",
           program_name);
    printf("  %s -i input.pgm -o output.pgm -s                  # Sequential "
           "(1 thread)\n",
           program_name);
    printf("  %s -i in.pgm -o out.pgm -t 8 -w 3 -m 5            # 8 threads, "
           "benchmark\n",
           program_name);
    printf("  %s -i input.pgm -o output.pgm -q -t 2 -w 3 -m 5   # Quiet, 2 "
           "threads\n",
           program_name);
}

struct ProgramConfig parse_arguments(int argc, char *argv[])
{
    struct ProgramConfig config = {.input_file   = NULL,
                                   .output_file  = NULL,
                                   .warmup_runs  = 0,
                                   .measure_runs = 1,
                                   .verbose_mode = 1,
                                   .use_parallel = 1,
                                   .num_threads  = 0};

    int opt;
    while ((opt = getopt(argc, argv, "i:o:w:m:t:qpsh")) != -1)
    {
        switch (opt)
        {
        case 'i': config.input_file = optarg; break;
        case 'o': config.output_file = optarg; break;
        case 'w':
            config.warmup_runs = atoi(optarg);
            if (config.warmup_runs < 0)
            {
                fprintf(stderr, "Error: Warmup runs must be >= 0\n");
                exit(1);
            }
            break;
        case 'm':
            config.measure_runs = atoi(optarg);
            if (config.measure_runs < 1)
            {
                fprintf(stderr, "Error: Measurement runs must be >= 1\n");
                exit(1);
            }
            break;
        case 't':
            config.num_threads = atoi(optarg);
            if (config.num_threads < 1)
            {
                fprintf(stderr, "Error: Number of threads must be >= 1\n");
                exit(1);
            }
            break;
        case 'q': config.verbose_mode = 0; break;
        case 'p': config.use_parallel = true; break;
        case 's': config.use_parallel = false; break;
        case 'h': print_usage(argv[0]); exit(0);
        default: print_usage(argv[0]); exit(1);
        }
    }

    if (!config.input_file)
    {
        fprintf(stderr, "Error: Missing input file\n");
        print_usage(argv[0]);
        exit(1);
    }
    if (!config.output_file)
    {
        fprintf(stderr, "Error: Missing output file\n");
        print_usage(argv[0]);
        exit(1);
    }

    return config;
}

int main(int argc, char *argv[])
{
    struct ProgramConfig config = parse_arguments(argc, argv);
    struct PGMImage      input, raw_edges, output;

    if (!read_pgm_p5(config.input_file, &input))
    {
        printf("Error reading input file\n");
        return 1;
    }

    if (config.verbose_mode)
    {
        printf("\n=== EDGE DETECTION BENCHMARK ===\n");
        printf("Image: %dx%d pixels\n", input.width, input.height);
        printf("Mode: %s\n", config.use_parallel ? "PARALLEL" : "SEQUENTIAL");
        printf("Threads: %d, Warmup runs: %d, Measurement runs: %d\n",
               config.num_threads, config.warmup_runs, config.measure_runs);
        printf("================================\n");
    }
    output.width  = input.width;
    output.height = input.height;
    output.data   = malloc(input.width * input.height);

    raw_edges.width  = input.width;
    raw_edges.height = input.height;
    raw_edges.data   = malloc(input.width * input.height);

    if (config.use_parallel)
    {
        // result = measure_adaptive_multiple_runs(
        //     sobel_edge_detection_parallel, compute_histogram_parallel,
        //     otsu_threshold_parallel, &input, &raw_edges, &output,
        //     config.warmup_runs, config.measure_runs, config.verbose_mode,
        //     "Parallel Adaptive (with sync)");
    }
    else
    {
        benchmark(&input, &raw_edges, &output, config.warmup_runs,
                  config.measure_runs, config.verbose_mode,
                  config.use_parallel);
    }
    // measure_times(sobel_edge_detection_sequential,
    // compute_histogram_sequential,
    //               otsu_threshold_sequential, &input, &raw_edges,
    //               &output, "Sequential Sobel");

    write_pgm_p5(config.output_file, &output);

    free(input.data);
    free(raw_edges.data);
    free(output.data);

    return 0;
}
