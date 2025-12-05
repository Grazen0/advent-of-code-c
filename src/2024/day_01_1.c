#include "lib/macros.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *const a, const void *const b)
{
    const int a_val = *(const int *)a;
    const int b_val = *(const int *)b;
    return a_val - b_val;
}

typedef Vec(int) VecInt;

int main(void)
{
    VecInt v1 = vec_new();
    VecInt v2 = vec_new();

    int n = -1;

    while (true) {
        BREAK_EOF(scanf("%i", &n));
        vec_push(&v1, &n);

        BREAK_EOF(scanf("%i", &n));
        vec_push(&v2, &n);
    }

    qsort(vec_data(&v1), vec_size(&v1), vec_data_size(&v1), cmp);
    qsort(vec_data(&v2), vec_size(&v2), vec_data_size(&v2), cmp);

    int sum = 0;

    for (size_t i = 0; i < vec_size(&v1); ++i)
        sum += abs(*vec_get(&v1, i) - *vec_get(&v2, i));

    printf("%i\n", sum);

    vec_destroy(&v1);
    vec_destroy(&v2);

    return 0;
}
