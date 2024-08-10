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

#define NUM_OF_BOARD_LENGTH 4
#define TOTAL_WORDS (NUM_OF_BOARD_LENGTH*NUM_OF_BOARD_LENGTH)

//NEW DICTIONARY????
//https://boardgames.stackexchange.com/questions/38366/latest-collins-scrabble-words-list-in-text-file

//adds a surrounding adjacent letter to the targetLetter's adjacent array based on coordinates
void checkSide(letter* targetLetter, letter** adjArray, int row, int col)
{
    targetLetter->adjacentLetters[(*targetLetter).numOfAdjacentLetters] = adjArray[row][col];
    targetLetter->numOfAdjacentLetters++;
}

//compiles all surrounding adjacent letters to targetLetter's adjacent array
void checkAllSides (letter* targetLetter, letter** adjArray, int row, int col)
{
    //allocates memory for array of adjacent of letters
    targetLetter->adjacentLetters = (letter *) calloc(8, sizeof(letter));

    if(row == 0) //if on the top row
    {
        if(col == 0 || col != (NUM_OF_BOARD_LENGTH-1)) //if not the last column
        {
            checkSide(targetLetter, adjArray, row, col+1); //Check Right
            checkSide(targetLetter, adjArray, row+1, col+1); //Check Bottom Right
        }
        if(col == (NUM_OF_BOARD_LENGTH-1) || col != 0) //if not the first column
        {
            checkSide(targetLetter, adjArray, row, col-1); //Check Left
            checkSide(targetLetter, adjArray, row+1, col-1); //Check Bottom Left
        }
        checkSide(targetLetter, adjArray, row+1, col); //Check Bottom
    }
    else if(row == (NUM_OF_BOARD_LENGTH-1)) //if on the last row
    {
        if(col == 0 || col != (NUM_OF_BOARD_LENGTH-1)) //if not the last column
        {
            checkSide(targetLetter, adjArray, row, col+1); //Check Right
            checkSide(targetLetter, adjArray, row-1, col+1); //Check Top Right
        }
        if(col == (NUM_OF_BOARD_LENGTH-1) || col != 0) //if not the first column
        {
            checkSide(targetLetter, adjArray, row, col-1); //Check Left
            checkSide(targetLetter, adjArray, row-1, col-1); //Check Top Left
        }
        checkSide(targetLetter, adjArray, row-1, col); //Check Up
    }
    else //middle rows
    {
        if(col == 0 || col != (NUM_OF_BOARD_LENGTH-1)) //if not the last column
        {
            checkSide(targetLetter, adjArray, row-1, col+1); //Check Top Right
            checkSide(targetLetter, adjArray, row, col+1); //Check Right
            checkSide(targetLetter, adjArray, row+1, col+1); //Check Bottom Right
        }
        if(col == (NUM_OF_BOARD_LENGTH-1) || col != 0) //if not the first column
        {
            checkSide(targetLetter, adjArray, row-1, col-1); //Check Top Left
            checkSide(targetLetter, adjArray, row, col-1); //Check Left
            checkSide(targetLetter, adjArray, row+1, col-1); //Check Bottom Left
        }
        checkSide(targetLetter, adjArray, row-1, col); //Check Up
        checkSide(targetLetter, adjArray, row+1, col); //Check Bottom
    }
}

//prints the words based on length of words
void printWords(Node* wordArray[], int numOfWordsHashed[], FILE* file)
{
    //runs through full hash to find all words
    for(int i = TOTAL_WORDS - 1; i >= 0; i--)
    {
        Node* temp = wordArray[i];

        //if a particular hash has words in it, then print out the words
        //printf("%d words that have %d letters\n", numOfWordsHashed[i], i);
        if(numOfWordsHashed[i] > 0)
        {
            fprintf(file, "Words with %d letters: \n", i);
            fprintf(file, "--------------------------\n");

            //prints out node by node per hash
            for(; temp->next != NULL; temp = temp->next)
            {
                fprintf(file, "%s\n", temp->value);
                fflush(file);
            }

            fprintf(file, "%s\n", temp->value);
            fflush(file);
            fprintf(file, "\n");
        }
    }
}

//inserts the words into the hashtable based on length
void insertWord(Node* wordArray[], int numOfWordsHashed[], char* word, int length)
{
    if(wordArray[length] == NULL)
    {
        wordArray[length] = createHashNode(word, NULL);
    }
    else 
    {
        insertNode(wordArray[length], createHashNode(word, NULL));
    }
    numOfWordsHashed[length]++;
}

//frees hashtable and linkedlists inside each hash
void clearHash(Node* wordArray[])
{
    //for every hash, we will check if there are any words there and free them
    for(int i = 0; i < TOTAL_WORDS; i++)
    {
        Node* temp = wordArray[i];
        
        //frees words till end of linked list
        while(temp != NULL)
        {
            Node* hold = temp;
            temp = temp->next;
            free(hold->value);
            free(hold);
        }
    }
}

//creates the board (the 2d array) of letters that holds surrounding letters and its index
letter** makeBoard(char* inputLetters)
{
    //checks if there are the right amount of letters per board length
    if(strlen(inputLetters) != TOTAL_WORDS)
    {
        if(strcmp(inputLetters, "exit") == 0)
        {
            printf("Exiting\n");
            exit(EXIT_SUCCESS);
        }
        printf("exiting - not %d letters\n", TOTAL_WORDS);
        exit(EXIT_FAILURE);
    }

    //creates the "board" that consists of the letters and its surrounding letters
    letter** arr = (letter**) malloc(NUM_OF_BOARD_LENGTH * sizeof(letter*));

    //loops through inputted letters and assigns its correct allocation in the board
    for(int i = 0; i < NUM_OF_BOARD_LENGTH; i++)
    {
        arr[i] = (letter*) malloc(NUM_OF_BOARD_LENGTH * sizeof(letter));
        for(int j = 0; j < NUM_OF_BOARD_LENGTH; j++)
        {
            arr[i][j] = (letter) {((char) tolower(inputLetters[(i*4)+j])), (i*4)+j, NULL};
        }
    }

    //loops through all the letters in the board and finds all its surrounding letters
    for(int i = 0; i < NUM_OF_BOARD_LENGTH; i++)
    {
        for(int j = 0; j < NUM_OF_BOARD_LENGTH; j++)
        {
            checkAllSides(&arr[i][j], arr, i, j);
        }
    }

    //reformats board so that surrounding letters have their OWN surrounding letters
    for(int i = 0; i < NUM_OF_BOARD_LENGTH; i++)
    {
        for(int j = 0; j < NUM_OF_BOARD_LENGTH; j++)
        {
            for(int k = 0; k < arr[i][j].numOfAdjacentLetters; k++)
            {
                letter temp = arr[i][j].adjacentLetters[k];
                int newRow = floor(temp.index/4);
                int newCol = temp.index % NUM_OF_BOARD_LENGTH;
                arr[i][j].adjacentLetters[k] = arr[newRow][newCol];
            }
        }
    }
    return arr;
}

//frees malloced board after use
void freeBoard(letter** arr)
{
    for(int i = 0; i < NUM_OF_BOARD_LENGTH; i++)
    {
        for(int j = 0; j < NUM_OF_BOARD_LENGTH; j++)
        {
            free((arr[i][j]).adjacentLetters);
        }
        free(arr[i]);
    }
    free(arr);
}

//recursively checks all combinations of words and adds valid words to hashtable
void recursiveWordSearch(letter targetChar, char* word, bool visited[], int length, Node* wordArray[], int numOfWordsHashed[])
{
    //checks if it is a valid word and adds to hash table
    if(length > 2 && checkDictionary(word))
    {
        insertWord(wordArray, numOfWordsHashed, word, length);
    }

    //goes through adjacent letters
    for(int i = 0; i < targetChar.numOfAdjacentLetters; i++)
    {
        letter node = targetChar.adjacentLetters[i];

        //if surrounding letter not visited (because cannot repeat same tile), then recurses into that letter
        if(visited[node.index] == false)
        {
            visited[node.index] = true;
            word[length] = node.letter;
            recursiveWordSearch(node, word, visited, length+1, wordArray, numOfWordsHashed);
            visited[node.index] = false;
            word[length] = '\0';
        }
    }
}

//helper function to make the board of letters, create all combinations of words, and add valid words to hash table
void checkWithDictionary(letter** adjArray)
{
    //opens up file to write to
    FILE *file = fopen("words.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
    }

    //creates hashtable to hold the words
    Node* wordArray[TOTAL_WORDS] = {NULL};

    //helper array for hashtable to hold amount of words per hash
    int numOfWordsHashed[TOTAL_WORDS] = {0};

    //double loop to go through all letters, to make words starting from that letter tile
    for(int i = 0; i < NUM_OF_BOARD_LENGTH; i++)
    {
        for(int j = 0; j < NUM_OF_BOARD_LENGTH; j++)
        {
            //restarts the visited array
            bool visited[TOTAL_WORDS] = {false};

            //creates the word stack
            char* word = malloc((TOTAL_WORDS+1)*sizeof(char));
            memset(word, '\0', ((TOTAL_WORDS+1)*sizeof(char)));
            word[0] = adjArray[i][j].letter;

            //marks current letter as visited
            visited[adjArray[i][j].index] = true;

            //recurses
            recursiveWordSearch(adjArray[i][j], word, visited, 1, wordArray, numOfWordsHashed);

            //frees word
            free(word);
        }
    }

    //frees full board
    freeBoard(adjArray);

    //prints all words
    printWords(wordArray, numOfWordsHashed, file);

    //clears hashtable
    clearHash(wordArray);

    //closes file
    fclose(file);
}

int main(int argc, char **argv)
{
    //creates the dictionary of words to make searching for valid words easier
    createDictionary("/usr/share/dict/words");
    
    //primer for the tool
    printf("\n************************************************************ \n"); 
    printf("             Welcome to the Word Hunt Solver! \n"); 
    printf(" Input your letters in order and with no spaces in between \n"); 
    printf("   Your solved words will open up in the words.txt file\n");
    printf("                           Enjoy!"); 
    printf("\n************************************************************ \n\n"); 

    //looping for endless input (so that the dictionary does not have to be remade each time)
    do
    {
        char* inputCommand = malloc(0);
        int commandLength = 0;
        char firstLetter;
        ssize_t readStatus;

        printf(">> ");   // interface
        fflush(stdout);

        readStatus = read(STDIN_FILENO,&firstLetter,sizeof(char));

        // runs until new line char or read finds a 0 which is the end of the file or stream close
        while(firstLetter != '\n' && readStatus != 0)
        {   
            commandLength++;
            inputCommand = realloc(inputCommand,sizeof(char) * commandLength);
            inputCommand[commandLength-1] = firstLetter;
            readStatus = read(STDIN_FILENO,&firstLetter,sizeof(char));
        }

        commandLength++;    // move out last read char
        inputCommand = realloc(inputCommand,sizeof(char) * commandLength);
        inputCommand[commandLength-1] = firstLetter;
        inputCommand[commandLength-1] = '\0'; //ensure null termination

        //adds the inputted letters into make a board which returns a board to be used to find all valid combinations of words
        checkWithDictionary(makeBoard(inputCommand));

        //frees memory
        free(inputCommand);

    } while(1);
}