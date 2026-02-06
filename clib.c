#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROJECT_NAME "clib"
#define INITIAL_CAP 2
#define GROWTH_FACTOR 2

typedef struct {
    void *data;
    size_t elm_size;
    long long size;
    long long cap;
} ClibArray;

ClibArray *clib_array_create(size_t elm_size) {
    ClibArray *a = malloc(sizeof(ClibArray));
    if (!a) {
        fprintf(stderr, "%s\n", "ClibArray: Failed to allocate a new array");
        exit(1);
    }
    a->data = malloc(elm_size * INITIAL_CAP);
    if (!a->data) {
        free(a);
        fprintf(stderr, "%s\n", "ClibArray: Failed to allocate for data");
        exit(1);
    }
    a->elm_size = elm_size;
    a->size = 0;
    a->cap = INITIAL_CAP;
    return a;
}

void clib_array_push(ClibArray *arr, void *elm) {
    if (arr->size == arr->cap) {
        size_t new_cap = arr->cap * 2;
        void *new_data = realloc(arr->data, new_cap * arr->elm_size);
        if (!new_data) {
            fprintf(stderr, "%s\n", "ClibArray: Failed to rellocate the data");
            exit(1);
        }

        arr->cap = new_cap;
        arr->data = new_data;
    }

    void *new_elm_addr = (char *)arr->data + (arr->elm_size * arr->size);
    memcpy(new_elm_addr, elm, arr->elm_size);
    arr->size++;
}

void clib_array_pop(ClibArray *arr) {
    if (arr->size == 0) {
        fprintf(stderr, "%s\n", "ClibArray: Array is already empty");
        exit(1);
    }
    arr->data = (char *)arr->data - arr->elm_size;
    arr->size--;
}

void *clib_array_get(ClibArray *arr, long long index) {
    if (!arr) {
        fprintf(stderr, "%s\n", "ClibArray: Array is not initialized");
        exit(1);
    }
    if (index < 0 || index >= arr->size) {
        fprintf(stderr, "%s: '%lld' of array size: '%lld'\n",
                "ClibArray: Index is out of bounds", index, arr->size);
        exit(1);
    }
    void *elm = (char *)arr->data + (arr->elm_size * index);
    return elm;
}

void clib_array_free(ClibArray *arr) {
    if (arr) {
        free(arr->data);
        arr->data = NULL;
        free(arr);
    }
}

/* ---------- ClibString ------------ */
typedef struct {
    char *data;
    size_t len;
} ClibString;

ClibString *clib_string_create(char *data) {
    ClibString *str = malloc(sizeof(ClibString));
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

size_t clib_string_len(ClibString *str) { return str->len; }

char clib_string_char_at(ClibString *str, size_t pos) {
    assert(pos < clib_string_len(str));
    return str->data[pos];
}

ClibString *clib_string_to_lowercase(ClibString *str) {
    for (size_t i = 0; i < clib_string_len(str); i++)
        str->data[i] = tolower(str->data[i]);
    return str;
}

ClibString *clib_string_to_uppercase(ClibString *str) {
    for (size_t i = 0; i < clib_string_len(str); i++)
        str->data[i] = toupper(str->data[i]);
    return str;
}

ClibString *clib_string_title(ClibString *str) {
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

ClibString *clib_string_left_trim(ClibString *str) {
    for (size_t i = 0; i < clib_string_len(str); i++) {
        if (!isspace(str->data[i])) {
            str->data += i;
            str->len -= i;
            break;
        }
    }
    return str;
}

ClibString *clib_string_right_trim(ClibString *str) {
    for (int i = clib_string_len(str); i > 0; i--) {
        if (!isspace(str->data[i]) && str->data[i] != '\0') {
            str->data[clib_string_len(str) - i] = '\0';
            str->len -= (clib_string_len(str) - i - 1);
            break;
        }
    }
    return str;
}

ClibString *clib_string_trim(ClibString *str) {
    str = clib_string_left_trim(str);
    str = clib_string_right_trim(str);
    return str;
}

long long clib_string_find(ClibString *str, ClibString *sub) {
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

long long clib_string_rfind(ClibString *str, ClibString *sub) {
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

ClibString *clib_string_substring(ClibString *str, size_t start, size_t end) {
    str->data += start;
    str->len -= start;
    str->data[end] = '\0';
    str->len -= clib_string_len(str) - end;
    return str;
}

size_t clib_string_count(ClibString *str, ClibString *sub) {
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

bool clib_string_startswith(ClibString *str, ClibString *prefix) {
    return clib_string_find(str, prefix) == 0;
}

bool clib_string_endswith(ClibString *str, ClibString *suffix) {
    return (clib_string_rfind(str, suffix) + clib_string_len(suffix)) >=
           clib_string_len(str);
}

bool clib_string_is_alpha(ClibString *str) {
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

bool clib_string_is_numeric(ClibString *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        bool is_digit = str->data[i] >= '0' && str->data[i] <= '9';
        if (!is_digit)
            return false;
        i++;
    }
    return true;
}

bool clib_string_is_alphanumeric(ClibString *str) {
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

bool clib_string_is_lower(ClibString *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        if (!islower(str->data[i]))
            return false;
        i++;
    }
    return true;
}

bool clib_string_is_upper(ClibString *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        if (!isupper(str->data[i]))
            return false;
        i++;
    }
    return true;
}

bool clib_string_is_space(ClibString *str) {
    size_t i = 0;
    while (i < clib_string_len(str)) {
        if (!isspace(str->data[i]))
            return false;
        i++;
    }
    return true;
}

void clib_string_free(ClibString *str) {
    assert(str != NULL);
    free(str->data);
    str->data = NULL;
    free(str);
    str = NULL;
}

void clib_string_print(ClibString *str) {
    assert(str != NULL);
    printf("%s\n", str->data);
}

int main(int argc, char **argv) {
    if (argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }

    ClibArray *arr = clib_array_create(sizeof(ClibString));
    ClibString *str = clib_string_create("fuck off");
    clib_array_push(arr, str);
    clib_string_print((ClibString *)clib_array_get(arr, 1));

    return 0;
}
