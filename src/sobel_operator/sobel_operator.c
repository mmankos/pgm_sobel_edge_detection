#include "sobel_operator/sobel_operator.h"

void sobel_edge_detection_sequential(struct PGMImage *input,
                                     struct PGMImage *output, int threshold)
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
