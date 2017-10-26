/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 2: Vigenere                            |
|     Created by Davy Boekhout on 11/09/2017             |
|                                                        |
|     Using Vigenere cipher, takes a plaintext string    |
|     and returns the ciphertext.                        |
----------------------------------------------------------
*/

#define ALPHA_LENGTH 26

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void encrypt(char *plaintext, char *key);

int main(int argc, char * argv[])
{
    // validate user input
    if (argc == 2)
    {
        // reserve space for the key and loop over every character
        char key[strlen(argv[1])];
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            // determine if the i'th character is valid
            if (!isalpha(argv[1][i]))
            {
                printf("ERROR: Keyword may only consist of alphanumeric characters\n");
                return 1;
            }
            // while input is valid, create the key string
            else
                key[i] = argv[1][i];
        }

        // key is valid so we can retrieve the plaintext
        char plaintext[128];
        printf("plaintext: ");
        fgets(plaintext, sizeof(plaintext), stdin);
        // after retrieving the plaintext, call encrypt() which prints the ciphertext
        if (plaintext[strlen(plaintext) - 1] == '\n')
            encrypt(plaintext,key);
    }
    // quit on mismatching argument count
    else
        return 1;
}

void encrypt(char *plaintext, char *key)
{
    // determine the numerical key shift for every character in key
    int length = strlen(key);
    for (int k = 0; k < length; k++)
    {
        if(isupper(key[k]))
            key[k] = key[k] - 'A';
        else
            key[k] = key[k] - 'a';
    }

    printf("ciphertext: ");

    // declare i for looping over the plaintext, j for looping over the key
    int i = 0;
    int j = 0;
    while (i < strlen(plaintext))
    {
        // determine if i'th character is a letter
        if (isalpha(plaintext[i]))
        {
            // determine if i'th character is upper or lower case
            if (islower(plaintext[i]))
            {
                // shifting the i'th character of the plaintext with the shift of key[j]
                // if plaintext[i] + key[j] is greater than 122, we need to wrap around the alphabet
                if ((plaintext[i] + (key[j % length]) > 'z'))
                {
                    printf("%c", (plaintext[i] + (key[j % length]) - ALPHA_LENGTH));
                    j++;
                }
                // if plaintext[i] + key[j] does not require wrapping around, continue
                else
                {
                    printf("%c", (plaintext[i] + (key[j % length])));
                    j++;
                }
            }
            // same rules apply as before, in this case for uppercase letters
            else if (isupper(plaintext[i]))
            {
                if ((plaintext[i] + (key[j % length]) > 'Z'))
                {
                    printf("%c", (plaintext[i] + (key[j % length]) - ALPHA_LENGTH));
                    j++;
                }
                else
                {
                    printf("%c", (plaintext[i] + (key[j % length])));
                    j++;
                }
            }
        }
        // print every character that does not match the criteria above
        else
            printf("%c", plaintext[i]);
        i++;
    }
}