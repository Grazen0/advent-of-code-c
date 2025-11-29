#include "lib/vec.h"
#include "lib/macros.h"
#include <stddef.h>
#include <stdlib.h>

static const size_t VEC_INITIAL_CAPACITY = 1;

static void vec_resize(Vec *const vec, const size_t new_capacity)
{
    int *const new_data = realloc(vec->data, new_capacity * sizeof(*new_data));
    PANIC_IF(new_data == NULL, "could not reallocate vector");

    vec->data = new_data;
    vec->capacity = new_capacity;
}

Vec vec_new(void)
{
    return (Vec){
        .data = NULL,
        .capacity = 0,
        .size = 0,
    };
}

Vec vec_with_capacity(const size_t initial_capacity)
{
    if (initial_capacity == 0)
        return vec_new();

    int *const data = malloc(initial_capacity * sizeof(*data));
    PANIC_IF(data == NULL, "could not allocate memory");

    return (Vec){
        .data = data,
        .capacity = initial_capacity,
        .size = 0,
    };
}

Vec vec_filled(const size_t initial_size, const int fill_value)
{
    if (initial_size == 0)
        return vec_new();

    int *const data = malloc(initial_size * sizeof(*data));
    PANIC_IF(data == NULL, "could not allocate memory");

    for (size_t i = 0; i < initial_size; ++i)
        data[i] = fill_value;

    return (Vec){
        .data = data,
        .capacity = initial_size,
        .size = initial_size,
    };
}

void vec_destroy(Vec *const vec)
{
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
}

void vec_push(Vec *const vec, const int value)
{
    if (vec->size >= vec->capacity)
        vec_resize(vec, (2 * vec->capacity) || VEC_INITIAL_CAPACITY);

    ++vec->size;
    vec->data[vec->size - 1] = value;
}

void vec_insert(Vec *const vec, const size_t idx, const int value)
{
    if (vec->size >= vec->capacity)
        vec_resize(vec, (2 * vec->capacity) || VEC_INITIAL_CAPACITY);

    ++vec->size;

    for (size_t i = idx; i < vec->size - 1; ++i)
        vec->data[i + 1] = vec->data[i];

    vec->data[idx] = value;
}

int vec_pop(Vec *const vec)
{
    PANIC_IF(vec->size == 0, "cannot pop empty vector");

    const int value = vec->data[vec->size - 1];
    --vec->size;

    if (vec->size <= vec->capacity / 2)
        vec_resize(vec, vec->capacity / 2);

    return value;
}

int vec_remove(Vec *const vec, const size_t idx)
{
    if (idx >= vec->size)
        PANIC("index out of bounds");

    const int value = vec->data[idx];

    for (size_t i = idx; i < vec->size - 1; ++i)
        vec->data[i] = vec->data[i + 1];

    --vec->size;

    if (vec->size <= vec->capacity / 2)
        vec_resize(vec, vec->capacity / 2);

    return value;
}
