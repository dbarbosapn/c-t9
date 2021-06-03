#ifndef _T9_KEYS
#define _T9_KEYS

#include <int_hashtable.h>
#include <linked_list.h>
#include <trie.h>

char* get_word(char* input);

Node* get_permutations(char* input);

Node* run_t9(TrieNode* trie, HashTable* ht, char* input);

#endif