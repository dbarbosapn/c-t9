#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <hashtable.h>

#define HASHSIZE 16381


int hash(char *str)
{
     unsigned long hash = *str++;
     int c;

     while ((c = *str++))
     hash = (hash*251 + c) % HASHSIZE;

     return hash;
}

void put(HashTable* hashtable, char *str) {
     int h = hash(str);
     HashNode *n;

     for (n = hashtable->chains[h]; n != NULL; n = n->next) {
          if (strcmp(n->key, str) == 0) break;
     }
     if (n != NULL) {
          n->value++;
     }
     else {
          HashNode *node = malloc(sizeof (HashNode));
          node->value = 1;
          node->key = malloc(sizeof(char)*(65));
          strcpy(node->key,str);                    //TODO: strcpy is unsafe
          node->next = hashtable->chains[h];
          hashtable->chains[h] = node;
     }
}

int get(HashTable *hashtable, char *key)
{
     int h = hash(key);
     HashNode *n;

     for (n = hashtable->chains[h]; n != NULL; n = n->next) {
          if (strcmp(n->key, key) == 0) break;
     }
     if (n != NULL) return n->value;
     return 0;
}

HashTable *init()
{
     HashTable *hashtable = (HashTable *)malloc(sizeof(HashTable));

     for (int i = 0; i < HASHSIZE; i++)
     {
          hashtable->chains[i] = NULL; //malloc = segfault
     }
     return hashtable;
}