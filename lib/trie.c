#include <linked_list_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stringutils.h>
#include <trie.h>

/**
 * Creates an empty Trie node
 **/
TrieNode* create_trie_node() {
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
            node->children[index] = create_trie_node();
        }

        node = node->children[index];
    }

    node->is_word = 1;
}

/**
 * Loads a dictionary of words into the trie (FILE pointer should be already
 * open with read permissions)
 **/
void trie_load_dict(TrieNode* root, FILE* dict) {
    char buffer[LOAD_DICT_BUF];
    while (fscanf(dict, "%s", buffer) != EOF) {
        str_normalize(buffer);
        if (str_scan(buffer, LOAD_DICT_BUF)) {
            insert_word(root, buffer);
        }
    }
}

/**
 * Searches for all the words with the given prefix. (Make sure the it's
 * lowercase)
 * If a non-null hashtable is passed, it will be used to order the results
 * Returns a linked list
 **/
Node* prefix_search(TrieNode* root, char* prefix, HashTable* ht) {
    TrieNode* start = root;
    for (int i = 0; prefix[i]; i++) {
        int index = prefix[i] - 'a';
        if (start->children[index] != NULL) {
            start = start->children[index];
        } else {
            return NULL;
        }
    }
    Node* list = NULL;
    size_t prefix_s = sizeof(prefix);
    char* copy = (char*)malloc(prefix_s);
    memcpy(copy, prefix, prefix_s);
    get_words(&list, start, copy, ht);
    return list;
}

/**
 * Gets all the words using DFS. Remember to free the nodes' values when you
 * don't need them anymore!
 **/
void get_words(Node** list, TrieNode* root, char* current, HashTable* ht) {
    int curr_len = strlen(current);

    if (root->is_word) {
        if (ht != NULL) {
            *list = list_add_inorder_ht(*list, (void*)current,
                                        sizeof(char) * (curr_len + 1), ht);
        } else {
            *list = list_prepend(*list, (void*)current,
                                 sizeof(char) * (curr_len + 1));
        }
    }

    for (int i = 0; i < NUM_CHARS; i++) {
        if (root->children[i] != NULL) {
            char* new_prefix = (char*)malloc(sizeof(char) * (curr_len + 2));

            int j;
            for (j = 0; j < curr_len; j++) {
                new_prefix[j] = current[j];
            }
            new_prefix[j] = i + 'a';
            new_prefix[j + 1] = '\0';

            get_words(list, root->children[i], new_prefix, ht);
        }
    }

    if (!root->is_word) {
        free(current);
    }
}