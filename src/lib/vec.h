#ifndef AOC_LIB_VEC_H
#define AOC_LIB_VEC_H

#include "lib/numeric.h"
#include <stddef.h>

#define Vec(type)             \
    union {                   \
        VecInternal internal; \
        type *payload;        \
    }

#define vec_new() {.internal = __vec_new()}

#define vec_destroy(vec) __vec_destroy(&(vec)->internal)

#define vec_data_size(vec) (sizeof(*(vec)->payload))

#define vec_size(vec) __vec_size(&(vec)->internal)

#define vec_is_empty(vec) __vec_is_empty(&(vec)->internal)

#define vec_capacity(vec) __vec_capacity(&(vec)->internal)

#define vec_data(vec) ((typeof((vec)->payload))__vec_data(&(vec)->internal))

#define vec_get(vec, idx) \
    ((typeof((vec)->payload))__vec_get(&(vec)->internal, idx, sizeof(*(vec)->payload)))

#define vec_get_unchecked(vec, idx) \
    ((typeof((vec)->payload))__vec_get_unchecked(&(vec)->internal, idx, sizeof(*(vec)->payload)))

#define vec_first(vec) ((typeof((vec)->payload))__vec_first(&(vec)->internal))

#define vec_last(vec) \
    ((typeof((vec)->payload))__vec_last(&(vec)->internal, sizeof(*(vec)->payload)))

#define vec_push(vec, value) \
    __vec_push(&(vec)->internal, true ? (value) : (vec)->payload, sizeof(*(vec)->payload))

#define vec_insert(vec, idx, value) \
    __vec_insert(&(vec)->internal, idx, true ? (value) : (vec)->payload, sizeof(*(vec)->payload))

#define vec_pop(vec, out) \
    __vec_pop(&(vec)->internal, true ? (out) : (vec)->payload, sizeof(*(vec)->payload))

#define vec_remove(vec, idx, out) \
    __vec_remove(&(vec)->internal, idx, true ? (out) : (vec)->payload, sizeof(*(vec)->payload))

#define vec_reserve(vec, capacity) \
    __vec_reserve(&(vec)->internal, (capacity), sizeof(*(vec)->payload))

#define vec_resize(vec, size) __vec_resize(&(vec)->internal, (size), sizeof(*(vec)->payload))

#define vec_clear(vec) __vec_clear(&(vec)->internal)

#define vec_for(vec, item)                                                               \
    for (typeof((vec)->payload) item = (typeof((vec)->payload))&(vec)->internal.data[0]; \
         item != &((typeof((vec)->payload))(vec)->internal.data)[(vec)->internal.size]; ++item)

#define vec_qsort(vec, cmp) qsort(vec_data(vec), vec_size(vec), vec_data_size(vec), cmp)

typedef struct {
    u8 *data;
    size_t capacity;
    size_t size;
} VecInternal;

VecInternal __vec_new(void);

void __vec_destroy(VecInternal *vec);

size_t __vec_size(const VecInternal *vec);

bool __vec_is_empty(const VecInternal *vec);

size_t __vec_capacity(const VecInternal *vec);

void *__vec_data(VecInternal *vec);

void *__vec_get(VecInternal *vec, size_t idx, size_t item_size);

void *__vec_get_unchecked(VecInternal *vec, size_t idx, size_t item_size);

void *__vec_first(VecInternal *vec);

void *__vec_last(VecInternal *vec, size_t item_size);

void __vec_push(VecInternal *vec, const void *value, size_t item_size);

void __vec_insert(VecInternal *vec, size_t idx, int value, size_t item_size);

void __vec_pop(VecInternal *vec, void *out, size_t item_size);

void __vec_remove(VecInternal *vec, size_t idx, size_t item_size);

void __vec_reserve(VecInternal *vec, size_t capacity, size_t item_size);

void __vec_resize(VecInternal *vec, size_t size, size_t item_size);

void __vec_clear(VecInternal *vec);

#endif
