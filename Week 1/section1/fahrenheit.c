//
/*
-----------------------------------------------
|                                             |
|     Section 1: fahrenheit                   |
|     Created by                              |
|     Davy Boekhout & Mark Pijnenburg         |
|     on 5/09/2017                            |
|                                             |
-----------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int celcius = 0;
    while (celcius > -274){
        celcius = get_int();
        printf("C: %i\n", celcius);
        printf("F: %.1f\n", (float) (((celcius * 9 )/ 5) + 32));
    }
}