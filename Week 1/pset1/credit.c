/*
----------------------------------------------------------
|     University of Amsterdam: Minor Programmeren        |
|     Problem Set 1: Credit                              |
|     Created by Davy Boekhout on 5/09/2017              |
|                                                        |
|     Checks a given credit card number validity         |
|     and identifies the corresponding credit            |
|     card company.                                      |
----------------------------------------------------------
*/

#include <stdio.h>
#include <cs50.h>

int main (void)
{
    // Get credit card number
    printf("Number: ");
    long long input = get_long_long();
    long long credit_card = input;
    long long credit_card2 = input / 10;
    long long credit_card3 = input;
    int number_of_digits = 0;
    int total_value = 0;

    // Determine how long the number is
    while (input !=0)
    {
        input /= 10;
        number_of_digits++;
    }

    // Add the numbers, always starting with the last one
    int add = 0;
    while (credit_card != 0)
    {
        add = add + (credit_card % 10);
        credit_card = credit_card / 100;
    }

    // Multiply the numbers, starting second-to-last
    int multiply = 0;
    int multiply_total = 0;
    while (credit_card2 != 0)
    {
        multiply = (credit_card2 % 10) * 2;
        if (multiply >= 10)
            multiply_total = multiply_total + multiply - 9;
        else
            multiply_total += multiply;
        credit_card2 = credit_card2 / 100;
    }

    // Getting the sum of multiplying and adding according to Hans Peter Luhn algorithm
    total_value += add + multiply_total;

    // Determine if credit card number is valid and what company it is from (AMEX/MASTERCARD/VISA)
    if (total_value % 10 == 0)
    {
        int count = number_of_digits;
        // Looping once more over the credit card number to get the first two digits
        while (count > 0)
        {
            credit_card3 = credit_card3 / 10;
            count--;
            if (count == 2)
            {
                int first_two = credit_card3;
                int first_digit = credit_card3 / 10;
                // Switching to determine credit card company
                switch (first_two)
                {
                    case 34:
                    case 37:
                        printf("AMEX\n");
                        break;
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                        printf("MASTERCARD\n");
                        break;
                    default:
                        if (first_digit == 4)
                            printf("VISA\n");
                        else
                            printf("INVALID\n");
                }
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }
}