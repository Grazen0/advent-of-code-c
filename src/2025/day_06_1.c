#include "lib/aoc.h"
#include "lib/str.h"
#include "lib/vec.h"
#include "stdio.h"
#include <stddef.h>
#include <stdio.h>

typedef Vec(String) VecString;
typedef Vec(char) VecChar;

int main(void)
{
    VecString lines = vec_new();
    read_into_lines(stdin, &lines);

    VecChar operations = vec_new();

    const String *last = vec_last(&lines);

    FILE *fline = fmemopen(last->data, last->size, "r");

    char op;
    while (fscanf(fline, " %c", &op) != EOF) {
        vec_push(&operations, &op);
    }

    fclose(fline);

    size_t cols = vec_size(&operations);

    long results[cols];

    for (size_t i = 0; i < cols; ++i) {
        if (*vec_get(&operations, i) == '+')
            results[i] = 0;
        else
            results[i] = 1;
    }

    for (size_t i = 0; i < vec_size(&lines) - 1; ++i) {
        const String *const line = vec_get(&lines, i);

        FILE *fline = fmemopen(line->data, line->size, "r");

        for (size_t j = 0; j < cols; ++j) {
            long n;
            fscanf(fline, "%li", &n);

            if (*vec_get(&operations, j) == '+')
                results[j] += n;
            else
                results[j] *= n;
        }

        fclose(fline);
    }

    long sum = 0;

    for (size_t i = 0; i < cols; ++i)
        sum += results[i];

    printf("%li\n", sum);

    vec_destroy(&operations);
    vec_destroy(&lines);

    return 0;
}
