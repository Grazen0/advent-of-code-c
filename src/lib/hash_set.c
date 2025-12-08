#include "lib/hash_set.h"
#include "lib/macros.h"
#include "lib/numeric.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static constexpr float MAX_FILL_FACTOR = 0.75F;
static constexpr size_t INITIAL_BUCKETS = 8;

struct HashSetNode {
    HashSetNode *next;
    size_t hash;
    u8 value[];
};

typedef HashSetNode Node;

static inline float fill_factor(HashSetInternal *const set)
{
    return (float)set->used_buckets / (float)set->buckets_size;
}

static inline void rehash(HashSetInternal *const set)
{
    const size_t new_buckets_size =
        set->buckets_size == 0 ? INITIAL_BUCKETS : 2 * set->buckets_size;

    Node **const new_buckets = (Node **)calloc(new_buckets_size, sizeof(*new_buckets));

    size_t new_used_buckets = 0;

    for (size_t i = 0; i < set->buckets_size; ++i) {
        Node *cur = set->buckets[i];

        while (cur != nullptr) {
            const size_t new_idx = cur->hash % new_buckets_size;

            Node *const next = cur->next;
            cur->next = new_buckets[new_idx];

            if (cur->next == nullptr)
                ++new_used_buckets;

            new_buckets[new_idx] = cur;

            cur = next;
        }
    }

    free((void *)set->buckets);
    set->buckets = new_buckets;
    set->buckets_size = new_buckets_size;
    set->used_buckets = new_used_buckets;
}

HashSetInternal __hset_new(EqFn eq, HashFn hash)
{
    return (HashSetInternal){
        .size = 0,
        .used_buckets = 0,
        .buckets_size = 0,
        .eq = eq,
        .hash = hash,
        .buckets = nullptr,
    };
}

void __hset_destroy(HashSetInternal *const set)
{
    for (size_t i = 0; i < set->buckets_size; ++i) {
        Node *cur = set->buckets[i];

        while (cur != nullptr) {
            Node *const next = cur->next;
            free(cur);
            cur = next;
        }
    }

    free((void *)set->buckets);
    set->buckets_size = 0;
    set->size = 0;
    set->used_buckets = 0;
}

bool __hset_contains(const HashSetInternal *const set, const void *const key)
{
    const size_t hash = set->hash(key);
    const size_t idx = hash % set->buckets_size;

    Node *cur = set->buckets[idx];

    while (cur != nullptr) {
        if (set->eq(cur->value, key))
            return true;

        cur = cur->next;
    }

    return false;
}

void *__hset_get(HashSetInternal *const set, const void *const key)
{
    PANIC_IF(set->buckets_size == 0, "key not found");

    const size_t hash = set->hash(key);
    const size_t idx = hash % set->buckets_size;

    Node *cur = set->buckets[idx];

    while (cur != nullptr) {
        if (set->eq(cur->value, key))
            return cur->value;

        cur = cur->next;
    }

    PANIC("key not found");
}

void *__hset_get_or(HashSetInternal *const set, const void *value, void *const default_value)
{
    if (set->buckets_size == 0)
        return default_value;

    const size_t hash = set->hash(value);
    const size_t idx = hash % set->buckets_size;

    Node *cur = set->buckets[idx];

    while (cur != nullptr) {
        if (set->eq(cur->value, value))
            return cur->value;

        cur = cur->next;
    }

    return default_value;
}

bool __hset_insert(HashSetInternal *const set, const void *const value, const size_t item_size)
{
    if (set->buckets_size == 0 || fill_factor(set) > MAX_FILL_FACTOR)
        rehash(set);

    const size_t hash = set->hash(value);
    const size_t idx = hash % set->buckets_size;

    Node **cur = &set->buckets[idx];

    while (*cur != nullptr && !set->eq((*cur)->value, value))
        cur = &(*cur)->next;

    if (*cur != nullptr) {
        memcpy((*cur)->value, value, item_size);
        return false;
    }

    if (set->buckets[idx] == nullptr)
        ++set->used_buckets;

    *cur = malloc(sizeof(*set->buckets[0]) + item_size);
    **cur = (Node){
        .next = nullptr,
        .hash = hash,
    };

    memcpy((*cur)->value, value, item_size);

    ++set->size;
    return true;
}

bool __hset_remove(HashSetInternal *const set, const void *const key)
{
    const size_t hash = set->hash(key);
    const size_t idx = hash % set->buckets_size;

    Node **cur = &set->buckets[idx];

    while (*cur != nullptr && !set->eq((*cur)->value, key))
        cur = &(*cur)->next;

    if (*cur == nullptr)
        return false;

    Node *const next = (*cur)->next;
    free(*cur);
    *cur = next;

    if (set->buckets[idx] == nullptr)
        --set->used_buckets;

    --set->size;
    return true;
}
