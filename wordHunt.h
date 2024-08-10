
#define NUM_OF_BOARD_LENGTH 4
#define TOTAL_WORDS (NUM_OF_BOARD_LENGTH*NUM_OF_BOARD_LENGTH)

typedef struct letter {
    char letter;
    int index;
    struct letter* adjacentLetters;
    int numOfAdjacentLetters;
} letter;

// Linked list node
typedef struct Node {
    char* value;
    struct Node* next;
} Node;

void checkSide(letter* targetLetter, letter** adjArray, int row, int col);
void checkAllSides (letter* targetLetter, letter** adjArray, int row, int col);
void printWords(Node* wordArray[], int numOfWordsHashed[], FILE* file);
void insertWord(Node* wordArray[], int numOfWordsHashed[], char* word, int length);
void clearHash(Node* wordArray[]);
letter** makeBoard(char* inputLetters);
void freeBoard(letter** arr);
void recursiveWordSearch(letter targetChar, char* word, bool visited[], int length, Node* wordArray[], int numOfWordsHashed[]);
void checkWithDictionary(letter** wordsArray);
