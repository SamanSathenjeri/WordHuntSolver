#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>

//includes wordHunt
#include "wordHunt.h"

//includes dictionary
#include "dictionary.h"

//includes dictionary
#include "Node.h"

//sets the static variable
static trienode* root;

//allows for setting the root
void setRootNode (trienode* node)
{
    root = node;
}

//allows for getting the root
trienode* getRootNode()
{
    return root;
}

//helper function to create a new trienode
trienode* createNode(char character)
{
    trienode* newNode = malloc(sizeof(*newNode));
    newNode->value = character;
    for(int i = 0; i < NUM_CHARS; i++)
    {
        newNode->children[i] = NULL;
    }
    newNode->endOfWord = false;
    return newNode;
}

//inserts a trienode into the tree
bool insertTrie(trienode* node, char* word)
{
    trienode* temp = node;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        if (temp->children[word[i]] == NULL)
        {
            temp->children[word[i]] = createNode(word[i]);
        }

        temp = temp->children[word[i]];
    }

    if (temp->endOfWord == true)
    {
        return false;
    }
    else
    {
        temp->endOfWord = true;
        return true;
    }
}

//prints out the trie words
void printTrieRec(trienode* hold, char* segment, int length)
{
    trienode* node = hold;
    char newPrefix[length + 2];

    memcpy(newPrefix, segment, length);

    newPrefix[length + 1] = 0;

    if (node->endOfWord == true)
    {
        printf("Word: %s\n", segment);
    }

    for (int i = 0; i < NUM_CHARS; i++)
    {
        if (node->children[i] != NULL)
        {
            newPrefix[length] = i;
            printTrieRec(node->children[i], newPrefix, length+1);
        }
    }
}

//prints out the trie
void printTrie(trienode* node)
{
    if (node == NULL)
    {
        printf("trie is empty\n");
    }
    printTrieRec(node, NULL, 0);
}

//helps append to word
char* addToWord(char* word, char addedCharacter)
{
    char* finalWord = strcat(word, &addedCharacter);
    return finalWord;
}

//checks if the word memory is empty
bool wordNotEmpty(char* word)
{
    char* holder = word;
    for (int i = 0; i < 200; i++)
    {
        holder = word + i;
        if (*holder != NULL)
        {
            return true;
        }
    }
    return false;
}

//checks if a word is in the dictionary
bool checkDictionary(char* word)
{
    trienode* temp = getRootNode();
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        //checks if there is not a corresponding letter in the trie
        //printf("character: %c\n", word[i]);
        if (temp->children[word[i]] == NULL)
        {
            return false;
        }
        
        //continues through the trie
        temp = temp->children[word[i]];
    }

    //at the end of the trie, checks if final letter is the end of the word
    if (temp->endOfWord == true)
    {
        //printf("correct word: %s\n", word);
        return true;
    }
    else
    {
        //printf("in here\n");
        return false;
    }
}

//function to run through dictionary and add words to trie data structure
void createDictionary(char* filepath)
{
    //open dictionary file
    int fd = open(filepath, O_RDONLY);
    if (fd == -1)
    {
        printf("File: %s cannot open\n", filepath);
        exit(EXIT_FAILURE);
    }

    //create root node for trie
    setRootNode(createNode('*'));

    //read dictionary file and add words into trie
    char buf[BUFSIZE];
    int r;
    char* word = calloc(20, sizeof(char));

    while ((r = read(fd, buf, BUFSIZE)) > 0) 
    {
        //printf("added line: %s with %d bytes\n", buf, r);
        for (int i = 0; i < r; i++) 
        {
            if (buf[i] != '\n' && isalpha(buf[i]))
            {
                addToWord(word, buf[i]);
            }
            else
            {
                insertTrie(root, word);
                //printf("added Word: %s\n", word);
                memset(word, 0, sizeof word);
            }
        }
    }

    if (wordNotEmpty(word))
    {
        insertTrie(root, word);
    }

    free(word);

    //print trie
    //printTrie(root);

    //close dictionary file
    close(fd);
}
