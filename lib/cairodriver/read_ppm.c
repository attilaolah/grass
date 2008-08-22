#include "cairodriver.h"

void cairo_read_ppm(void)
{
    char *mask_name = G_store(ca.file_name);
    FILE *input, *mask;
    int x, y;
    int i_width, i_height, maxval;

    input = fopen(ca.file_name, "rb");
    if (!input)
	G_fatal_error("cairo: couldn't open input file %s", ca.file_name);

    if (fscanf(input, "P6 %d %d %d", &i_width, &i_height, &maxval) != 3)
	G_fatal_error("cairo: invalid input file %s", ca.file_name);

    fgetc(input);

    if (i_width != ca.width || i_height != ca.height)
	G_fatal_error
	    ("cairo: input file has incorrect dimensions: expected: %dx%d got: %dx%d",
	     ca.width, ca.height, i_width, i_height);

    mask_name[strlen(mask_name) - 2] = 'g';

    input = fopen(mask_name, "rb");
    if (!input)
	G_fatal_error("cairo: couldn't open input mask file %s", mask_name);

    if (fscanf(input, "P5 %d %d %d", &i_width, &i_height, &maxval) != 3)
	G_fatal_error("cairo: invalid input mask file %s", mask_name);

    fgetc(input);

    if (i_width != ca.width || i_height != ca.height)
	G_fatal_error
	    ("cairo: input mask file has incorrect dimensions: expected: %dx%d got: %dx%d",
	     ca.width, ca.height, i_width, i_height);

    G_free(mask_name);

    for (y = 0; y < ca.height; y++) {
	unsigned int *row = (unsigned int *)(ca.grid + y * ca.stride);

	for (x = 0; x < ca.width; x++) {
	    int r = fgetc(input);
	    int g = fgetc(input);
	    int b = fgetc(input);
	    int a = fgetc(mask);

	    r = r * 255 / maxval;
	    g = g * 255 / maxval;
	    b = b * 255 / maxval;
	    a = a * 255 / maxval;

	    if (a > 0 && a < 0xFF) {
		r = r * a / 0xFF;
		g = g * a / 0xFF;
		b = b * a / 0xFF;
	    }

	    row[x] = (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}
    }

    fclose(input);
    fclose(mask);
}
