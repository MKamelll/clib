#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
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

long long clib_string_find(String *str, String *sub) {
    size_t i = 0;
    size_t j = 0;

    while (i < clib_string_len(str)) {
        if (str->data[i] == sub->data[j]) {
            long long ii = i;
            while (j < clib_string_len(sub)) {
                if (str->data[ii] != sub->data[j]) {
                    j = 0;
                    break;
                }
                ii++;
                j++;
            }

            if (j >= clib_string_len(sub)) {
                return ii - clib_string_len(sub);
            }
        }
        i++;
    }

    return -1;
}

long long clib_string_rfind(String *str, String *sub) {
    long long i = clib_string_len(str) - 1;
    long long j = clib_string_len(sub) - 1;

    while (i >= 0) {
        if (str->data[i] == sub->data[j]) {
            long long ii = i;
            while (j >= 0) {
                if (str->data[ii] != sub->data[j]) {
                    j = clib_string_len(sub) - 1;
                    break;
                }
                ii--;
                j--;
            }

            if (j <= 0) {
                return ii + 1;
            }
        }
        i--;
    }

    return -1;
}

String *clib_string_substring(String *str, size_t start, size_t end) {
    str->data += start;
    str->len -= start;
    str->data[end] = '\0';
    str->len -= clib_string_len(str) - end;
    return str;
}

size_t clib_string_count(String *str, String *sub) {
    size_t count = 0;
    while (clib_string_len(str) != 0) {
        long long idx = clib_string_find(str, sub);
        if (idx == -1)
            break;
        count++;
        str = clib_string_substring(str, idx + clib_string_len(sub),
                                    clib_string_len(str));
    }
    return count;
}

bool clib_string_startswith(String *str, String *prefix) {
    return clib_string_find(str, prefix) == 0;
}

bool clib_string_endswith(String *str, String *suffix) {
    return (clib_string_rfind(str, suffix) + clib_string_len(suffix)) >=
           clib_string_len(str);
}

bool clib_string_is_alpha(String *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        bool is_lower_case = str->data[i] >= 'a' && str->data[i] <= 'z';
        bool is_upper_case = str->data[i] >= 'A' && str->data[i] <= 'Z';
        if (!is_lower_case && !is_upper_case)
            return false;
        i++;
    }
    return true;
}

bool clib_string_is_numeric(String *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        bool is_digit = str->data[i] >= '0' && str->data[i] <= '9';
        if (!is_digit)
            return false;
        i++;
    }
    return true;
}

bool clib_string_is_alphanumeric(String *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        bool is_lower_case = str->data[i] >= 'a' && str->data[i] <= 'z';
        bool is_upper_case = str->data[i] >= 'A' && str->data[i] <= 'Z';
        bool is_digit = str->data[i] >= '0' && str->data[i] <= '9';
        if (!is_digit && !is_lower_case && !is_upper_case)
            return false;
        i++;
    }
    return true;
}

bool clib_string_is_lower(String *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        if (!islower(str->data[i]))
            return false;
        i++;
    }
    return true;
}

bool clib_string_is_upper(String *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        if (!isupper(str->data[i]))
            return false;
        i++;
    }
    return true;
}

void clib_string_free(String *str) {
    assert(str != NULL);
    free(str->data);
    str->data = NULL;
    free(str);
    str = NULL;
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

    String *str = clib_string_create("TTTTOIIG");
    String *sub = clib_string_create("kff");
    clib_string_print(str);
    printf("%b\n", clib_string_is_upper(str));

    return 0;
}
