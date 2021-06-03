#include <stdlib.h>
#include <string.h>
#include <t9_keys.h>

const char* t9_key_map[10] = {" ",   "",        "abcàáãâç", "defèéê", "ghiìíî",
                              "jkl", "mnoòóõô", "pqrs",     "tuvùúû", "wxyz"};

void get_permutations_util(Node** list, char* input, int input_len,
                           int cur_index, char output[], int offset) {
    if (cur_index == input_len) {
        output[input_len + offset] = '\0';
        size_t alloc_size = sizeof(char) * (input_len + offset + 1);

        *list = list_prepend(*list, output, alloc_size);
        return;
    }

    int number = input[cur_index] - '0';

    for (int i = 0; i < strlen(t9_key_map[number]); i++) {
        output[cur_index + offset] = t9_key_map[number][i];
        int offset_inc = 0;
        if (output[cur_index + offset] == -61) {
            i++;
            offset_inc = 1;
            output[cur_index + offset + offset_inc] = t9_key_map[number][i];
        }
        get_permutations_util(list, input, input_len, cur_index + 1, output,
                              offset + offset_inc);
    }
}

/**
 * Gets permutations for key presses (on T9 mode)
 * Returns a linked list
 **/
Node* get_permutations(char* input) {
    Node* list = NULL;
    int input_len = strlen(input);
    char output[input_len * 2 + 1];
    output[input_len * 2] = '\0';
    get_permutations_util(&list, input, input_len, 0, output, 0);
    return list;
}

/**
 * Runs the T9 algorithm. Returns the ordered list of words
 **/
Node* run_t9(TrieNode* trie, HashTable* ht, char* input) {
    Node* permutations = get_permutations(input);
    Node* curr = permutations;

    Node* result = NULL;

    while (curr != NULL) {
        char* prefix = curr->value;

        Node* new = prefix_search(trie, prefix, ht, result);

        if (new != NULL) result = new;

        free(prefix);
        Node* prev = curr;
        curr = curr->next;
        free(prev);
    }

    return result;
}