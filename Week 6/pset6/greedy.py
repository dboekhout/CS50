#!/usr/bin/env python3
# ----------------------------------------------------------
# |     University of Amsterdam: Minor Programmeren        |
# |     Problem Set 6: Greedy (less)                       |
# |     Created by Davy Boekhout on 09/10/2017             |
# |                                                        |
# |     Calculates the minimal amount of coins needed      |
# |     to give back a given amount of change.             |
# ----------------------------------------------------------


def get_positive_float():
    """Take input from the user and return a positive floating number"""
    try:
        f = float(input("How much change is owed?\n"))
        if f <= 0.0:
            raise ValueError
    # in case user inputs text or a negative number
    except ValueError:
        return
    else:
        return round(f * 100)


def get_change(change):
    """Returns the minimal amount of coins needed to give back change"""
    coins = 0

    # test how many times change_owed goes into nickel, dime and cent
    while change != 0:
        if change > 24:
            coins += 1
            change -= 25
        elif change > 9:
            coins += 1
            change -= 10
        elif change > 4:
            coins += 1
            change -= 5
        else:
            coins += 1
            change -= 1
    return coins


def main():
    print("O hai! ", end="")

    # validate input
    change_owed = None
    while change_owed == None:
        change_owed = get_positive_float()

    # get the minimal amount of coins needed
    coin_amount = get_change(change_owed)
    print("{}".format(coin_amount))


if __name__ == '__main__':
    main()
