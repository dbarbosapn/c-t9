#ifndef _INT_HASHTABLE
#define _INT_HASHTABLE

#include <stdio.h>

#define HASHSIZE 509

typedef struct HashNode {
    char *key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    HashNode *chains[HASHSIZE];
} HashTable;

HashTable *hashtable_create();

int hash(char *str);

void hashtable_put(HashTable *hashtable, char *key, int value);

int hashtable_get(HashTable *hashtable, char *key);

void hashtable_save(HashTable *ht, FILE *fp);

HashTable *hashtable_load(FILE *fp);

#endif