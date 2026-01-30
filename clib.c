#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROJECT_NAME "clib"

typedef struct {
    char *data;
    size_t len;
} String;

String *clib_string_create(char *data) {
    String *str = malloc(sizeof(String));
    assert(str != NULL);
    size_t len = strlen(data);
    char *buff = malloc(len + 1);
    assert(buff != NULL);
    strncpy(buff, data, len);

    if (strlen(data) > 0)
        buff[len] = '\0';

    str->data = buff;
    str->len = len;

    return str;
}

size_t clib_string_len(String *str) { return str->len; }

char clib_string_char_at(String *str, size_t pos) {
    assert(pos < clib_string_len(str));
    return str->data[pos];
}

String *clib_string_to_lowercase(String *str) {
    for (size_t i = 0; i < clib_string_len(str); i++)
        str->data[i] = tolower(str->data[i]);
    return str;
}

String *clib_string_to_uppercase(String *str) {
    for (size_t i = 0; i < clib_string_len(str); i++)
        str->data[i] = toupper(str->data[i]);
    return str;
}

String *clib_string_title(String *str) {
    if (clib_string_len(str) > 0) {
        str->data[0] = toupper(str->data[0]);
    }
    for (size_t i = 0; i < clib_string_len(str); i++) {
        if (i - 1 > 0 && str->data[i - 1] == ' ') {
            str->data[i] = toupper(str->data[i]);
        }
    }
    return str;
}

String *clib_string_left_trim(String *str) {
    for (size_t i = 0; i < clib_string_len(str); i++) {
        if (!isspace(str->data[i])) {
            str->data += i;
            str->len -= i;
            break;
        }
    }
    return str;
}

String *clib_string_right_trim(String *str) {
    for (int i = clib_string_len(str); i > 0; i--) {
        if (!isspace(str->data[i]) && str->data[i] != '\0') {
            str->data[clib_string_len(str) - i] = '\0';
            str->len -= (clib_string_len(str) - i - 1);
            break;
        }
    }
    return str;
}

String *clib_string_trim(String *str) {
    str = clib_string_left_trim(str);
    str = clib_string_right_trim(str);
    return str;
}

void clib_string_free(String *str) {
    assert(str != NULL);
    free(str->data);
    free(str);
}

void clib_string_print(String *str) {
    assert(str != NULL);
    printf("%s\n", str->data);
}

int main(int argc, char **argv) {
    if (argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }

    String *str = clib_string_create("         fuck off           ");
    str = clib_string_trim(str);
    clib_string_print(str);
    printf("%zu\n", clib_string_len(str));

    return 0;
}
