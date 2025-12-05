#include "lib/macros.h"
#include "lib/str.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>

typedef Vec(String) VecString;

int main(void)
{
    VecString lines = vec_new();

    FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size)
    {
        String s = str_from(line);
        if (s.data[s.size - 1] == '\n')
            str_pop(&s);

        vec_push(&lines, &s);
    }
    FOR_EACH_LINE_END(line);

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

    for (ssize_t i = 0; i < rows; ++i) {
        for (ssize_t j = 0; j < cols; ++j) {
            if (paper[i][j] && counts[i][j] < 4)
                ++sum;
        }
    }

    printf("%li\n", sum);

    vec_destroy(&lines);

    return 0;
}
