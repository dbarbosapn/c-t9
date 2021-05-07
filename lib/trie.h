#include <linked_list.h>
#include <stdio.h>

#define NUM_CHARS 26
#define LOAD_DICT_BUF 64

typedef struct trie_node {
    int is_word;
    struct trie_node* children[NUM_CHARS];
} TrieNode;

TrieNode* create_trie_node();

void insert_word(TrieNode* root, char* word);

void trie_load_dict(TrieNode* root, FILE* dict);

Node* prefix_search(TrieNode* root, char* prefix);

Node* get_words(TrieNode* root, char* current);