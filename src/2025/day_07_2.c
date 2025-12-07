#include "lib/aoc.h"
#include "lib/queue.h"
#include "lib/str.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t i;
    size_t j;
} Position;

typedef Vec(String) VecString;
typedef Queue(Position) QueuePosition;

typedef struct {
    size_t rows;
    size_t cols;
    VecString lines;
    long *cache;
} Context;

long solution(Context *ctx, size_t i, size_t j, long cache[ctx->rows][ctx->cols])
{

    if (i >= ctx->rows)
        return 1;

    if (j == (size_t)-1 || j >= ctx->cols)
        return 0;

    if (cache[i][j] != -1)
        return cache[i][j];

    long result;

    if (vec_get(&ctx->lines, i)->data[j] != '^')
        result = solution(ctx, i + 1, j, cache);
    else
        result = solution(ctx, i + 1, j - 1, cache) + solution(ctx, i + 1, j + 1, cache);

    cache[i][j] = result;
    return result;
}

int main(void)
{
    VecString lines = vec_new();
    read_into_lines(stdin, &lines);

    size_t rows = vec_size(&lines);
    size_t cols = vec_first(&lines)->size;

    Position initial_pos = {};

    long cache[rows][cols];

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            cache[i][j] = -1;

            if (vec_get(&lines, i)->data[j] == 'S')
                initial_pos = (Position){.i = i, .j = j};
        }
    }

    Context ctx = {
        .lines = lines,
        .rows = rows,
        .cols = cols,
    };

    printf("%li\n", solution(&ctx, initial_pos.i, initial_pos.j, cache));

    return 0;
}
