#include <int_hashtable.h>
#include <linked_list.h>
#include <stdlib.h>
#include <string.h>
#include <t9_keys.h>
#include <trie.h>

char* string_serializer(void* value) {
    char* str = (char*)value;
    size_t alloc_size = sizeof(char) * (strlen(str) + 1);
    char* data = (char*)malloc(alloc_size);
    memcpy(data, value, alloc_size);
    return data;
}

TrieNode* trie_test(HashTable* ht) {
    TrieNode* root = create_trie_node();
    FILE* fp = fopen("../data/lusiadas.txt", "r");
    trie_load_dict(root, fp);

    char* input_val = "aju";

    char* input = (char*)malloc(sizeof(input_val));
    memcpy(input, input_val, sizeof(input_val));

    Node* list = prefix_search(root, input, ht, NULL);
    printf("%s\n", list_serialize(list, string_serializer));

    return root;
}

HashTable* hashtable_test() {
    FILE* dict = fopen("../data/lusiadas.txt", "r");
    HashTable* ht = hashtable_create();
    char buffer[64];
    while (fscanf(dict, "%s", buffer) != EOF) {
        str_normalize(buffer);
        if (str_scan(buffer, 64)) {
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

    return ht;
}

void interactive_test(TrieNode* trie, HashTable* ht) {
    while (1) {
        char buffer[60];
        printf("Insert Prefix: ");
        scanf("%s", &buffer);
        Node* list = prefix_search(trie, buffer, ht, NULL);
        printf("%s\n", list_serialize(list, string_serializer));
    }
}

void t9_keys_test() {
    printf("6665552 - %s\n", get_word("6665552"));
    printf("(perm) 652 - %s\n",
           list_serialize(get_permutations("652"), string_serializer));
}

int main(int argc, char const* argv[]) {
    HashTable* ht = hashtable_test();
    putchar('\n');
    TrieNode* trie = trie_test(ht);
    putchar('\n');
    t9_keys_test();
    putchar('\n');

    interactive_test(trie, ht);

    return 0;
}
