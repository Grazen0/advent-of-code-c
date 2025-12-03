#include "lib/macros.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *const a, const void *const b)
{
    const int a_val = *(const int *)a;
    const int b_val = *(const int *)b;
    return a_val - b_val;
}

int main(void)
{
    Vec v1 = vec_new();
    Vec v2 = vec_new();

    int n = -1;

    while (true) {
        BREAK_EOF(scanf("%i", &n));
        vec_push(&v1, n);

        BREAK_EOF(scanf("%i", &n));
        vec_push(&v2, n);
    }

    qsort(v1.data, v1.size, sizeof(*v1.data), cmp);
    qsort(v2.data, v2.size, sizeof(*v2.data), cmp);

    int sum = 0;

    for (size_t i = 0; i < v1.size; ++i)
        sum += abs(v1.data[i] - v2.data[i]);

    printf("%i\n", sum);

    vec_destroy(&v1);
    vec_destroy(&v2);

    return 0;
}
