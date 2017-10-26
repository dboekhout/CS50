/**
 * Implements the change image functionality.
 */

#include "censor.h"

/**
 * Processes a given pixel (triple) and return the new one.
 */
RGBTRIPLE process_pixel(RGBTRIPLE old_triple, int y, int x)
{
    RGBTRIPLE new_triple = old_triple;

    if ((y > 83 && y < 104) && (x > 170 && x < 255))
    {
        new_triple.rgbtBlue = 0;
        new_triple.rgbtRed = 0;
        new_triple.rgbtGreen = 0;
    }

    return new_triple;
}