/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 4: recover                             |
|     Created by Davy Boekhout on 21/09/2017             |
|                                                        |
|     Recovers JPEGs from a forensic image.              |
|     NOTE: This tool recovers data 512 bytes at a time. |
|     Slack space may be included when recovering an     |
|     image.                                             |
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover raw_image\n");
        return 1;
    }

    // open image file
    FILE *raw_image = fopen(argv[1], "r");
    if (raw_image == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // store the block size used in the forensic image
    unsigned int block_size = 512;

    // buffer to store the JPEG data in (512 bytes at a time)
    unsigned char *buffer;
    buffer = malloc(sizeof(unsigned char) * block_size);

    // keep track of images found
    int found_jpeg = 0;
    int img_count = 0;

    // generate image filename, starting with 000.jpg
    char filename[sizeof "000.jpg"];
    sprintf(filename, "%03i.jpg", img_count);

    // create the new JPEG
    FILE *img_curr = fopen(filename, "w");
    if (img_curr == NULL)
    {
        fclose(raw_image);
        fprintf(stderr, "Could not write %s.\n", filename);
        return 3;
    }

    // read raw_image till EOF
    while (fread(buffer, block_size, 1, raw_image))
    {
        // look for the JPEG file header in every block
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // header is found and an image is currently being written
            if (found_jpeg)
            {
                // close current image
                fclose(img_curr);

                // increment filename
                img_count += 1;
                sprintf(filename, "%03i.jpg", img_count);

                // write buffer to new JPEG
                img_curr = fopen(filename, "w");
                fwrite(buffer, block_size, 1, img_curr);
            }
            // header found, nothing being written; create first image
            else
            {
                found_jpeg = 1;
                fwrite(buffer, block_size, 1, img_curr);
            }
        }
        // if currently writing image, continue, else skip current block
        else
        {
            if (found_jpeg)
            {
                fwrite(buffer, block_size, 1, img_curr);
            }
        }
    }

    // clean up
    fclose(img_curr);
    free(buffer);

    return 0;
}
