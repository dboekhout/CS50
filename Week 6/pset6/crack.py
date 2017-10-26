#!/usr/bin/env python3
# ----------------------------------------------------------
# |     University of Amsterdam: Minor Programmeren        |
# |     Problem Set 6: Crack (more)                        |
# |     Created by Davy Boekhout on 11/10/2017             |
# |                                                        |
# |     Cracks a password that's been hashed with the unix |
# |     crypt() function. Default max password length is 4.|
# ----------------------------------------------------------

import sys
import crypt
import itertools


def crack_password(hash_to_crack, salt, max_password_length, character_set):
    """Returns a cracked password that's been hashed with the unix crypt()
    function.
    """
    # start generating passwords of length 1
    pass_length = 1

    # generate the cartesian product of character_set
    while pass_length <= max_password_length:
        for password in itertools.product(character_set, repeat=pass_length):
            # if hashed combination matches the hash we're looking for
            if crypt.crypt(''.join(password), salt) == hash_to_crack:
                # return plaintext password
                return ''.join(password)
        pass_length += 1


def main():
    """Defines brute force properties and prints out the cracked password."""
    # exit on incorrect argument count
    if len(sys.argv) != 2:
        sys.exit('Usage: {} hashed_password'.format(sys.argv[0]))

    # brute force properties
    chr_set = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ"
    max_len = 4
    pw_salt = sys.argv[1][:2]

    # brute force given hash
    password = crack_password(sys.argv[1], pw_salt, max_len, chr_set)
    print("{}".format(password))


if __name__ == '__main__':
    main()
