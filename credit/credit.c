#include <cs50.h>
#include <stdio.h>


// FUNCTION TO SEE IF CARD NUMBER IS VALID - WOULD HAVE LIKED TO DO PROTOTYPE BUT WASN'T WORKING
bool valid(long long num)
{
    long long n = num;
    int count = 0;

    // Find Number of Digits in Num
    do{
        n = n / 10;
        count = count + 1;
    }
    while (n != 0);

    int reverseNum[count];
    int lastDigits[10] = {0};
    int secondToLastDigits[10] = {0};
    int j = 0;
    int k = 0;

    // Reverse the order of Num and puts into 2 arrays
    for (int i = 0; i < count; i++)
    {
        reverseNum[i] = num % 10;
        num = num / 10;
        if (i % 2 == 0)
            {   // Values at Even Array Positions  (Last Digit of Num)
                lastDigits[j] = reverseNum[i];
                j++;
            }
        else
        {   // Values at Odd Array Positions  (Second to Last Digit of Num * 2) (STLD)
            if (reverseNum[i] != 0)
            {
                secondToLastDigits[k] = reverseNum[i] * 2;
                k++;
            }
        }
    }

    // Preallocating & Declaring Arrays for Luhn's Algorihm
    int digitsOfSTLD[20] = {0};
    int numToDigits = 1;
    k = 0;
    int i = 0;

    while (secondToLastDigits[i] != 0)
    {
        // Takes double digits & separates them, adding to digitsofSTLD array
        if (secondToLastDigits[i] > 9)
        {
            numToDigits = secondToLastDigits[i];
            while (numToDigits != 0)
            {
                digitsOfSTLD[k] = numToDigits % 10;
                numToDigits = numToDigits / 10;
                k = k + 1;
            }
        }
        // Takes single digits & adds to digitsofSTLD array
        else
        {
            digitsOfSTLD[k] = secondToLastDigits[i];
            k = k + 1;
        }
        i = i + 1;
    }

    int sumOfSTLD_digits = 0;
    int sumOfLD = 0;



    // Sum elements of digitsOfSTLD & lastDigits
    for (i = 0; i < sizeof(digitsOfSTLD) / sizeof(digitsOfSTLD[0]); i++)
    {
        sumOfSTLD_digits = sumOfSTLD_digits + digitsOfSTLD[i];
    }
    for (i = 0; i < sizeof(lastDigits) / sizeof(lastDigits[0]); i++)
    {
        sumOfLD = sumOfLD + lastDigits[i];
    }


    int sum = sumOfSTLD_digits + sumOfLD;
    int lastDigSum = sum % 10;
    bool val = 0;

    // Legit or Not
    if (lastDigSum == 0)
    {
        val = 1;
    }

    // --------------- DEBUGGING BY PRINTING VALUES OUT ------------
    // printf("reverseNum, lastDigits, secondToLastDigits Arrays: \n");
    // for(int a = 0; a < count; a++)
    // {
    //     printf("%d ", reverseNum[a]);
    // }

    // printf("\n");

    // for(int a = 0; a < (count/2)+1; a++)
    // {
    //     printf("%d ", lastDigits[a]);
    // }

    // printf("\n");

    // for(int a = 0; a < (count/2)+1; a++)
    // {
    // printf("%d ", secondToLastDigits[a]);
    // }

    // printf("\n");

    // for(int a = 0; a < (count); a++)
    // {
    // printf("%d ", digitsOfSTLD[a]);
    // }

    // printf("\n");

    // printf("Sum of STLD: %d \n", sumOfSTLD_digits);
    // printf("Sum of LD: %d \n", sumOfLD);
    // printf("Sum: %d \n", sum);
    // printf("LD of Sum: %d \n", lastDigSum);

    return val;
}

int main(void)
{
    long long num;

    // ensure entering more then 13 digits
    do {
        num = get_long("Number: ");
    }
    while (num<1000000000);

    // if legti card provide type of card
    if (valid(num) == 1)
    {
        long n = num;
        int count = 0;

        // Find Number of Digits in Num
        do {
            n = n / 10;
            count = count + 1;
        }
        while (n != 0);

        int reverseNum[count];

        // Reverse the order of Num
        for (int i = 0; i < count; i++)
        {
            reverseNum[i] = num % 10;
            num = num / 10;
        }

        // Looks for starting digits of num
        int initial_digit = reverseNum[count - 1];
        int initial_digits = (initial_digit * 10) + reverseNum[count - 2];

        // USED FOR DEBUGGING
        // printf("Initial Digit: %i\n", initial_digit);
        // printf("Initial Digits: %i\n", initial_digits);
        // printf("Count: %d", count);

        if (count == 15)
        {
            if (initial_digits == 34 || initial_digits == 37)
            {
                printf("AMEX\n");
            }
            else
            {printf("INVALID\n");
            }
        }
        else if (count == 16)
        {
            if (initial_digits == 51 || initial_digits == 52 || initial_digits == 53 || initial_digits == 54 || initial_digits == 55)
            {
                printf("MASTERCARD\n");
            }
            else if (initial_digit == 4)
            {
                printf("VISA\n");
            }
            else
            {printf("INVALID\n");
            }
        }
        else if (count == 13)
        {
            if (initial_digit == 4)
            {
                printf("VISA\n");
            }
            else
            {printf("INVALID\n");
            }
        }

        else
        {
            {printf("INVALID\n");
            }
        }
    }

    else
    {
        printf("INVALID\n");
    }
}




