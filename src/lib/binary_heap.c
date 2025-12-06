#include "lib/binary_heap.h"
#include "lib/macros.h"
#include "lib/numeric.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static inline void memswap(void *const a, void *const b, const size_t n)
{
    u8 tmp[n];

    memcpy(tmp, a, n);
    memcpy(a, b, n);
    memcpy(b, tmp, n);
}

static inline size_t parent(const size_t idx)
{
    return (idx - 1) / 2;
}

static inline size_t left(const size_t idx)
{
    return (2 * idx) + 1;
}

static inline size_t right(const size_t idx)
{
    return (2 * idx) + 2;
}

static void bubble_up(BinaryHeapInternal *const heap, const size_t idx, const size_t item_size)
{
    size_t cur_idx = idx;

    while (cur_idx != 0) {
        const size_t par_idx = parent(cur_idx);

        void *const cur = __vec_get(&heap->data, cur_idx, item_size);
        void *const par = __vec_get(&heap->data, par_idx, item_size);

        if (!heap->cmp(par, cur)) // par >= cur
            break;

        memswap(cur, par, item_size);
        cur_idx = par_idx;
    }
}

static void bubble_down(BinaryHeapInternal *const heap, const size_t idx, const size_t item_size)
{
    size_t cur_idx = idx;
    const size_t size = __vec_size(&heap->data);

    while (cur_idx < size) {
        void *const cur = __vec_get(&heap->data, cur_idx, item_size);

        size_t target_idx = cur_idx;
        void *target = cur;

        const size_t le_idx = left(cur_idx);
        const size_t ri_idx = right(cur_idx);

        void *const le = __vec_get_unchecked(&heap->data, le_idx, item_size);
        void *const ri = __vec_get_unchecked(&heap->data, ri_idx, item_size);

        if (le_idx < size && heap->cmp(target, le)) { // target < le
            target_idx = le_idx;
            target = le;
        }

        if (ri_idx < size && heap->cmp(target, ri)) { // target < ri
            target_idx = ri_idx;
            target = ri;
        }

        if (cur_idx == target_idx)
            break;

        memswap(cur, target, item_size);
        cur_idx = target_idx;
    }
}

BinaryHeapInternal __heap_new(const CompareFn cmp)
{
    return (BinaryHeapInternal){
        .data = __vec_new(),
        .cmp = cmp,
    };
}

void __heap_destroy(BinaryHeapInternal *const heap)
{
    __vec_destroy(&heap->data);
}

size_t __heap_size(const BinaryHeapInternal *const heap)
{
    return __vec_size(&heap->data);
}

bool __heap_is_empty(const BinaryHeapInternal *const heap)
{
    return __vec_is_empty(&heap->data);
}

void __heap_push(BinaryHeapInternal *const heap, const void *value, const size_t item_size)
{
    __vec_push(&heap->data, value, item_size);
    bubble_up(heap, __vec_size(&heap->data) - 1, item_size);
}

void *__heap_top(BinaryHeapInternal *const heap)
{
    PANIC_IF(heap->data.size == 0, "heap is empty");
    return __vec_first(&heap->data);
}

void __heap_pop(BinaryHeapInternal *const heap, void *const out, const size_t item_size)
{
    PANIC_IF(heap->data.size == 0, "heap is empty");

    if (out != nullptr)
        memcpy(out, __vec_first(&heap->data), item_size);

    memcpy(__vec_first(&heap->data), __vec_last(&heap->data, item_size), item_size);
    __vec_pop(&heap->data, item_size);
    bubble_down(heap, 0, item_size);
}

void __heap_clear(BinaryHeapInternal *const heap)
{
    __vec_clear(&heap->data);
}
