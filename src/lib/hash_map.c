#include "lib/hash_map.h"
#include "lib/macros.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const float MAX_FILL_FACTOR = 0.75F;
static const size_t INITIAL_BUCKETS = 8;

static inline size_t get_hash_code(const int key)
{
    return key;
}

static inline float fill_factor(HashMap *const map)
{
    return (float)map->used_buckets / (float)map->buckets_size;
}

static inline void rehash(HashMap *const map)
{
    const size_t new_buckets_size =
        map->buckets_size == 0 ? INITIAL_BUCKETS : 2 * map->buckets_size;

    HashMapNode **const new_buckets =
        (HashMapNode **)calloc(new_buckets_size, sizeof(*new_buckets));

    size_t new_used_buckets = 0;

    for (size_t i = 0; i < map->buckets_size; ++i) {
        HashMapNode *cur = map->buckets[i];

        while (cur != NULL) {
            const size_t new_idx = cur->hash % new_buckets_size;

            HashMapNode *const next = cur->next;
            cur->next = new_buckets[new_idx];

            if (cur->next == NULL)
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

HashMap hmap_new(void)
{
    return (HashMap){
        .size = 0,
        .used_buckets = 0,
        .buckets = NULL,
        .buckets_size = 0,
    };
}

void hmap_destroy(HashMap *const map)
{
    for (size_t i = 0; i < map->buckets_size; ++i) {
        HashMapNode *cur = map->buckets[i];

        while (cur != NULL) {
            HashMapNode *const next = cur->next;
            free(cur);
            cur = next;
        }
    }

    free((void *)map->buckets);
    map->buckets_size = 0;
    map->size = 0;
    map->used_buckets = 0;
}

bool hmap_contains_key(const HashMap *const map, const int key)
{
    const size_t hash = get_hash_code(key);
    const size_t idx = hash % map->buckets_size;

    HashMapNode *cur = map->buckets[idx];

    while (cur != NULL) {
        if (cur->key == key)
            return true;

        cur = cur->next;
    }

    return false;
}

int hmap_get(HashMap *const map, const int key)
{
    if (map->buckets_size == 0)
        PANIC("key %i not found", key);

    const size_t hash = get_hash_code(key);
    const size_t idx = hash % map->buckets_size;

    HashMapNode *cur = map->buckets[idx];

    while (cur != NULL) {
        if (cur->key == key)
            return cur->value;

        cur = cur->next;
    }

    PANIC("key %i not found", key);
}

int hmap_get_or(HashMap *const map, const int key, const int default_value)
{
    if (map->buckets_size == 0)
        return default_value;

    const size_t hash = get_hash_code(key);
    const size_t idx = hash % map->buckets_size;

    HashMapNode *cur = map->buckets[idx];

    while (cur != NULL) {
        if (cur->key == key)
            return cur->value;

        cur = cur->next;
    }

    return default_value;
}

bool hmap_insert(HashMap *const map, const int key, const int value)
{
    if (map->buckets_size == 0 || fill_factor(map) > MAX_FILL_FACTOR)
        rehash(map);

    const size_t hash = get_hash_code(key);
    const size_t idx = hash % map->buckets_size;

    HashMapNode **cur = &map->buckets[idx];

    while (*cur != NULL && (*cur)->key != key)
        cur = &(*cur)->next;

    if (*cur != NULL) {
        (*cur)->value = value;
        return false;
    }

    if (map->buckets[idx] == NULL)
        ++map->used_buckets;

    const HashMapNode new_node = {
        .key = key,
        .value = value,
        .hash = hash,
        .next = NULL,
    };

    *cur = malloc(sizeof(*map->buckets[0]));
    memcpy(*cur, &new_node, sizeof(new_node));

    ++map->size;
    return true;
}

bool hmap_remove(HashMap *const map, const int key)
{
    const size_t hash = get_hash_code(key);
    const size_t idx = hash % map->buckets_size;

    HashMapNode **cur = &map->buckets[idx];

    while (*cur != NULL && (*cur)->key != key)
        cur = &(*cur)->next;

    if (*cur == NULL)
        return false;

    HashMapNode *const next = (*cur)->next;
    free(*cur);
    *cur = next;

    if (map->buckets[idx] == NULL)
        --map->used_buckets;

    --map->size;
    return true;
}
