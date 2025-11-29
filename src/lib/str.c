#include "lib/str.h"
#include "lib/macros.h"
#include "lib/numeric.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void String_reallocate(String *const str, const size_t new_capacity)
{
    char *const new_data =
        realloc(str->data, (new_capacity + 1) * sizeof(*new_data));

    PANIC_IF(new_data == NULL, "Could not reallocate string");

    str->data = new_data;
    str->capacity = new_capacity;
}

String str_new(void)
{
    return str_with_capacity(0);
}

String str_with_capacity(const size_t initial_capacity)
{
    String s = {
        .data = malloc((initial_capacity + 1) * sizeof(*s.data)),
        .size = 0,
        .capacity = initial_capacity,
    };

    s.data[0] = '\0';
    return s;
}

String str_from(const char *const data)
{
    if (data == NULL)
        return str_new();

    const size_t size = strlen(data);

    String s = {
        .data = malloc((size + 1) * sizeof(*data)),
        .size = size,
        .capacity = size,
    };

    memcpy(s.data, data, (size + 1) * sizeof(*data));
    return s;
}

void str_push(String *const str, const char ch)
{
    if (str->size == str->capacity)
        String_reallocate(str, str->capacity == 0 ? 1 : 2 * str->capacity);

    str->data[str->size] = ch;
    ++str->size;
    str->data[str->size] = '\0';
}

void str_push_str(String *const str, const String other)
{
    if (other.size == 0)
        return;

    if (str->size + other.size >= str->capacity) {
        const size_t new_capacity =
            sz_max(2 * str->capacity, str->size + other.size);
        String_reallocate(str, new_capacity);
    }

    memcpy(str->data + str->size, other.data, other.size * sizeof(*other.data));

    str->size += other.size;
    str->data[str->size] = '\0';
}

void str_push_raw(String *const str, const char *const other)
{
    const size_t other_size = strlen(other);

    if (other_size == 0)
        return;

    if (str->size + other_size > str->capacity) {
        const size_t new_capacity =
            sz_max(2 * str->capacity, str->size + other_size);
        String_reallocate(str, new_capacity);
    }

    memcpy(str->data + str->size, other, other_size * sizeof(*other));

    str->size += other_size;
    str->data[str->size] = '\0';
}

String str_clone(const String str)
{
    String out = str_with_capacity(str.size);
    str_push_str(&out, str);
    return out;
}

void str_clear(String *const str)
{
    free(str->data);

    str->data = malloc(1);
    str->data[0] = '\0';
    str->size = 0;
    str->capacity = 0;
}

void str_destroy(String *const str)
{
    free(str->data);
    str->data = NULL;
    str->capacity = 0;
    str->size = 0;
}
