/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// create root node and traversal node for our trie
node *root;
node *trav;

// count words we encounter in the dictionary
int word_count;
// determine node size for calloc
int node_size = sizeof(node);

/**
 * Retrieve index of a given character
 */
int getIndex(const char c)
{
    if (c == '\'')
    {
        return APOSTROPHE;
    }
    else
    {
        return tolower(c) % 'a';
    }
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // set traversal node back to the root for new word
    trav = root;

    // loop over each character in word
    for (int i = 0; word[i] != '\0'; i++)
    {
        // declare our index for children[]
        int j = getIndex(word[i]);

        // if character j is not in our trie, word is mispelled
        if (trav->children[j] == NULL)
        {
            return false;
        }
        // otherwise continue down the trie
        else
        {
            trav = trav->children[j];
        }
    }

    // if we're at the end of our word, check if it's marked in the trie
    if (trav->is_word == true)
    {
        return true;
    }
    return false;
}

/**
 * Returns a pointer to the newly created (trie) node
 */
node *newnode(void)
{
    node *ptr = calloc(1, node_size);

    // quit if calloc was unsuccesful
    if (ptr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for new node.\n");
        exit(1);
    }

    return ptr;
}

/**
 * Inserts a character into the trie and returns the (un)changed node
 */
node *insert(node *trav, const char character)
{
    // index value
    int i = getIndex(character);

    // check if children[i] exists in the traversal node
    if (trav->children[i] == NULL)
    {
        trav->children[i] = newnode();
    }
    return trav->children[i];
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary in read mode
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    // set root node and copy it to the traversal node
    root = newnode();
    trav = root;

    // loop over words in the dictionary, character by character
    for (int c = fgetc(dict); c != EOF; c = fgetc(dict))
    {
        // while working on the same word, insert character into trie
        if (c != '\n')
        {
            trav = insert(trav, c);
        }
        // on a new line, mark word and reset our traversal node
        else
        {
            trav->is_word = true;
            trav = root;
            word_count++;
        }
    }

    // we succesfully loaded the dictionary
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return word_count > 0 ? word_count : 0;
}

/**
 * Recursively unload memory reserved by load();
 */
void r_unload(node *trienode)
{
    // loop over every child in current node
    for (int i = 0; i < ALPHA_LENGTH; i++)
    {
        // if child is a pointer, traverse the trie
        if (trienode->children[i] != NULL)
        {
            r_unload(trienode->children[i]);
        }
    }
    // free memory from the bottom up
    free(trienode);
}

/**
 * Unloads dictionary from memory.
 */
bool unload(void)
{
    r_unload(root);
    return true;
}
