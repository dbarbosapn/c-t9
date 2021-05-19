#include <int_hashtable.h>
#include <simple_input.h>
#include <stdio.h>
#include <string.h>
#include <stringutils.h>
#include <t9_keys.h>
#include <trie.h>
#include <unistd.h>

int file_exists(char* filename) { return access(filename, F_OK) == 0; }

/**
 * Loads the trie from file. If not available, creates a new one from the
 * dictionary
 **/
TrieNode* load_trie() {
    if (!file_exists("data/trie.bin")) {
        FILE* dict = fopen("data/lusiadas.txt", "r");
        TrieNode* result = create_trie_node();
        trie_load_dict(result, dict);
        fclose(dict);
        return result;
    } else {
        FILE* fp = fopen("data/trie.bin", "rb");
        TrieNode* trie = trie_load(fp);
        fclose(fp);
        return trie;
    }
}

/**
 * Loads the hashtable from file. If not available, creates a new one from the
 * dictionary
 **/
HashTable* load_hashtable() {
    if (!file_exists("data/hashtable.bin")) {
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
    } else {
        FILE* fp = fopen("data/hashtable.bin", "rb");
        HashTable* result = hashtable_load(fp);
        fclose(fp);

        return result;
    }
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

        // For debug only
        if (strcmp(input, "quit") == 0) {
            FILE* fp = fopen("data/trie.bin", "wb");
            trie_save(trie, fp);
            fclose(fp);
            FILE* fp2 = fopen("data/hashtable.bin", "wb");
            hashtable_save(ht, fp2);
            fclose(fp2);
            return 0;
        }
        putchar('\n');

        Node* list = run_t9(trie, ht, input);
        free(input);

        Node* curr = list;
        while (curr != NULL) {
            if (curr != list) printf(", ");
            printf("%s: %d", (char*)curr->value,
                   hashtable_get(ht, (char*)curr->value));

            free(curr->value);
            Node* prev = curr;
            curr = curr->next;
            free(prev);
        }

        printf("\n\n");
    }

    return 0;
}
