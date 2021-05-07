#include <stdlib.h>
#include <string.h>
#include <stringutils.h>
#include <trie.h>

/**
 * Creates an empty Trie node
 **/
TrieNode* create_node() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));

    node->is_word = 0;
    for (int i = 0; i < NUM_CHARS; i++) {
        node->children[i] = NULL;
    }

    return node;
}

/**
 * Inserts a word into the trie. (Make sure it's lowercase)
 **/
void insert_word(TrieNode* root, char* word) {
    TrieNode* node = root;

    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';

        if (node->children[index] == NULL) {
            node->children[index] = create_node();
        }

        node = node->children[index];
    }

    node->is_word = 1;
}

/**
 * Loads a dictionary of words (a text) into the trie
 **/
void load_dict(TrieNode* root, char* dict) {
    // TODO: tokenize the dictionary and add the words to the trie
}

/**
 * Searches for all the words with the given prefix. (Make sure the it's
 * lowercase)
 **/
void prefix_search(TrieNode* root, char* prefix) {
    TrieNode* start = root;
    for (int i = 0; prefix[i]; i++) {
        int index = prefix[i] - 'a';
        start = start->children[index];
    }

    // TODO: Use DFS to find all the words below the 'start' node. Keep track of
    // the current word!
}