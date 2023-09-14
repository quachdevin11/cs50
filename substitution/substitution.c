#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Needs a Key
    if (argc != 2)
    {
        printf("Error: Needs Key \n");
        return 1;
    }

    // Needs a Key with 26 characters
    if (strlen(argv[1]) != 26)
    {
        printf("Error: Key Needs to be 26 Characters \n");
        return 1;
    }

    // Key Needs to be alphabetical
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Error: Key Needs to be Alphabetical \n");
            return 1;
        }

        // Key must not have repeat characters
        for (int j = i + 1; j < n; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Error: Key Needs to not repeat characters \n");
                return 1;
            }

        }
    }

    string plain_text = get_string("plaintext: ");
    char cipher_text[strlen(plain_text)];

    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        // Only changes alphabet chars
        if (isalpha(plain_text[i]) != 0)
        {
            int place_in_alphabet = tolower((char) plain_text[i]) - 97;

            // Keeps case type
            if (islower(plain_text[i]) != 0)
            {
                cipher_text[i] = tolower(argv[1][place_in_alphabet]);
            }
            else
            {
                cipher_text[i] = toupper(argv[1][place_in_alphabet]);
            }
        }

        // Keeps symbols/non alphabet chars
        else
        {
            cipher_text[i] = plain_text[i];
        }
    }

    printf("ciphertext: ");

    // Prints each char in cipher_text
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        printf("%c", cipher_text[i]);

        if (i == n - 1)
        {
            printf("\n");
        }
    }

}