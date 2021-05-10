#include <int_hashtable.h>
#include <linked_list.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <trie.h>
#include <wctype.h>

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
    FILE* dict = fopen("../data/lusiadas.txt", "r");
    HashTable* ht = hashtable_create();
    char buffer[64];
    while (fscanf(dict, "%s", buffer) != EOF) {
        if (str_scan(buffer, 64)) {
            str_to_lower(buffer);
            int curr_val = hashtable_get(ht, buffer);
            if (curr_val == -1) curr_val = 0;
            hashtable_put(ht, buffer, curr_val + 1);
        }
        buffer[0] = '\0';
    }
    fclose(dict);

    printf("|minha| = %d\n", hashtable_get(ht, "minha"));
    printf("|palavras| = %d\n", hashtable_get(ht, "palavras"));
    printf("|armas| = %d\n", hashtable_get(ht, "armas"));
}

int main(int argc, char const* argv[]) {
    trie_test();
    putchar('\n');
    hashtable_test();

    return 0;
}
