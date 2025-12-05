#include "lib/aoc.h"
#include "lib/str.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef Vec(String) VecString;

int main(void)
{
    VecString lines = vec_new();
    read_into_lines(stdin, &lines);

    const ssize_t rows = vec_size(&lines);
    const ssize_t cols = vec_first(&lines)->size;

    bool paper[rows][cols] = {};
    int counts[rows][cols] = {};

    for (ssize_t i = 0; i < rows; ++i) {
        String *const line = vec_get(&lines, i);

        for (ssize_t j = 0; j < cols; ++j) {
            if (line->data[j] == '@') {
                paper[i][j] = true;

                for (ssize_t di = i - 1; di <= i + 1; ++di) {
                    if (di < 0 || di >= rows)
                        continue;

                    for (ssize_t dj = j - 1; dj <= j + 1; ++dj) {
                        if ((di == i && dj == j) || dj < 0 || dj >= cols)
                            continue;

                        ++counts[di][dj];
                    }
                }
            }
        }
    }

    long sum = 0;
    long prev_sum = 0;

    do {
        prev_sum = sum;

        int new_counts[rows][cols];
        memcpy(new_counts, counts, rows * cols * sizeof(**counts));

        for (ssize_t i = 0; i < rows; ++i) {
            for (ssize_t j = 0; j < cols; ++j) {
                if (paper[i][j] && counts[i][j] < 4) {
                    paper[i][j] = false;
                    ++sum;

                    for (ssize_t di = i - 1; di <= i + 1; ++di) {
                        if (di < 0 || di >= rows)
                            continue;

                        for (ssize_t dj = j - 1; dj <= j + 1; ++dj) {
                            if ((di == i && dj == j) || dj < 0 || dj >= cols)
                                continue;

                            --new_counts[di][dj];
                        }
                    }
                }
            }
        }

        memcpy(counts, new_counts, rows * cols * sizeof(**counts));
    } while (sum != prev_sum);

    printf("%li\n", sum);

    vec_destroy(&lines);

    return 0;
}
