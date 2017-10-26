/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Section 2: Strings                                 |
|     Created by Davy Boekhout & Mark Pijnenburg         |
|     on 8/09/2017                                       |
|                                                        |
|     Takes a string from the user and prints every      |
|     other character                                    |
|                                                        |
----------------------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    // Get input from the user
    printf("Type something: ");
    string s = get_string();

    // Loop over each character, print if i is even
    for(int i = 0, n = strlen(s); i < n; i += 2)
        printf("%c", s[i]);
    printf("\n");
}