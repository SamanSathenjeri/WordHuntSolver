#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>

//includes wordHunt
#include "wordHunt.h"

//includes dictionary
#include "dictionary.h"

//includes dictionary
#include "Node.h"

Node* createHashNode(char* word, Node* next)
{
    Node* insertedNode = (Node*) malloc(sizeof(Node));
    char* newWord = malloc((strlen(word)+1) * sizeof(char));
    strcpy(newWord, word);
    newWord[(strlen(word))] = '\0';
    (*insertedNode) = (Node) {newWord, next};
    return insertedNode;
}

void insertNode(Node* root, Node* insertedNode)
{
    bool sameWord = false;
    Node* temp = root;
    char* word = (*insertedNode).value;

    for(; temp->next != NULL; temp = temp->next)
    {
        if(strcmp(word, (*temp).value) == 0)
        {
            sameWord = true;
            free(insertedNode);
            break;
        }
    }

    if(!sameWord)
    {
        if(strcmp(word, (*temp).value) == 0)
        {
            free(insertedNode);
        }
        else
        {
            temp->next = insertedNode;
        }
    }
}

