/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // argument validation, need at least argv[1], optionally argv[2]
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // convert the given n from a character to an integer
    int n = atoi(argv[1]);

    // in case argv[2] is provided, convert it and use it as a seed value
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    // otherwise use time() to generate the pseudorandom numbers
    else
    {
        srand48((long) time(NULL));
    }

    // given n, generate positive pseudorandom integers
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
