/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 1: Mario                               |
|     Created by Davy Boekhout on 5/09/2017              |
|                                                        |
|     Draws a pyramid using hashes from the mario game   |
|     where the height depends on the users input.       |
|                                                        |
----------------------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>

int main (void)
{
    // Get the height of the pyramid the user wants (minimal of 1, max of 23)
    int height = -1;
    while (height < 0 || height > 23)
    {
        printf("Height: ");
        height = get_int();

        int p;
        int sizeofrow = height + 1;
        int spaces = height - 2;
        int hashes = 1;

        // We need to print p amount of rows to complete the pyramid
        for (p = height; p > 0; p--)
        {
            // Use width of the pyramid to print the spaces and hashtags on the left side
            for (int i = sizeofrow - hashes; i > 1; i--)
                printf(" ");
            for (int o = sizeofrow - spaces; o > 2; o--)
                printf("#");

            // Print the middle gap of the pyramid
            printf("  ");

            // Use width of the pyramid to print the hashtags on the right side
            for (int o = sizeofrow - spaces; o > 2; o--)
                printf("#");

            // Decrement spaces for the next round and increment hashes for the next round, then print a newline
            spaces--;
            hashes++;
            printf("\n");
        }
    }
}