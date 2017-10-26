/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 4: resize                              |
|     Created by Davy Boekhout on 19/09/2017             |
|                                                        |
|     Enlarges 24-bit uncompressed BMPs                  |
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy resize_factor infile outfile\n");
        return 1;
    }

    // get resize scale
    int rfactor = atoi(argv[1]);

    // check rfactor bounds
    if (rfactor > 100 || rfactor < 1)
    {
        fprintf(stderr, "Resize factor must be in the range of 0-100.\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // keep track of the padding in the input and output file
    int padding_old = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_new = (4 - ((bi.biWidth * rfactor) * sizeof(RGBTRIPLE))
                       % 4) % 4;

    // save the width and height of the input file
    int width_old = bi.biWidth;
    int height_old = abs(bi.biHeight);

    // determine backwards seek for recopy
    int rgb_size = sizeof(RGBTRIPLE);
    int go_back = width_old * rgb_size + padding_old;

    // update header info
    bi.biWidth = bi.biWidth * rfactor;
    bi.biHeight = bi.biHeight * rfactor;
    bi.biSizeImage = ((bi.biWidth * bi.biWidth) * sizeof(RGBTRIPLE)) +
                     (padding_new * bi.biWidth);
    bf.bfSize = bi.biSizeImage + sizeof(bi) + sizeof(bf);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // resize input file using the recopy method
    for (int i = 0; i < height_old; i++)
    {
        // repeat each row n - 1 times
        for (int j = 0; j < rfactor - 1; j++)
        {
            // iterate over each pixel
            for (int k = 0; k < width_old; k++)
            {
                RGBTRIPLE triple;

                // read 3 bytes (1 pixel)
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write pixel * rfactor in output file
                for (int l = 0; l < rfactor; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip padding in the input file
            fseek(inptr, padding_old, SEEK_CUR);

            // write new padding to output file
            for (int k = 0; k < padding_new; k++)
            {
                fputc(0x00, outptr);
            }

            // go back to beginning of line in input file we just processed
            fseek(inptr, go_back * -1, SEEK_CUR);

        }

        // resize input file using the recopy method, now horizontally
        for (int j = 0; j < width_old; j++)
        {
            RGBTRIPLE triple;

            // read 3 bytes (1 pixel)
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // repeat every pixel * rfactor
            for (int k = 0; k < rfactor; k++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // skip padding in the input file
        fseek(inptr, padding_old, SEEK_CUR);

        // write new padding to output file
        for (int j = 0; j < padding_new; j++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
