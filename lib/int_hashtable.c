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

    while (c = *str++) hash = ((hash << 5) + hash) + c;

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
        node->key = (char *)malloc(sizeof(key));
        memcpy(node->key, key, sizeof(key));
        node->next = hashtable->chains[h];
        hashtable->chains[h] = node;
    }
}

/**
 * Gets a value from the table from the given key
 **/
void *hashtable_get(HashTable *hashtable, char *key) {
    int h = hash(key);
    HashNode *n;

    for (n = hashtable->chains[h]; n != NULL; n = n->next) {
        if (strcmp(n->key, key) == 0) break;
    }
    if (n != NULL) return n->value;

    return -1;
}
