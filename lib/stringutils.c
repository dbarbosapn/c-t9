#include <ctype.h>
#include <string.h>
#include <stringutils.h>

void str_normalize(char* str) {
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
        } else if (buffer[i] == -61) {
            new_buf[str_size] = buffer[i];
            new_buf[str_size + 1] = buffer[i + 1];
            str_size += 2;
        }
    }
    new_buf[str_size] = '\0';

    if (str_size > 0) {
        strcpy(buffer, new_buf);
        return 1;
    }

    return 0;
}
