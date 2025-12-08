#include "lib/hash_map.h"
#include "lib/macros.h"
#include "lib/vec.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x;
    float y;
    float z;
} Coord;

typedef struct {
    size_t from;
    size_t to;
    float dist;
} Edge;

typedef struct {
    size_t parent;
    size_t rank;
} Node;

typedef Vec(Coord) VecCoord;
typedef Vec(Edge) VecEdge;
typedef HashMap(size_t, size_t) HashMapSizeSize;

static float distance(const Coord *from, const Coord *to)
{
    float dx = to->x - from->x;
    float dy = to->y - from->y;
    float dz = to->z - from->z;

    return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}

static int cmp(const void *a, const void *b)
{
    const Edge *e1 = a;
    const Edge *e2 = b;
    return e1->dist - e2->dist;
}

static int cmp_size(const void *a, const void *b)
{
    return *(const size_t *)b - *(const size_t *)a;
}

void djset_make_set(Node nodes[], const size_t x)
{
    nodes[x] = (Node){
        .parent = x,
        .rank = 0,
    };
}

size_t djset_find(Node nodes[], const size_t x)
{
    if (nodes[x].parent == x)
        return x;

    size_t root = djset_find(nodes, nodes[x].parent);
    nodes[x].parent = root;

    return root;
}

void djset_union(Node nodes[], const size_t x, const size_t y)
{
    const size_t x_root = djset_find(nodes, x);
    const size_t y_root = djset_find(nodes, y);

    if (x_root == y_root)
        return;

    if (nodes[x_root].rank < nodes[y_root].rank) {
        nodes[x_root].parent = y_root;
    } else if (nodes[y_root].rank < nodes[x_root].rank) {
        nodes[y_root].parent = x_root;
    } else {
        nodes[y_root].parent = x_root;
        ++nodes[x_root].rank;
    }
}

int main(void)
{
    VecCoord coords = vec_new();

    FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size)
    {
        FILE *fline = fmemopen(line, len, "r");

        Coord coord;
        fscanf(fline, "%f,%f,%f", &coord.x, &coord.y, &coord.z);

        vec_push(&coords, &coord);

        fclose(fline);
    }
    FOR_EACH_LINE_END(line);

    size_t n = vec_size(&coords);

    VecEdge edges = vec_new();

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            Edge edge = {
                .from = i,
                .to = j,
                .dist = distance(vec_get(&coords, i), vec_get(&coords, j)),
            };

            vec_push(&edges, &edge);
        }
    }

    vec_qsort(&edges, cmp);

    Node nodes[n];

    for (size_t i = 0; i < n; ++i)
        djset_make_set(nodes, i);

    size_t last_from = 0;
    size_t last_to = 0;

    vec_for(&edges, edge)
    {
        if (djset_find(nodes, edge->from) != djset_find(nodes, edge->to)) {
            djset_union(nodes, edge->from, edge->to);

            last_from = edge->from;
            last_to = edge->to;
        }
    }

    float result = vec_get(&coords, last_from)->x * vec_get(&coords, last_to)->x;
    printf("%li\n", (long)result);

    vec_destroy(&edges);
    vec_destroy(&coords);

    return 0;
}
