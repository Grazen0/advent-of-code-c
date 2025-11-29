#ifndef AOC_LIB_HASH_SET_H
#define AOC_LIB_HASH_SET_H

#include <stdbool.h>
#include <stddef.h>

typedef struct HashSetNode HashSetNode;

typedef struct {
    size_t size;
    size_t used_buckets;
    HashSetNode **buckets;
    size_t buckets_size;
} HashSet;

HashSet hset_new(void);

void hset_destroy(HashSet *set);

bool hset_contains(const HashSet *set, int value);

bool hset_insert(HashSet *set, int value);

bool hset_remove(HashSet *set, int value);

#endif
