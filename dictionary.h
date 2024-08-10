#ifndef TRIE_H
#define TRIE_H

//number of ascii characters
#define NUM_CHARS 256

//buffer length
#define BUFSIZE 16

typedef struct trienode {
    char value;
    struct trienode* children[NUM_CHARS];
    bool endOfWord;
} trienode;

bool insertTrie(trienode* root, char* word);
void printTrieRec(trienode* node, char* segment, int length);
void printTrie(trienode* root);
void createDictionary(char* filepath);
char* addToWord(char* word, char addedCharacter);
bool wordNotEmpty(char* word);
void clearWord(char* word);
bool checkDictionary(char* word);

#endif // TRIE_H
