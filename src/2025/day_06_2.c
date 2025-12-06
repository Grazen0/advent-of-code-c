#include "lib/aoc.h"
#include "lib/macros.h"
#include "lib/str.h"
#include "lib/vec.h"
#include "stdio.h"
#include <stddef.h>
#include <stdio.h>

typedef Vec(String) VecString;
typedef Vec(long) VecLong;

int main(void)
{
    VecString lines = vec_new();
    read_into_lines(stdin, &lines);

    long sum = 0;

    size_t const rows = vec_size(&lines);
    size_t cols = 0;

    for (size_t i = 0; i < rows; ++i) {
        const String *line = vec_get(&lines, i);
        cols = MAX(cols, line->size);
    }

    VecLong nums = vec_new();

    for (size_t j = cols - 1; j != (size_t)-1; --j) {
        long n = 0;
        char op = '\0';
        bool col_empty = true;

        for (size_t i = 0; i < rows; ++i) {
            const String *line = vec_get(&lines, i);

            if (j >= line->size)
                continue;

            char ch = line->data[j];

            if (ch >= '0' && ch <= '9') {
                col_empty = false;
                n = (10 * n) + (ch - '0');
            } else if (ch != ' ') {
                // Operation
                op = ch;
                break;
            }
        }

        if (!col_empty)
            vec_push(&nums, &n);

        if (op != '\0') {
            long result = op == '+' ? 0 : 1;

            vec_for(&nums, n)
            {
                if (op == '+')
                    result += *n;
                else
                    result *= *n;
            }

            sum += result;

            vec_clear(&nums);
        }
    }

    printf("%li\n", sum);

    vec_destroy(&lines);

    return 0;
}
