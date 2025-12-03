#include "tree_map.h"
#include "lib/macros.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    COLOR_RED,
    COLOR_BLACK,
} Color;

struct TreeMapNode {
    int key;
    int value;
    Color color;
    ssize_t height;
    TreeMapNode *parent;
    TreeMapNode *left;
    TreeMapNode *right;
};

typedef TreeMapNode Node;

static inline void destroy_node(Node *const node)
{
    if (node == nullptr)
        return;

    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
}

static inline Color color(const Node *const node)
{
    if (node == nullptr)
        return COLOR_BLACK;

    return node->color;
}

static inline ssize_t height(const Node *const node)
{
    if (node == nullptr)
        return -1;

    return node->height;
}

static inline Node *sibling(Node *const node)
{
    PANIC_IF(node->parent == nullptr, "node does not have parent");

    Node *const parent = node->parent;
    return node == parent->left ? parent->right : parent->left;
}

static inline Node **holder(TreeMap *const map, Node *const node)
{
    if (node->parent == nullptr)
        return &map->root;

    if (node == node->parent->left)
        return &node->parent->left;

    return &node->parent->right;
}

static inline void update_height(Node *const node)
{
    const ssize_t l = height(node->left);
    const ssize_t r = height(node->right);
    node->height = 1 + (l > r ? l : r);
}

static inline void update_heights_upward(Node *const node)
{
    Node *cur = node;

    while (cur != nullptr) {
        update_height(cur);
        cur = cur->parent;
    }
}

static void rotate_left(Node **const node)
{
    Node *const x = *node;
    Node *const z = x->right;

    *node = z;
    z->parent = x->parent;

    x->right = z->left;
    if (x->right != nullptr)
        x->right->parent = x;

    z->left = x;
    z->left->parent = z;

    update_heights_upward(x);
}

static void rotate_right(Node **const node)
{
    Node *const x = *node;
    Node *const z = x->left;

    *node = z;
    z->parent = x->parent;

    x->left = z->right;
    if (x->left != nullptr)
        x->left->parent = x;

    z->right = x;
    z->right->parent = z;

    update_heights_upward(x);
}

static inline void rebalance_from(TreeMap *const map, Node *const node)
{
    Node *cur = node;
    Node *parent = cur->parent;

    while (cur->parent != nullptr) { // Case 3
        if (color(parent) == COLOR_BLACK) {
            // Case 1
            break;
        }

        Node *const grandpa = parent->parent;

        if (grandpa == nullptr) {
            // Case 4
            parent->color = COLOR_BLACK;
            break;
        }

        Node *const uncle = sibling(parent);

        if (color(uncle) == COLOR_BLACK) {
            if (cur == parent->right && parent == grandpa->left) {
                // Case 5 (left)
                rotate_left(&grandpa->left);
                cur = parent;
                parent = grandpa->left;
            } else if (cur == parent->left && parent == grandpa->right) {
                // Case 5 (right)
                rotate_right(&grandpa->right);
                cur = parent;
                parent = grandpa->right;
            }

            if (parent == grandpa->left) {
                // Case 6 (left)
                rotate_right(holder(map, grandpa));
                parent->color = COLOR_BLACK;
                grandpa->color = COLOR_RED;
            } else {
                // Case 6 (right)
                rotate_left(holder(map, grandpa));
                parent->color = COLOR_BLACK;
                grandpa->color = COLOR_RED;
            }

            break;
        }

        // Case 2
        parent->color = COLOR_BLACK;
        uncle->color = COLOR_BLACK;
        grandpa->color = COLOR_RED;
        cur = grandpa;
    }
}

TreeMap tmap_new(void)
{
    return (TreeMap){
        .size = 0,
        .root = nullptr,
    };
}

void tmap_destroy(TreeMap *const map)
{
    destroy_node(map->root);
    map->root = nullptr;
    map->size = 0;
}

bool tmap_contains_key(const TreeMap *const map, const int key)
{
    Node *cur = map->root;

    while (cur != nullptr) {
        if (cur->key == key)
            return true;

        if (key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }

    return false;
}

int tmap_get(TreeMap *const map, const int key)
{
    Node *cur = map->root;

    while (cur != nullptr) {
        if (cur->key == key)
            return cur->value;

        if (key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }

    PANIC("key %i not found", key);
}

int tmap_get_or(TreeMap *const map, const int key, const int default_value)
{
    Node *cur = map->root;

    while (cur != nullptr) {
        if (cur->key == key)
            return cur->value;

        if (key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }

    return default_value;
}

bool tmap_insert(TreeMap *const map, const int key, const int value)
{
    Node *parent = nullptr;
    Node **cur = &map->root;

    while (*cur != nullptr) {
        if ((*cur)->key == key) {
            (*cur)->value = value;
            return false;
        }

        parent = *cur;

        if (key < (*cur)->key)
            cur = &(*cur)->left;
        else
            cur = &(*cur)->right;
    }

    const Node new_node = {
        .key = key,
        .value = value,
        .color = COLOR_RED,
        .height = 0,
        .parent = parent,
        .left = nullptr,
        .right = nullptr,
    };

    *cur = malloc(sizeof(**cur));
    memcpy(*cur, &new_node, sizeof(new_node));
    ++map->size;

    rebalance_from(map, *cur);
    update_heights_upward(parent);
    return true;
}

bool tmap_remove(TreeMap *const map, const int key)
{
    // TODO: implement
}
