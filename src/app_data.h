#ifndef _APP_DATA
#define _APP_DATA

#include <trie.h>

#include "graphics.h"

typedef struct {
    Graphics* gr;
    HashTable* ht;
    TrieNode* trie;
} AppData;

#endif