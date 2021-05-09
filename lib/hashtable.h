#define HASHSIZE 509

typedef struct HashNode {
    char *key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    HashNode *chains[HASHSIZE];
} HashTable;

int hash(char *str);

void put(HashTable *hashtable, char *str);

int get(HashTable *hashtable, char *key);

HashTable *init();
