#ifndef AOC_LIB_VEC_H
#define AOC_LIB_VEC_H

#include <stddef.h>

typedef struct {
    int *data;
    size_t capacity;
    size_t size;
} Vec;

Vec vec_new(void);

void vec_destroy(Vec *vec);

void vec_push(Vec *vec, int data);

void vec_pop(Vec *vec);

#endif
