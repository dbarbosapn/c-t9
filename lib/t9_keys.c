#include <stdlib.h>
#include <string.h>
#include <t9_keys.h>

const char* t9_key_map[10] = {" ",   "",    "abc",  "def", "ghi",
                              "jkl", "mno", "pqrs", "tuv", "wxyz"};

/**
 * Gets word for key presses (not T9 mode)
 **/
char* get_word(char* input) {
    char curr_char = input[0];
    int counter = 0;

    char word_buffer[60];
    int word_size = 0;

    for (int i = 0; input[i]; i++) {
        if (input[i] == curr_char)
            counter++;
        else {
            word_buffer[word_size] = t9_key_map[curr_char - '0'][counter - 1];
            word_size++;
            counter = 1;
            curr_char = input[i];
        }
    }

    word_buffer[word_size] = t9_key_map[curr_char - '0'][counter - 1];
    word_buffer[word_size + 1] = '\0';

    char* result = (char*)malloc((word_size + 1) * sizeof(char));
    strcpy(result, word_buffer);

    return result;
}

void get_permutations_util(Node** list, char* input, int input_len,
                           int cur_index, char output[]) {
    if (cur_index == input_len) {
        size_t alloc_size = sizeof(char) * (input_len + 1);
        *list = list_prepend(*list, output, alloc_size);
        return;
    }

    int number = input[cur_index] - '0';

    for (int i = 0; i < strlen(t9_key_map[number]); i++) {
        output[cur_index] = t9_key_map[number][i];
        get_permutations_util(list, input, input_len, cur_index + 1, output);
    }
}

/**
 * Gets permutations for key presses (on T9 mode)
 * Returns a linked list
 **/
Node* get_permutations(char* input) {
    Node* list = NULL;
    int input_len = strlen(input);
    char output[input_len + 1];
    output[input_len] = '\0';
    get_permutations_util(&list, input, input_len, 0, output);
    return list;
}