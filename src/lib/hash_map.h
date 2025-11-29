#ifndef AOC_LIB_HASH_MAP_H
#define AOC_LIB_HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct MapNode MapNode;

struct MapNode {
    int key;
    int value;
    size_t hash;
    MapNode *next;
};

typedef struct {
    size_t size;
    size_t used_buckets;
    MapNode **buckets;
    size_t buckets_size;
} HashMap;

HashMap map_new(void);

void map_destroy(HashMap *map);

bool map_contains_key(HashMap *map, int key);

int map_get(HashMap *map, int key);

int map_get_or(HashMap *map, int key, int default_value);

bool map_insert(HashMap *map, int key, int value);

bool map_remove(HashMap *map, int key);

#endif
