#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 Wins! \n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 Wins! \n");
    }
    else
    {
        printf("Tie! \n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string

    // Makes chars in word all lowercase
    int n = strlen(word);
    int lower_word[n];
    int score_word[n];

    for (int i = 0; i < n; i++)
    {
        lower_word[i] = tolower(word[i]);
    }

    // assigns value to chars
    for (int i = 0; i < n; i++)
    {
        if (lower_word[i] > 96)
        {
            score_word[i] = POINTS[ lower_word[i] - 97];
        }
        else
        {
            score_word[i] = 0;
        }
    }

    // Calc Score
    int score = 0;
    for (int i = 0; i < n; i++)
    {
        score = score + score_word[i];
    }

    return score;
}
