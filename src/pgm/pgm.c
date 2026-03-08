#include "pgm/pgm.h"

int read_pgm_p5(const char *filename, struct PGMImage *image)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        return 0;
    }

    char magic_number[3];
    fscanf(fp, "%2s\n", magic_number);
    if (strcmp(magic_number, "P5") != 0)
    {
        return 0;
    }

    // skip comments
    char c = fgetc(fp);
    while (c == '#')
    {
        while (fgetc(fp) != '\n');
        c = fgetc(fp);
    }
    ungetc(c, fp);

    fscanf(fp, "%d %d\n255\n", &image->width, &image->height);

    image->data = malloc(image->width * image->height);
    fread(image->data, 1, image->width * image->height, fp);

    fclose(fp);

    return 1;
}

void write_pgm_p5(const char *filename, struct PGMImage *image)
{
    FILE *fp = fopen(filename, "wb");
    fprintf(fp, "P5\n%d %d\n255\n", image->width, image->height);
    fwrite(image->data, 1, image->width * image->height, fp);
    fclose(fp);
}
