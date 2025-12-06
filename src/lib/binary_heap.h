#ifndef AOC_LIB_BINARY_HEAP_H
#define AOC_LIB_BINARY_HEAP_H

#include "lib/vec.h"
#include <stddef.h>

#define BinaryHeap(type)             \
    union {                          \
        BinaryHeapInternal internal; \
        type *payload;               \
    }

#define heap_new(cmp) {.internal = __heap_new(cmp)}

#define heap_destroy(heap) __heap_destroy(&(heap)->internal)

#define heap_size(heap) __heap_size(&(heap)->internal)

#define heap_is_empty(heap) __heap_is_empty(&(heap)->internal)

#define heap_push(heap, value) \
    __heap_push(&(heap)->internal, true ? (value) : (heap)->payload, sizeof(*(heap)->payload))

#define heap_top(heap) ((typeof((heap)->payload))__heap_top(&(heap)->internal))

#define heap_pop(heap, out) \
    __heap_pop(&(heap)->internal, true ? (out) : (heap)->payload, sizeof(*(heap)->payload))

typedef bool (*CompareFn)(const void *a, const void *b);

typedef struct {
    VecInternal data;
    CompareFn cmp;
} BinaryHeapInternal;

BinaryHeapInternal __heap_new(CompareFn cmp);

void __heap_destroy(BinaryHeapInternal *heap);

size_t __heap_size(const BinaryHeapInternal *heap);

bool __heap_is_empty(const BinaryHeapInternal *heap);

void __heap_push(BinaryHeapInternal *heap, const void *value, size_t item_size);

void *__heap_top(BinaryHeapInternal *heap);

void __heap_pop(BinaryHeapInternal *heap, void *out, size_t item_size);

void __heap_clear(BinaryHeapInternal *heap);

#endif
