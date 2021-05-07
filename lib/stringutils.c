#include <ctype.h>
#include <stringutils.h>

void str_to_lower(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}