#!/usr/bin/env python3
# ----------------------------------------------------------
# |     University of Amsterdam: Minor Programmeren        |
# |     Problem Set 6: Credit (more)                       |
# |     Created by Davy Boekhout on 11/10/2017             |
# |                                                        |
# |     Checks if a given credit number is valid and is    |
# |     either from American Express, Mastercard or Visa.  |
# ----------------------------------------------------------


def get_cc_number():
    """Returns the credit card number the user entered."""
    while True:
        number = input("Number: ")
        if number.isdigit():
            return number


def main():
    """Applies Luhn's algorithm to check a credit card number and prints
    the vendor to stdout.
    """
    # define valid prefixes for credit cards
    AMEX = [34, 37]
    MCRD = [50, 51, 52, 53, 54, 55]
    VISA = [4]

    # set credit card properties
    cc_number = get_cc_number()
    cc_prefix = int(str(cc_number[:2]))
    cc_length = len(cc_number)
    cc_string = ""
    cc_summed = 0

    # apply Luhn's check algorithm by multiplying every other digit
    cc_part_1 = [str(int(x) * 2) for x in cc_number[-2::-2]]

    # and then saving the remaining numbers we haven't processed yet
    if cc_length % 2 == 0:
        cc_part_2 = [x for x in cc_number[1::2]]
    else:
        cc_part_2 = [x for x in cc_number[::2]]

    # sum results in cc_part_1 and cc_part_2
    for digit in cc_part_1:
        cc_string += digit
    for digit in cc_string:
        cc_summed += int(digit)
    for digit in cc_part_2:
        cc_summed += int(digit)

    # if cc_summed ends with 0, high probability of valid cc_number
    if cc_summed % 10 == 0:
        if cc_prefix in AMEX and cc_length == 15:
            print("AMEX")
        elif cc_prefix in MCRD and cc_length == 16:
            print("MASTERCARD")
        elif int(cc_prefix / 10) in VISA and \
                (cc_length == 13 or cc_length == 16):
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")


if __name__ == '__main__':
    main()
