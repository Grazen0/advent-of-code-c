#include "lib/vec.h"
#include "lib/macros.h"
#include <stddef.h>
#include <stdlib.h>

static const size_t VEC_INITIAL_CAPACITY = 1;

Vec vec_new(void)
{
    return (Vec){
        .data = NULL,
        .capacity = 0,
        .size = 0,
    };
}

void vec_destroy(Vec *const vec)
{
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
}

void vec_push(Vec *const vec, const int data)
{
    if (vec->size >= vec->capacity) {
        const size_t new_capacity =
            vec->capacity == 0 ? VEC_INITIAL_CAPACITY : 2 * vec->capacity;

        int *const new_data =
            realloc(vec->data, new_capacity * sizeof(*new_data));
        if (new_data == NULL)
            PANIC("could not reallocate vector");

        vec->data = new_data;
        vec->capacity = new_capacity;
    }

    vec->data[vec->size] = data;
    ++vec->size;
}

void vec_pop(Vec *const vec)
{
    if (vec->size == 0)
        PANIC("cannot pop empty vector");

    --vec->size;

    if (vec->size <= vec->capacity / 2) {
        const size_t new_capacity = vec->capacity / 2;

        int *const new_data = realloc(vec->data, new_capacity);
        if (new_data == NULL)
            PANIC("could not reallocate vector");

        vec->data = new_data;
        vec->capacity = new_capacity;
    }
}
