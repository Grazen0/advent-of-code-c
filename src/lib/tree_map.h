#ifndef AOC_LIB_TREE_MAP_H
#define AOC_LIB_TREE_MAP_H

#include <stddef.h>

typedef struct TreeMapNode TreeMapNode;

typedef struct {
    size_t size;
    TreeMapNode *root;
} TreeMap;

TreeMap tmap_new(void);

void tmap_destroy(TreeMap *map);

bool tmap_contains_key(const TreeMap *map, int key);

int tmap_get(TreeMap *map, int key);

int tmap_get_or(TreeMap *map, int key, int default_value);

bool tmap_insert(TreeMap *map, int key, int value);

bool tmap_remove(TreeMap *map, int key);

#endif
