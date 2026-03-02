#include "sobel_operator/sobel_operator.h"
#include "time_measurement/time_measurement.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s input.pgm output.pgm\n", argv[0]);
        return 1;
    }

    struct PGMImage input, output_seq;
    int             threshold = 0;

    if (!read_pgm_p5(argv[1], &input))
    {
        printf("Error reading input file\n");
        return 1;
    }

    output_seq.width  = input.width;
    output_seq.height = input.height;
    output_seq.data   = malloc(input.width * input.height);

    measure_times(sobel_edge_detection_sequential, &input, &output_seq,
                  threshold, "Sequential Sobel");

    write_pgm_p5(argv[2], &output_seq);

    free(input.data);
    free(output_seq.data);

    return 0;
}
