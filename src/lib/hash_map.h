#ifndef AOC_LIB_HASH_MAP_H
#define AOC_LIB_HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct HashMapNode HashMapNode;

struct HashMapNode {
    int key;
    int value;
    size_t hash;
    HashMapNode *next;
};

typedef struct {
    size_t size;
    size_t used_buckets;
    HashMapNode **buckets;
    size_t buckets_size;
} HashMap;

HashMap hmap_new(void);

void hmap_destroy(HashMap *map);

bool hmap_contains_key(const HashMap *map, int key);

int hmap_get(HashMap *map, int key);

int hmap_get_or(HashMap *map, int key, int default_value);

bool hmap_insert(HashMap *map, int key, int value);

bool hmap_remove(HashMap *map, int key);

#endif
