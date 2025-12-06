#ifndef AOC_LIB_HASH_MAP_H
#define AOC_LIB_HASH_MAP_H

#include <stddef.h>

#define HashMap(key_type, value_type) \
    union {                           \
        HashMapInternal internal;     \
        key_type *key_payload;        \
        value_type *value_payload;    \
    }

#define hmap_new(eq, hash) {.internal = __hmap_new(eq, hash)}

#define hmap_destroy(map) __hmap_destroy(&(map)->internal)

#define hmap_contains_key(map, key) \
    __hmap_contains_key(&(map)->internal, true ? (key) : (map)->payload)

#define hmap_get(map, key)                                                                     \
    ((typeof((map)->value_payload))__hmap_get(&(map)->internal, true ? (key) : (map)->payload, \
                                              sizeof(*(map)->key_payload)))

#define hmap_get_or(map, key, default_value)                 \
    ((typeof((map)->value_payload))__hmap_get_or(            \
        &(map)->internal, true ? (key) : (map)->key_payload, \
        true ? (default_value) : (map)->value_payload, sizeof(*(map)->key_payload)))

#define hmap_insert(map, key, value)                                                  \
    __hmap_insert(&(map)->internal, true ? (key) : (map)->key_payload,                \
                  true ? (value) : (map)->value_payload, sizeof(*(map)->key_payload), \
                  sizeof(*(map)->value_payload))

#define hmap_remove(map, key) __hmap_remove(&(map)->internal, true ? (key) : (map)->key_payload)

typedef struct HashMapNode HashMapNode;

typedef bool (*EqFn)(const void *a, const void *b);

typedef size_t (*HashFn)(const void *data);

typedef struct {
    size_t size;
    size_t used_buckets;
    size_t buckets_size;
    EqFn eq;
    HashFn hash;
    HashMapNode **buckets;
} HashMapInternal;

HashMapInternal __hmap_new(EqFn eq, HashFn hash);

void __hmap_destroy(HashMapInternal *map);

bool __hmap_contains_key(const HashMapInternal *map, const void *key);

void *__hmap_get(HashMapInternal *map, const void *key, size_t key_size);

void *__hmap_get_or(HashMapInternal *map, const void *key, void *default_value, size_t key_size);

bool __hmap_insert(HashMapInternal *map, const void *key, const void *value, size_t key_size,
                   size_t value_size);

bool __hmap_remove(HashMapInternal *map, const void *key);

#endif
