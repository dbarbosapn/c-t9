#include <ctype.h>
#include <string.h>
#include <stringutils.h>

void str_normalize(char* str) {
    // TODO: Normalize accents too (?)
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int str_scan(char* buffer, int buf_size) {
    char new_buf[buf_size];
    int str_size = 0;

    for (int i = 0; buffer[i]; i++) {
        if (isalpha(buffer[i])) {
            new_buf[str_size] = buffer[i];
            str_size++;
        }
    }
    new_buf[str_size] = '\0';

    if (str_size > 0) {
        strcpy(buffer, new_buf);
        return 1;
    }

    return 0;
}