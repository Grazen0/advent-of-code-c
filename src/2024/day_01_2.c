#include "lib/hash_map.h"
#include "lib/macros.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    Vec v1 = vec_new();
    HashMap freq_map = hmap_new();

    int n = -1;

    while (true) {
        BREAK_EOF(scanf("%i", &n));
        vec_push(&v1, n);

        BREAK_EOF(scanf("%i", &n));
        hmap_insert(&freq_map, n, hmap_get_or(&freq_map, n, 0) + 1);
    }

    int sum = 0;

    for (size_t i = 0; i < v1.size; ++i)
        sum += v1.data[i] * hmap_get_or(&freq_map, v1.data[i], 0);

    printf("%i\n", sum);

    vec_destroy(&v1);
    hmap_destroy(&freq_map);

    return 0;
}
