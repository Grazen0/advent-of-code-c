#include "lib/hash_map.h"
#include "lib/macros.h"
#include "lib/numeric.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static constexpr float MAX_FILL_FACTOR = 0.75F;
static constexpr size_t INITIAL_BUCKETS = 8;

struct HashMapNode {
    HashMapNode *next;
    size_t hash;
    u8 key_value[];
};

typedef HashMapNode Node;

static inline void *node_key(HashMapNode *const node)
{
    return node->key_value;
}

static inline void *node_value(HashMapNode *const node, const size_t key_size)
{
    return &node->key_value[key_size];
}

static inline float fill_factor(HashMapInternal *const map)
{
    return (float)map->used_buckets / (float)map->buckets_size;
}

static inline void rehash(HashMapInternal *const map)
{
    const size_t new_buckets_size =
        map->buckets_size == 0 ? INITIAL_BUCKETS : 2 * map->buckets_size;

    Node **const new_buckets = (Node **)calloc(new_buckets_size, sizeof(*new_buckets));

    size_t new_used_buckets = 0;

    for (size_t i = 0; i < map->buckets_size; ++i) {
        Node *cur = map->buckets[i];

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

    free((void *)map->buckets);
    map->buckets = new_buckets;
    map->buckets_size = new_buckets_size;
    map->used_buckets = new_used_buckets;
}

HashMapInternal __hmap_new(EqFn eq, HashFn hash)
{
    return (HashMapInternal){
        .size = 0,
        .used_buckets = 0,
        .buckets_size = 0,
        .eq = eq,
        .hash = hash,
        .buckets = nullptr,
    };
}

void __hmap_destroy(HashMapInternal *const map)
{
    for (size_t i = 0; i < map->buckets_size; ++i) {
        Node *cur = map->buckets[i];

        while (cur != nullptr) {
            Node *const next = cur->next;
            free(cur);
            cur = next;
        }
    }

    free((void *)map->buckets);
    map->buckets_size = 0;
    map->size = 0;
    map->used_buckets = 0;
}

bool __hmap_contains_key(const HashMapInternal *const map, const void *const key)
{
    const size_t hash = map->hash(key);
    const size_t idx = hash % map->buckets_size;

    Node *cur = map->buckets[idx];

    while (cur != nullptr) {
        if (map->eq(node_key(cur), key))
            return true;

        cur = cur->next;
    }

    return false;
}

void *__hmap_get(HashMapInternal *const map, const void *const key, const size_t key_size)
{
    PANIC_IF(map->buckets_size == 0, "key not found");

    const size_t hash = map->hash(key);
    const size_t idx = hash % map->buckets_size;

    Node *cur = map->buckets[idx];

    while (cur != nullptr) {
        if (map->eq(node_key(cur), key))
            return node_value(cur, key_size);

        cur = cur->next;
    }

    PANIC("key not found");
}

void *__hmap_get_or(HashMapInternal *const map, const void *key, void *const default_value,
                    const size_t key_size)
{
    if (map->buckets_size == 0)
        return default_value;

    const size_t hash = map->hash(key);
    const size_t idx = hash % map->buckets_size;

    Node *cur = map->buckets[idx];

    while (cur != nullptr) {
        if (map->eq(node_key(cur), key))
            return node_value(cur, key_size);

        cur = cur->next;
    }

    return default_value;
}

bool __hmap_insert(HashMapInternal *const map, const void *const key, const void *const value,
                   const size_t key_size, const size_t value_size)
{
    if (map->buckets_size == 0 || fill_factor(map) > MAX_FILL_FACTOR)
        rehash(map);

    const size_t hash = map->hash(key);
    const size_t idx = hash % map->buckets_size;

    Node **cur = &map->buckets[idx];

    while (*cur != nullptr && !map->eq(node_key(*cur), key))
        cur = &(*cur)->next;

    if (*cur != nullptr) {
        memcpy(node_value(*cur, key_size), value, value_size);
        return false;
    }

    if (map->buckets[idx] == nullptr)
        ++map->used_buckets;

    *cur = malloc(sizeof(*map->buckets[0]) + key_size + value_size);
    **cur = (Node){
        .next = nullptr,
        .hash = hash,
    };

    memcpy(node_key(*cur), key, key_size);
    memcpy(node_value(*cur, key_size), value, value_size);

    ++map->size;
    return true;
}

bool __hmap_remove(HashMapInternal *const map, const void *const key)
{
    const size_t hash = map->hash(key);
    const size_t idx = hash % map->buckets_size;

    Node **cur = &map->buckets[idx];

    while (*cur != nullptr && !map->eq(node_key(*cur), key))
        cur = &(*cur)->next;

    if (*cur == nullptr)
        return false;

    Node *const next = (*cur)->next;
    free(*cur);
    *cur = next;

    if (map->buckets[idx] == nullptr)
        --map->used_buckets;

    --map->size;
    return true;
}
