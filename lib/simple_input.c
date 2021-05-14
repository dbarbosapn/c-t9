#include <stdio.h>
#include <stdlib.h>

/**
 * Reads a string from the input, ended by a line break. Returns a pointer to
 * the string.
 * NOTE: Since 'realloc' uses a lazy strategy where it tries to grow the
 * space instead of copying everything and allocating again, and since we're
 * running it in a loop with no other allocations, it will be efficient.
 **/
char* read_string_input() {
    char* str = malloc(sizeof(char));
    int counter = 0;

    char c;
    while ((c = getchar()) != '\n') {
        str = realloc(str, (counter + 2) * sizeof(char));
        str[counter] = c;
        counter++;
    }
    str[counter] = '\0';

    return str;
}

void clear_input() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Do nothing
    }
}