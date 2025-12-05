#include "lib/vec.h"
#include "lib/macros.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static constexpr size_t VEC_INIT_CAPACITY = 16;

static void vec_resize(VecInternal *const vec, const size_t new_capacity, const size_t item_size)
{
    u8 *const new_data = realloc(vec->data, new_capacity * item_size);
    PANIC_IF(new_data == nullptr, "could not reallocate vector");

    vec->data = new_data;
    vec->capacity = new_capacity;
}

VecInternal __vec_with_capacity(const size_t initial_capacity, const size_t item_size)
{
    if (initial_capacity == 0)
        return __vec_new();

    return (VecInternal){
        .data = malloc(initial_capacity * item_size),
        .capacity = initial_capacity,
        .size = 0,
    };
}

VecInternal __vec_filled(const size_t initial_size, const void *const fill_value,
                         const size_t item_size)
{
    if (initial_size == 0)
        return __vec_new();

    const size_t alloc_size = initial_size * item_size;
    u8 *const data = malloc(alloc_size);

    for (size_t i = 0; i < initial_size; i += item_size)
        memcpy(&data[i], fill_value, item_size);

    return (VecInternal){
        .data = data,
        .capacity = initial_size,
        .size = initial_size,
    };
}

VecInternal __vec_new(void)
{
    return (VecInternal){
        .data = nullptr,
        .size = 0,
        .capacity = 0,
    };
}

void __vec_destroy(VecInternal *const vec)
{
    free(vec->data);
    vec->data = nullptr;
    vec->capacity = 0;
    vec->size = 0;
}

size_t __vec_size(VecInternal *const vec)
{
    return vec->size;
}

size_t __vec_capacity(VecInternal *const vec)
{
    return vec->capacity;
}

void *__vec_data(VecInternal *const vec)
{
    return vec->data;
}

void *__vec_get(VecInternal *const vec, const size_t idx, const size_t item_size)
{
    PANIC_IF(idx >= vec->size, "index out of bounds");

    const size_t base = idx * item_size;
    return &vec->data[base];
}

void *__vec_first(VecInternal *const vec, const size_t item_size)
{
    return __vec_get(vec, 0, item_size);
}

void *__vec_last(VecInternal *const vec, const size_t item_size)
{
    return __vec_get(vec, vec->size - 1, item_size);
}

void __vec_push(VecInternal *const vec, const void *value, const size_t item_size)
{
    if (vec->size >= vec->capacity)
        vec_resize(vec, vec->capacity ? (2 * vec->capacity) : VEC_INIT_CAPACITY, item_size);

    const size_t base = item_size * vec->size;
    memcpy(&vec->data[base], value, item_size);
    ++vec->size;
}

void __vec_insert(VecInternal *const vec, const size_t idx, const int value, const size_t item_size)
{
    if (vec->size >= vec->capacity)
        vec_resize(vec, vec->capacity ? (2 * vec->capacity) : VEC_INIT_CAPACITY, item_size);

    ++vec->size;

    for (size_t i = idx + 1; i < vec->size - 1; ++i)
        memcpy(&vec->data[i * item_size], &vec->data[(i - 1) * item_size], item_size);

    vec->data[idx] = value;
}

void __vec_pop(VecInternal *const vec, const size_t item_size)
{
    PANIC_IF(vec->size == 0, "cannot pop empty vector");

    --vec->size;

    if (vec->size <= vec->capacity / 2)
        vec_resize(vec, vec->capacity / 2, item_size);
}

void __vec_remove(VecInternal *const vec, const size_t idx, const size_t item_size)
{
    if (idx >= vec->size)
        PANIC("index out of bounds");

    for (size_t i = idx; i < vec->size - 1; ++i)
        memcpy(&vec->data[i * item_size], &vec->data[(i + 1) * item_size], item_size);

    --vec->size;

    if (vec->size <= vec->capacity / 2)
        vec_resize(vec, vec->capacity / 2, item_size);
}
