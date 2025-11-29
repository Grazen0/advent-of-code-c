#ifndef AOC_LIB_STR_H
#define AOC_LIB_STR_H

#include <stddef.h>
#include <stdlib.h>

typedef struct String {
    char *data;
    size_t size;
    size_t capacity;
} String;

String str_new(void);

void str_destroy(String *str);

String str_with_capacity(size_t initial_capacity);

String str_from(const char *data);

void str_push(String *str, char ch);

void str_push_str(String *str, String other);

void str_push_raw(String *str, const char *other);

String str_clone(String str);

void str_clear(String *str);

#endif
