/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 2: Crack                               |
|     Created by Davy Boekhout on 11/09/2017             |
|                                                        |
|     Takes a hash and cracks the password               |
|                                                        |
|     PLEASE NOTE: This tool uses a dictionary attack    |
|     to crack the given hash. I've generated a wordlist |
|     containing all possible combinations ranging in    |
|     length 1 to 4, consisting of lower- and uppercase  |
|     characters. It's included in the zip file          |
|     called lower_uppercase_wordlist.txt.               |
----------------------------------------------------------
*/

#define _GNU_SOURCE
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        // store the given hash and the salt
        char *hash = argv[1];
        char *salt = "50";

        // get ready to read the wordlist
        FILE *fp;
        fp = fopen("lower_uppercase_wordlist.txt", "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        // while we are reading files
        while ((read = getline(&line, &len, fp)) != -1)
        {
            // strip away the newline to match the hashed password
            if (line[read - 1] == '\n')
            {
                line[read - 1] = '\0';

                // compare the crypted line from the wordlist with the given hash
                if(strcmp(crypt(line, salt), hash) == 0)
                {
                    printf("%s\n", line);
                    break;
                }
            }
        }
    }
    // exit if arguments are not valid
    else
    {
        printf("Usage: ./crack <hash>\n");
        return 1;
    }
}