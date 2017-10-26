#!/usr/bin/env python3
# ----------------------------------------------------------
# |     University of Amsterdam: Minor Programmeren        |
# |     Problem Set 6: Vigenere (less)                     |
# |     Created by Davy Boekhout on 09/10/2017             |
# |                                                        |
# |     Using Vigenere cipher, takes a plaintext string    |
# |     and returns the ciphertext.                        |
# ----------------------------------------------------------

import sys

ALPHA_LENGTH = 26


def encrypt(text, keyword):
    """Take in plaintext and return ciphertext using vigenere cipher"""
    # list comprehension creates numerical version of keyword (where a/A = 0)
    num_key = [ord(keyword[x]) - ord('A') if keyword[x].isupper() else
               ord(keyword[x]) - ord('a') for x in range(len(keyword))]

    # variable that's going to contain the ciphertext
    result = ""

    # declare key index and use length of keyword to wrap around
    k_len = len(keyword)
    k_idx = 0

    # loop over plaintext till we are done
    for i in range(len(text)):
        # encrypt if i'th character in text is in the alphabet
        if text[i].isalpha():
            # preserve case in ciphertext
            if text[i].islower():
                # add text[i] + key[k_idx] to result, wrap around if needed
                if (ord(text[i]) + num_key[k_idx % k_len]) > ord('z'):
                    result += chr(ord(text[i]) + num_key[k_idx % k_len] - ALPHA_LENGTH)
                    k_idx += 1
                else:
                    result += chr(ord(text[i]) + num_key[k_idx % k_len])
                    k_idx += 1

            if text[i].isupper():
                if (ord(text[i]) + num_key[k_idx % k_len]) > ord('Z'):
                    result += chr(ord(text[i]) + num_key[k_idx % k_len] - ALPHA_LENGTH)
                    k_idx += 1
                else:
                    result += chr(ord(text[i]) + num_key[k_idx % k_len])
                    k_idx += 1
        # leave non alphabetical characters unchanged
        else:
            result += text[i]
    return result


def main():
    # validate input
    if len(sys.argv) == 2 and sys.argv[1].isalpha():
        # get plaintext from the user
        plaintext = str(input("plaintext: "))

        # encrypt plaintext
        crypttext = encrypt(plaintext, sys.argv[1])

        # print the result to stdout
        print("ciphertext: {}".format(crypttext))

    # exit if user does not comply
    else:
        print("Usage: vigenere.py <alphabetical_key>")
        exit(1)


if __name__ == '__main__':
    main()
