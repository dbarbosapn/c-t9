#include <linked_list.h>
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
    // TODO: Improve this. Words ended on '.', ',' or started by '"' are being
    // ignored.
    char buffer[LOAD_DICT_BUF];
    while (fscanf(dict, "%s", buffer) != EOF) {
        if (str_is_word(buffer)) {
            str_to_lower(buffer);
            insert_word(root, buffer);
        }
    }
}

/**
 * Searches for all the words with the given prefix. (Make sure the it's
 * lowercase)
 * Returns a linked list
 **/
Node* prefix_search(TrieNode* root, char* prefix) {
    // TODO: PASS COMPARATOR TO THIS FUNCTION TO INSERT IN ORDER ON THE LIST (By
    // frequency on the dict).

    TrieNode* start = root;
    for (int i = 0; prefix[i]; i++) {
        int index = prefix[i] - 'a';
        if (start->children[index] != NULL) {
            start = start->children[index];
        } else {
            return NULL;
        }
    }

    return get_words(start, prefix);
}

/**
 * Gets all the words using DFS. Remember to free the nodes' values when you
 * don't need them anymore!
 **/
Node* get_words(TrieNode* root, char* current) {
    int curr_len = strlen(current);

    Node* head = NULL;
    if (root->is_word) {
        head = create_node((void*)current, sizeof(char) * (curr_len + 1));
    }

    Node* curr = head;

    for (int i = 0; i < NUM_CHARS; i++) {
        if (root->children[i] != NULL) {
            char* new_prefix = (char*)malloc(sizeof(char) * (curr_len + 2));

            int j;
            for (j = 0; j < curr_len; j++) {
                new_prefix[j] = current[j];
            }
            new_prefix[j] = i + 'a';
            new_prefix[j + 1] = '\0';

            Node* n = get_words(root->children[i], new_prefix);

            if (curr == NULL) {
                head = n;
                curr = head;
            } else {
                curr->next = n;
            }
            while (curr->next != NULL) curr = curr->next;
        }
    }

    if (!root->is_word) {
        free(current);
    }

    return head;
}