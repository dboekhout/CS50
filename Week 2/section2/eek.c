/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Section 2: Strings                                 |
|     Created by Davy Boekhout & Mark Pijnenburg         |
|     on 8/09/2017                                       |
|                                                        |
|     Takes a string from the user and prints the number |
|     of e's                                             |
|                                                        |
----------------------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Get input from the user
    printf("Type something: ");
    string s = get_string();

    // Store amount of e's
    int e = 0;

    // Loop over each character, if it's an e, increment counter
    for(int i = 0, n = strlen(s); i < n; i++)
    {
        if (tolower(s[i]) == 'e')
            e++;
    }
    printf("The text \"%s\" contains %d e's.\n", s, e);
}