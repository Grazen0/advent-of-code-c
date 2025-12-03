#include "lib/macros.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>

static constexpr size_t N = 12;

long thing_cached(int n[], size_t n_size, size_t start, size_t d, long cache[][N]);

long thing(int n[], size_t n_size, size_t start, size_t d, long cache[][N])
{

    if (d == 0)
        return 0;

    if (start + d > n_size)
        return -1;

    long largest = -1;

    for (size_t i = start + 1; i < n_size; ++i) {
        long r = thing_cached(n, n_size, i, d - 1, cache);
        if (r != -1 || largest == -1)
            largest = MAX(largest, r);
    }

    if (largest == -1)
        return -1;

    return (n[start] * powl(10, d - 1)) + largest;
}

long thing_cached(int n[], size_t n_size, size_t start, size_t d, long cache[][N])
{
    if (cache[start][d] != -1)
        return cache[start][d];

    long result = thing(n, n_size, start, d, cache);
    cache[start][d] = result;
    return result;
}

int main(void)
{
    long sum = 0;

    FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size)
    {
        int n[len];

        for (ssize_t i = 0; i < len; ++i)
            n[i] = line[i] - '0';

        long largest = 0;

        long cache[len][N];

        for (ssize_t i = 0; i < len; ++i) {
            for (size_t j = 0; j < N; ++j)
                cache[i][j] = -1;
        }

        for (ssize_t i = 0; i < len; ++i) {
            long r = thing(n, len, i, N, cache);
            largest = MAX(largest, r);
        }
        sum += largest;
    }
    FOR_EACH_LINE_END(line);

    printf("%li\n", sum);

    return 0;
}
