/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 3: Helpers                             |
|     Created by Davy Boekhout on 13/09/2017             |
|                                                        |
|     Used by find.c. helpers.c sorts a given array      |
|     and searches for a given value in the sorted       |
|     array                                              |
----------------------------------------------------------
*/

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"


/**
 * Using Binary Search algorithm, returns true if value is in array
 * of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // continue on valid array length
    if (n > 0)
    {
        // index takes on role as the new "zero" when looking at right side
        int index = 0;
        // centr holds value of the array that's currently being looked at
        int centr;

        // loop till we are out of numbers to check
        while (n >= 0)
        {
            // determine the middle value of values[]
            centr = values[n / 2 + index];

            // we're done if we found value
            if (centr == value)
            {
                return true;
            }
            // look to the right of centr in case centr < value
            else if (centr < value)
            {
                index = index + n / 2 + 1;
                n = (n - (n / 2)) - 1;
            }
            // look to the left of centr in case centr > value
            else
            {
                n = (n / 2) - 1;
            }
        }
    }
    // fail if we didn't find the number
    return false;
}

/**
 * Linearly sorts array of n values.
 */
void sort(int values[], int n)
{
    // array that holds occurrences of value in values[]
    int temp[65536] = {0};
    for (int i = 0; i < n; i++)
    {
        temp[values[i]]++;
    }

    // index for values[] and for temp[]
    int j = 0;
    int k = 0;

    // loop until we have processed all integers in values[]
    while (n > 0)
    {
        // if temp[j] == 0 it means corresponding index j is not in values[]
        if (temp[j] == 0)
        {
            j++;
        }
        // put j in values for as long temp[k] > 0
        else
        {
            values[k] = j;
            temp[j]--;
            k++;
            n--;
        }
    }
}
