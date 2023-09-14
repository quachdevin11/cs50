#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int count_letters(string passage);
int count_words(string passage);
int count_sentences(string passage);

int main(void)
{
    // Input Text/Passage
    string passage = get_string("Text: ");

    // declares number of letters, words, and sentences as floats
    float l = count_letters(passage);
    float w = count_words(passage);
    float s = count_sentences(passage);

    float L = (l / w) * 100.0;
    float S = (s / w) * 100.0;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    index = round(index);

    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %.0f\n", index);  // Needs to Print Just The Integer no decimal
    }

}

int count_letters(string passage)
{

    int lower_passage[strlen(passage)];
    int count = 0;

    for (int i = 0, len = strlen(passage); i < len; i++)
    {
        // Makes all letters into lower case values
        lower_passage[i] = tolower(passage[i]);

        // Adds to count whenever char is a lowercase value
        if (lower_passage[i] > 96 &&  lower_passage[i] < 124)
        {
            count = count + 1;
        }
    }
    return count;
}

int count_words(string passage)
{
    int count = 1;

    for (int i = 0, len = strlen(passage); i < len; i++)
    {
        if (passage[i] == 32)
        {
            count = count + 1;
        }
    }
    return count;
}

int count_sentences(string passage)
{
    int count = 0;

    for (int i = 0, len = strlen(passage); i < len; i++)
    {
        if (passage[i] == 46 || passage[i] == 33 || passage[i] == 63)
        {
            count = count + 1;
        }
    }
    return count;
}