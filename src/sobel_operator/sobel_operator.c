#include "sobel_operator/sobel_operator.h"

void sobel_edge_detection_sequential(const struct PGMImage *input,
                                     struct PGMImage       *output,
                                     const int              threshold)
{
    memset(output->data, 0, output->width * output->height);

    for (int y = 1; y < input->height - 1; y++)
    {
        for (int x = 1; x < input->width - 1; x++)
        {
            int sumx = 0, sumy = 0;

            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    unsigned char pixel =
                        input->data[(y + ky) * input->width + (x + kx)];
                    sumx += pixel * GX[ky + 1][kx + 1];
                    sumy += pixel * GY[ky + 1][kx + 1];
                }
            }

            int magnitude = abs(sumx) + abs(sumy);
            if (magnitude >= threshold)
            { // clamp to max 255
                output->data[y * output->width + x] =
                    magnitude > 255 ? 255 : magnitude;
            }
            else
            { // ignore weak edges
                output->data[y * output->width + x] = 0;
            }
        }
    }
}

void sobel_edge_detection_parallel(const struct PGMImage *input,
                                   struct PGMImage *output, const int threshold)
{}

void compute_histogram_sequential(const struct PGMImage *image,
                                  int                    histogram[256])
{
    for (int i = 0; i < 256; i++) { histogram[i] = 0; }

    for (int i = 0; i < image->width * image->height; i++)
    {
        histogram[image->data[i]]++;
    }
}

void compute_histogram_parallel(const struct PGMImage *image,
                                int                    histogram[256])
{}

int otsu_threshold(const int histogram[256], const size_t total_pixels)
{
    unsigned long long sum = 0;
    for (int i = 0; i < 256; i++)
    {
        sum += (unsigned long long)i * histogram[i];
    }

    unsigned long long sum_background    = 0;
    int                weight_background = 0;
    int                weight_foreground = 0;
    double             max_variance      = 0;
    int                optimal_threshold = 0;

    for (int t = 0; t < 256; t++)
    {
        weight_background += histogram[t];
        if (weight_background == 0)
        {
            continue;
        }

        weight_foreground = total_pixels - weight_background;
        if (weight_foreground == 0)
        {
            break;
        }

        sum_background += (unsigned long long)t * histogram[t];

        double mean_background = (double)sum_background / weight_background;
        double mean_foreground =
            (double)(sum - sum_background) / weight_foreground;

        double variance = (double)weight_background *
                          (double)weight_foreground *
                          (mean_background - mean_foreground) *
                          (mean_background - mean_foreground);

        if (variance > max_variance)
        {
            max_variance      = variance;
            optimal_threshold = t;
        }
    }

    return optimal_threshold;
}
