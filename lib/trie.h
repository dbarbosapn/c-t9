#define NUM_CHARS 26

typedef struct trie_node {
    int is_word;
    TrieNode* children[NUM_CHARS];
} TrieNode;

TrieNode* create_node();

void insert_word(TrieNode* root, char* word);