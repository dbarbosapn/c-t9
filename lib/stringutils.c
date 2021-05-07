#include <ctype.h>
#include <stringutils.h>

void str_to_lower(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int str_is_word(char* str) {
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i])) return 0;
    }

    return 1;
}