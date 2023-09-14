#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Asks for an input > 1 and < 8
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // For each row
    for (int i = 1; i < n + 1; i++)
    {

        // Centers Tip of Pyramid by adding n-i empty spaces
        for (int j = 0; j < n - i; j++)
        {
            printf(" ");
        }

        // Prints Front Half of Pyramid
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // Prints Gap in Pyramid
        printf("  ");

        // Prints Back Half of Pyramid
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // Moves Down a Row
        printf("\n");
    }
}