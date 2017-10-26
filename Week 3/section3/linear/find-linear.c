/**
 * Prompts user for as many as MAX values until EOF is reached,
 * then proceeds to search that "haystack" of values for given needle.
 *
 * Usage: ./find needle
 *
 * where needle is the value to find in a haystack of values
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

bool search(int value, int values[], int size);
void print_array(int values[], int size);

// maximum amount of hay
const int MAX = 65536;

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./find needle\n");
        return -1;
    }

    // remember needle
    int needle = atoi(argv[1]);

    // fill haystack
    int size;
    int haystack[MAX];
    for (size = 0; size < MAX; size++)
    {
        // wait for hay until EOF
        printf("\nhaystack[%i] = ", size);
        int straw = get_int();
        if (straw == INT_MAX)
        {
            break;
        }

        // add hay to stack
        haystack[size] = straw;
    }
    printf("\n");

    // show haystack
    printf("---------------------------------------------------------\n");
    printf("I will try to find needle \"%d\" in the following haystack:\n", needle);
    print_array(haystack, size);

    // try to find needle in haystack
    if (search(needle, haystack, size))
    {
        printf("\nFound needle in haystack!\n\n");
        return 0;
    }
    else
    {
        printf("\nDidn't find needle in haystack.\n\n");
        return 1;
    }
}

void print_array(int values[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d\n", values[i]);
    }
    return;
}

bool search(int value, int values[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (values[i] == value)
        {
            return true;
        }
    }
    return false;
}
