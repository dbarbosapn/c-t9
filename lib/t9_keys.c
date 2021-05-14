#include <stdlib.h>
#include <string.h>
#include <t9_keys.h>

const char* t9_key_map[10] = {" ",   "",    "abc",  "def", "ghi",
                              "jkl", "mno", "pqrs", "tuv", "wxyz"};

/**
 * Gets word for key presses (not T9 mode)
 **/
char* get_word(char* input) {
    // TODO: This method is very unsafe to invalid input. (e.g. 111 or abc)

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

    char* result = (char*)malloc(word_size + 1);
    strcpy(result, word_buffer);

    return result;
}

/**
 * Gets permutations for key presses (on T9 mode)
 * Returns a linked list
 **/
Node* get_permutations(char* input) {
    // TODO: Implement
}