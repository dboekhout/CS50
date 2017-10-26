/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 1: Greedy                              |
|     Created by Davy Boekhout on 6/09/2017              |
|                                                        |
|     Calculates the minimal amount of coins that        |
|     are needed to give back change                     |
----------------------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change = 0;
    // Get non-negative change from the user
    while (change <= 0)
    {
        printf("O hai! How much change is owed?\n");
        change = get_float();
    }

    int change_owed = round(change * 100);
    int count = 0;

    while (change_owed != 0)
    {
        if (change_owed > 24)
        {
            count++;
            change_owed = change_owed - 25;
        }
        else if (change_owed > 9)
        {
            count++;
            change_owed = change_owed - 10;
        }
        else if (change_owed > 4)
        {
            count++;
            change_owed = change_owed - 5;
        }
        else
        {
            count++;
            change_owed = change_owed - 1;
        }
    }
    printf("%d\n", count);
}