/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 2: Initials (more)                     |
|     Created by Davy Boekhout on 7/09/2017              |
|                                                        |
|     Takes a name as input and gives back the initials  |                                  |
----------------------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Get a name from the user
    string name = get_string();
    for (int i = 0, n = strlen(name); i < n; i++ )
    {
        // If current character is a space, skip it
        if (name[i] == ' ')
            printf("");

        // If the current character is a letter and the previous one was
        // a space, we can assume we are dealing with an initial
        else if (name[i] != ' ' && name[i - 1] == ' ')
           printf("%c", toupper(name[i]));

        // If name[0] is a letter, we can assume it's the first initial
        else if (i == 0 && name[i] != ' ')
            printf("%c", toupper(name[i]));
    }
    printf("\n");
}
