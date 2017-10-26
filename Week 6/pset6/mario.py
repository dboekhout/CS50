#!/usr/bin/env python3
# ----------------------------------------------------------
# |     University of Amsterdam: Minor Programmeren        |
# |     Problem Set 6: Mario (more)                        |
# |     Created by Davy Boekhout on 09/10/2017             |
# |                                                        |
# |     Prints half-pyramids using hashes and spaces.      |
# ----------------------------------------------------------


def get_height():
    """Validates user input and returns pyramid height."""
    while True:
        choice = input("Height: ")
        if choice.isdigit() and 0 <= int(choice) <= 23:
            return int(choice)


def main():
    """Print out a double half-pyramid from the Mario game using hashes."""
    # get pyramid height from the user
    height = get_height()

    # base values for pyramid dimensions
    spaces = height - 1
    hashes = 1

    # print out a double half pyramid
    for i in range(height):
        print(" " * spaces + "#" * hashes + "  " + "#" * hashes)
        spaces -= 1
        hashes += 1


if __name__ == '__main__':
    main()
