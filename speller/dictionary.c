// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table = # of letters * max character count of word
const unsigned int N = 26*45;

// Hash table
node *table[N];
int count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int word_index = hash(word);
    if (table[word_index] != NULL)
    {
        node *cursor = table[word_index];
        int j = strcasecmp(word, cursor->word);
        while (strcasecmp(word, cursor->word)!=0)
        {
            cursor = cursor->next;
            if (cursor == NULL)
            {
                return false;
            }
        }
        return true;

    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int i;
    int l = strlen(word);
    int value = 0;
    for(i = 0; i < l; ++i)
    {
        value = value + toupper(word[i]) - 'A'; // Adds up each ASCII character
    }

    return value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opens Dictionary
    FILE *d = fopen(dictionary,"r");

    // If Dictionary is open
    if (d != NULL)
    {
        char buffer[LENGTH + 1]; // Placeholder for word
        while (fscanf(d, "%s", buffer) != EOF) // Runs for each word, until EOF is returned
        {
            count = count + 1;

            // Allocates Memory for new node & checks if empty
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }

            // Copy's read word from buffer[] to word[] in node
            strcpy(n->word,buffer);
            n->next = NULL;

            // Input node into table with hashed index
            int index = hash(n->word);
            if (table[index] != NULL)
            {
                n->next = table[index];
                table[index] = n;
            }
            else // If node is already present prepend node into array index's linked list
            {
                table[index] = n;
            }
        }

        return true;
    }

    return false;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Pulls from count, a global var that counts within load()

    if (count != 0)
    {
        return count;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        if(table[i]!=NULL)
        {
            node *cursor = table[i];
            while(cursor->next != NULL)
            {
                node *tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
            free(table[i]);

            return true;
        }
    }

    return false;
}
