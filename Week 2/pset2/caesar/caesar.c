/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 2: Caesar                              |
|     Created by Davy Boekhout on 8/09/2017              |
|                                                        |
|     Using caesar cipher, takes a plaintext string      |
|     and returns the ciphertext.                        |
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    // validate user input
    if (argc == 2 && atoi(argv[1]) >= 0)
    {
        // convert argument to integer so we can use it as a key
        int key = atoi(argv[1]);

        // if key is bigger than 65, use modulo to get the lower equivalent
        if (key > 26)
            key = key % 26;

        // get plaintext from the user, up to 128 characters
        printf("plaintext: ");
        char plaintext[128];
        fgets(plaintext, sizeof(plaintext), stdin);

        // if fgets is done receiving input from stdin, continue
        if (plaintext[strlen(plaintext) - 1] == '\n')
        {
            // we can start printing the cipertext, looping over the plaintext
            printf("ciphertext: ");
            for(int j = 0, n = strlen(plaintext) - 1; j < n; j++)
            {
                // check if j is an uppercase or lowercase character
                if (isupper(plaintext[j]))
                {
                    // if no wrapping is required, print character + key
                    if ((plaintext[j] + key) <= 'Z')
                        printf("%c", (plaintext[j] + key));
                    // substract 26 if we need to wrap the alphabet
                    else
                        printf("%c", ((plaintext[j] + key) - 26));
                }
                else if (islower(plaintext[j]))
                {
                    // similar code for lowercase characters
                    if ((plaintext[j] + key) <= 'z')
                        printf("%c", (plaintext[j] + key));
                    else
                        printf("%c", ((plaintext[j] + key) - 26));
                }
                // leave special characters as is
                else
                    printf("%c", plaintext[j]);
            }
            printf("\n");
        }
    }
    else
        return 1;
}