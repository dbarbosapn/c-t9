#include <linked_list.h>
#include <stdlib.h>
#include <string.h>
#include <trie.h>

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

    char* input_val = "ve";

    char* input = (char*)malloc(sizeof(input_val));
    memcpy(input, input_val, sizeof(input));

    Node* list = prefix_search(root, input);
    printf("%s\n", list_serialize(list, string_serializer));
}

int main(int argc, char const* argv[]) {
    trie_test();
    return 0;
}
