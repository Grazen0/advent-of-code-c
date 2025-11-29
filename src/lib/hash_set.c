#include "lib/hash_set.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const float MAX_FILL_FACTOR = 0.75F;
static const size_t INITIAL_BUCKETS = 8;

struct HashSetNode {
    int value;
    size_t hash;
    HashSetNode *next;
};

typedef HashSetNode Node;

static inline size_t get_hash_code(const int value)
{
    return value;
}

static inline float fill_factor(HashSet *const set)
{
    return (float)set->used_buckets / (float)set->buckets_size;
}

static inline void rehash(HashSet *const set)
{
    const size_t new_buckets_size =
        set->buckets_size == 0 ? INITIAL_BUCKETS : 2 * set->buckets_size;

    Node **const new_buckets =
        (Node **)calloc(new_buckets_size, sizeof(*new_buckets));

    size_t new_used_buckets = 0;

    for (size_t i = 0; i < set->buckets_size; ++i) {
        Node *cur = set->buckets[i];

        while (cur != NULL) {
            const size_t new_idx = cur->hash % new_buckets_size;

            Node *const next = cur->next;
            cur->next = new_buckets[new_idx];

            if (cur->next == NULL)
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

HashSet hset_new(void)
{
    return (HashSet){
        .size = 0,
        .used_buckets = 0,
        .buckets = NULL,
        .buckets_size = 0,
    };
}

void hset_destroy(HashSet *const set)
{
    for (size_t i = 0; i < set->buckets_size; ++i) {
        Node *cur = set->buckets[i];

        while (cur != NULL) {
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

bool hset_contains(const HashSet *const set, const int value)
{
    const size_t hash = get_hash_code(value);
    const size_t idx = hash % set->buckets_size;

    Node *cur = set->buckets[idx];

    while (cur != NULL) {
        if (cur->value == value)
            return true;

        cur = cur->next;
    }

    return false;
}

bool hset_insert(HashSet *const set, const int value)
{
    if (set->buckets_size == 0 || fill_factor(set) > MAX_FILL_FACTOR)
        rehash(set);

    const size_t hash = get_hash_code(value);
    const size_t idx = hash % set->buckets_size;

    Node **cur = &set->buckets[idx];

    while (*cur != NULL && (*cur)->value != value)
        cur = &(*cur)->next;

    if (*cur != NULL)
        return false;

    if (set->buckets[idx] == NULL)
        ++set->used_buckets;

    const Node new_node = {
        .value = value,
        .hash = hash,
        .next = NULL,
    };

    *cur = malloc(sizeof(*set->buckets[0]));
    memcpy(*cur, &new_node, sizeof(new_node));

    ++set->size;
    return true;
}

bool hset_remove(HashSet *const set, const int value)
{
    const size_t hash = get_hash_code(value);
    const size_t idx = hash % set->buckets_size;

    Node **cur = &set->buckets[idx];

    while (*cur != NULL && (*cur)->value != value)
        cur = &(*cur)->next;

    if (*cur == NULL)
        return false;

    Node *const next = (*cur)->next;
    free(*cur);
    *cur = next;

    if (set->buckets[idx] == NULL)
        --set->used_buckets;

    --set->size;
    return true;
}
