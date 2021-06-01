#ifndef _TRIE
#define _TRIE

#include <int_hashtable.h>
#include <linked_list.h>
#include <stdio.h>

#define NUM_BUCKETS 26
#define LOAD_DICT_BUF 64

typedef struct trie_node {
    int is_word;
    Node* buckets[NUM_BUCKETS];
    char val;
    int is_diacritic;
} TrieNode;

TrieNode* create_trie_node();

void insert_word(TrieNode* root, char* word);

void trie_load_dict(TrieNode* root, FILE* dict);

Node* prefix_search(TrieNode* root, char* prefix, HashTable* ht,
                    Node* starting_list);

void get_words(Node** list, TrieNode* root, char* current, HashTable* ht);

void trie_save(TrieNode* head, FILE* fp);

TrieNode* trie_load(FILE* fp);

#endif