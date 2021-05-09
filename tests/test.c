#include <linked_list.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <trie.h>
#include <wctype.h>
#include <hashtable.h>

char* string_serializer(void* value) {
    char* str = (char*)value;
    size_t alloc_size = sizeof(char) * (strlen(str) + 1);
    char* data = (char*)malloc(alloc_size);
    memcpy(data, value, alloc_size);
    return data;
}

void trie_test() {
    TrieNode* root = create_trie_node();
    FILE* fp = fopen("../data/lusiadas.txt", "r");
    trie_load_dict(root, fp);

    char* input_val = "of";

    char* input = (char*)malloc(sizeof(input_val));
    memcpy(input, input_val, sizeof(input));

    Node* list = prefix_search(root, input);
    printf("%s\n", list_serialize(list, string_serializer));
}

void hashtable_test() {
     FILE *dict = fopen("../data/lusiadas.txt","r");
     HashTable *ht = init();
     char buffer[64];
     while (fscanf(dict, "%s", buffer) != EOF) {
          if (str_scan(buffer, 64)) {
              str_to_lower(buffer);
              put(ht, buffer);
          }
          buffer[0]='\0';
     }
     fclose(dict);

     printf("|minha| = 33\tget(hashtable, \"palavras\") = %d\n", get(ht,"palavras"));  //get(ht,"minha") dá uma a mais do q devia
}

int main(int argc, char const* argv[]) {
    trie_test();
    hashtable_test();

    return 0;
}
