/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 1: Water                               |
|     Created by Davy Boekhout on 5/09/2017              |
|                                                        |
|     Converts the amount of minutes that you shower     |
|     into bottles of water                              |
|                                                        |
----------------------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>

int main (void)
{
    printf("Enter the length of your shower in minutes: ");
    int minutes = get_int();
    printf("Minute(s): %i\n", minutes);
    printf("Bottles: %i\n", (192 /16) * minutes);
}