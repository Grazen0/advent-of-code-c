#include "lib/macros.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// naive ass solution
int main(void)
{
    long sum = 0;

    FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size)
    {
        int n[len];

        for (ssize_t i = 0; i < len; ++i)
            n[i] = line[i] - '0';

        int largest = 0;

        for (ssize_t i = 0; i < len; ++i) {
            for (ssize_t j = i + 1; j < len; ++j) {
                int joltage = (10 * n[i]) + n[j];
                largest = MAX(largest, joltage);
            }
        }

        sum += largest;
    }
    FOR_EACH_LINE_END(line);

    printf("%li\n", sum);

    return 0;
}
