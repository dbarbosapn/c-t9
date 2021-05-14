#include <int_hashtable.h>
#include <simple_input.h>
#include <stdio.h>
#include <stringutils.h>
#include <t9_keys.h>
#include <trie.h>

/**
 * TODO: Add this comment
 **/
TrieNode* load_trie() {
    // TODO: Check if there is already a binary file with a trie loaded.
    // Otherwise, load from the dictionary.

    FILE* dict = fopen("data/lusiadas.txt", "r");
    TrieNode* result = create_trie_node();
    trie_load_dict(result, dict);
    fclose(dict);
    return result;
}

/**
 * TODO: Add this comment
 **/
HashTable* load_hashtable() {
    // TODO: Check if there is already a binary file with a hashtable loaded.
    // Otherwise, load from the dictionary.

    FILE* dict = fopen("data/lusiadas.txt", "r");
    HashTable* result = hashtable_create();

    char buffer[64];
    while (fscanf(dict, "%s", buffer) != EOF) {
        str_normalize(buffer);
        if (str_scan(buffer, 64)) {
            int curr_val = hashtable_get(result, buffer);
            if (curr_val == -1) curr_val = 0;
            hashtable_put(result, buffer, curr_val + 1);
        }
        buffer[0] = '\0';
    }
    fclose(dict);

    return result;
}

/**
 * TODO: Add this comment
 **/
Node* run_t9(TrieNode* trie, HashTable* ht, char* input) {
    Node* permutations = get_permutations(input);
    Node* curr = permutations;

    Node* result = NULL;

    while (curr != NULL) {
        char* prefix = curr->value;

        result = prefix_search(trie, prefix, ht, result);

        free(prefix);
        Node* prev = curr;
        curr = curr->next;
        free(prev);
    }

    return result;
}

int main(int argc, char const* argv[]) {
    TrieNode* trie = load_trie();
    HashTable* ht = load_hashtable();

    while (1) {
        printf("Insert the input (numeric 2-9): ");
        char* input = read_string_input();
        putchar('\n');

        Node* list = run_t9(trie, ht, input);
        free(input);

        Node* curr = list;
        while (curr != NULL) {
            if (curr != list) printf(", ");
            printf("%s", (char*)curr->value);

            free(curr->value);
            Node* prev = curr;
            curr = curr->next;
            free(prev);
        }

        printf("\n\n");
    }

    return 0;
}
