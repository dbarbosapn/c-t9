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
    node->is_diacritic = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        node->buckets[i] = NULL;
    }

    return node;
}

/**
 * Inserts a word into the trie. (Make sure it's lowercase)
 **/
void insert_word(TrieNode* root, char* word) {
    TrieNode* node = root;

    for (int i = 0; word[i]; i++) {
        char c = word[i];
        int is_diacritic = c == -61;

        if (is_diacritic) {
            i++;
            c = word[i];
        }
        int index = c % NUM_BUCKETS;
        if (index < 0) index += NUM_BUCKETS;

        TrieNode* tnode = NULL;

        for (Node* curr = node->buckets[index]; curr != NULL;
             curr = curr->next) {
            TrieNode* t = curr->value;
            if (t->is_diacritic == is_diacritic && t->val == c) {
                tnode = t;
                break;
            }
        }

        if (tnode == NULL) {
            tnode = create_trie_node();
            tnode->is_diacritic = is_diacritic;
            tnode->val = c;
            node->buckets[index] = list_append(node->buckets[index], tnode, 0);
        }

        node = tnode;
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
Node* prefix_search(TrieNode* root, char* prefix, HashTable* ht,
                    Node* starting_list) {
    TrieNode* start = root;
    for (int i = 0; prefix[i]; i++) {
        char c = prefix[i];
        int is_diacritic = c == -61;
        if (is_diacritic) {
            i++;
            c = prefix[i];
        }
        int index = c % NUM_BUCKETS;
        if (index < 0) index += NUM_BUCKETS;

        int found = 0;
        for (Node* curr = start->buckets[index]; curr != NULL;
             curr = curr->next) {
            TrieNode* n = curr->value;
            if (n->is_diacritic == is_diacritic && n->val == c) {
                start = n;
                found = 1;
                break;
            }
        }
        if (!found) return NULL;
    }
    Node* list = starting_list;
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

    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (Node* curr = root->buckets[i]; curr != NULL; curr = curr->next) {
            char* new_prefix = (char*)malloc(sizeof(char) * (curr_len + 2));

            int j;
            for (j = 0; j < curr_len; j++) {
                new_prefix[j] = current[j];
            }

            TrieNode* tn = curr->value;

            if (tn->is_diacritic) {
                new_prefix[j] = -61;
                new_prefix[j + 1] = tn->val;
                new_prefix[j + 2] = '\0';
            } else {
                new_prefix[j] = tn->val;
                new_prefix[j + 1] = '\0';
            }

            get_words(list, tn, new_prefix, ht);
        }
    }

    if (!root->is_word) {
        free(current);
    }
}

/**
 * Saves the trie in the given file. fp must have write binary permissions.
 **/
void trie_save(TrieNode* head, FILE* fp) {
    fwrite(&head->is_word, sizeof(int), 1, fp);
    fwrite(&head->is_diacritic, sizeof(int), 1, fp);
    fwrite(&head->val, sizeof(char), 1, fp);
    int num_buckets = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (head->buckets[i] != NULL) num_buckets++;
    }
    fwrite(&num_buckets, sizeof(int), 1, fp);
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (head->buckets[i] != NULL) {
            fwrite(&i, sizeof(int), 1, fp);
            list_save_with_size(head->buckets[i], fp,
                                (void (*)(void*, FILE*))trie_save);
        }
    }
}

/**
 * Loads the trie from the given file. fp must have read binary permissions.
 **/
TrieNode* trie_load(FILE* fp) {
    TrieNode* node = create_trie_node();
    fread(&node->is_word, sizeof(int), 1, fp);
    fread(&node->is_diacritic, sizeof(int), 1, fp);
    fread(&node->val, sizeof(char), 1, fp);
    int n;
    fread(&n, sizeof(int), 1, fp);
    for (int i = 0; i < n; i++) {
        int index;
        fread(&index, sizeof(int), 1, fp);
        node->buckets[index] =
            list_load_with_size(fp, (void* (*)(FILE*))trie_load, 0);
    }
    return node;
}
