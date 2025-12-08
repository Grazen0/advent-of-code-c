#ifndef AOC_LIB_HASH_SET_H
#define AOC_LIB_HASH_SET_H

#include <stddef.h>

#define HashSet(type)             \
    union {                       \
        HashSetInternal internal; \
        type *payload;            \
    }

#define hset_new(eq, hash) {.internal = __hset_new(eq, hash)}

#define hset_destroy(set) __hset_destroy(&(set)->internal)

#define hset_contains(set, value) __hset_contains(&(set)->internal, true ? (value) : (set)->payload)

#define hset_get(set, value) \
    ((typeof((set)->payload))__hset_get(&(set)->internal, true ? (value) : (set)->payload))

#define hset_get_or(set, value, default_value)                                                \
    ((typeof((set)->payload))__hset_get_or(&(set)->internal, true ? (value) : (set)->payload, \
                                           true ? (default_value) : (set)->payload))

#define hset_insert(set, value) \
    __hset_insert(&(set)->internal, true ? (value) : (set)->payload, sizeof(*(set)->payload))

#define hset_remove(set, value) __hset_remove(&(set)->internal, true ? (value) : (set)->payload)

typedef struct HashSetNode HashSetNode;

typedef bool (*EqFn)(const void *a, const void *b);

typedef size_t (*HashFn)(const void *data);

typedef struct {
    size_t size;
    size_t used_buckets;
    size_t buckets_size;
    EqFn eq;
    HashFn hash;
    HashSetNode **buckets;
} HashSetInternal;

HashSetInternal __hset_new(EqFn eq, HashFn hash);

void __hset_destroy(HashSetInternal *set);

bool __hset_contains(const HashSetInternal *set, const void *value);

void *__hset_get(HashSetInternal *set, const void *value);

void *__hset_get_or(HashSetInternal *set, const void *value, void *default_value);

bool __hset_insert(HashSetInternal *set, const void *value, size_t item_size);

bool __hset_remove(HashSetInternal *set, const void *value);

#endif
