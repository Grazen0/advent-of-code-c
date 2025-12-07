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

int main(void)
{
    VecString lines = vec_new();
    read_into_lines(stdin, &lines);

    size_t rows = vec_size(&lines);
    size_t cols = vec_first(&lines)->size;

    bool visited[rows][cols] = {};

    Position initial_pos = {};

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (vec_get(&lines, i)->data[j] == 'S') {
                initial_pos = (Position){.i = i, .j = j};
                break;
            }
        }
    }

    QueuePosition queue = queue_new();
    queue_enqueue(&queue, &initial_pos);

    long sum = 0;

    while (!queue_is_empty(&queue)) {
        Position pos;
        queue_dequeue(&queue, &pos);

        if (pos.i >= rows || pos.j == (size_t)-1 || pos.j >= cols || visited[pos.i][pos.j])
            continue;

        visited[pos.i][pos.j] = true;

        if (vec_get(&lines, pos.i)->data[pos.j] == '^') {
            const Position left = {.i = pos.i + 1, .j = pos.j - 1};
            const Position right = {.i = pos.i + 1, .j = pos.j + 1};

            queue_enqueue(&queue, &left);
            queue_enqueue(&queue, &right);

            ++sum;
        } else {
            const Position down = {.i = pos.i + 1, .j = pos.j};
            queue_enqueue(&queue, &down);
        }
    }

    printf("%li\n", sum);

    return 0;
}
