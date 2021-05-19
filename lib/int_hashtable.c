#include <int_hashtable.h>
#include <stdlib.h>
#include <string.h>

/**
 * Creates a integer hashtable
 **/
HashTable *hashtable_create() {
    HashTable *hashtable = (HashTable *)malloc(sizeof(HashTable));

    for (int i = 0; i < HASHSIZE; i++) {
        hashtable->chains[i] = NULL;  // malloc = segfault
    }

    return hashtable;
}

/**
 * djb2 hash algorithm
 * http://www.cse.yorku.ca/~oz/hash.html
 **/
int hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) hash = ((hash << 5) + hash) + c;

    return hash % HASHSIZE;
}

/**
 * Inserts the value in the hashtable
 **/
void hashtable_put(HashTable *hashtable, char *key, int value) {
    int h = hash(key);
    HashNode *n;

    for (n = hashtable->chains[h]; n != NULL; n = n->next) {
        if (strcmp(n->key, key) == 0) break;
    }

    if (n != NULL) {
        n->value = value;
    } else {
        HashNode *node = malloc(sizeof(HashNode));
        node->value = value;
        size_t key_len = sizeof(char) * strlen(key);
        node->key = (char *)malloc(key_len);
        memcpy(node->key, key, key_len);
        node->next = hashtable->chains[h];
        hashtable->chains[h] = node;
    }
}

/**
 * Gets a value from the table from the given key
 **/
int hashtable_get(HashTable *hashtable, char *key) {
    int h = hash(key);
    HashNode *n;

    for (n = hashtable->chains[h]; n != NULL; n = n->next) {
        if (strcmp(n->key, key) == 0) break;
    }
    if (n != NULL) return n->value;

    return -1;
}

void hashtable_save(HashTable *ht, FILE *fp) {
    for (int i = 0; i < HASHSIZE; i++) {
        int chain_size = 0;
        if (ht->chains[i] != NULL) {
            HashNode *curr_node = ht->chains[i];
            while (curr_node != NULL) {
                chain_size++;
                curr_node = curr_node->next;
            }
            fwrite(&chain_size, sizeof(int), 1, fp);
            curr_node = ht->chains[i];
            while (curr_node != NULL) {
                int str_size = strlen(curr_node->key);
                fwrite(&str_size, sizeof(int), 1, fp);
                fwrite(curr_node->key, sizeof(char), str_size, fp);
                fwrite(&curr_node->value, sizeof(int), 1, fp);
                curr_node = curr_node->next;
            }
        } else {
            fwrite(&chain_size, sizeof(int), 1, fp);
        }
    }
}

HashTable *hashtable_load(FILE *fp) {
    HashTable *ht = hashtable_create();
    for (int i = 0; i < HASHSIZE; i++) {
        int chain_size = 0;
        fread(&chain_size, sizeof(int), 1, fp);
        HashNode *prev = NULL;
        for (int j = 0; j < chain_size; j++) {
            HashNode *node = (HashNode *)malloc(sizeof(HashNode));
            int str_size;
            fread(&str_size, sizeof(int), 1, fp);
            char *key = (char *)malloc(sizeof(char) * (str_size + 1));
            fread(key, sizeof(char), str_size, fp);
            key[str_size] = '\0';
            node->key = key;
            fread(&node->value, sizeof(int), 1, fp);

            if (prev == NULL) {
                ht->chains[i] = node;
                prev = node;
            } else {
                prev->next = node;
                prev = node;
            }
        }
    }

    return ht;
}