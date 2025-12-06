#include "lib/hash_map.h"
#include "lib/macros.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>

typedef Vec(int) VecInt;
typedef HashMap(int, int) HashMapIntInt;

bool int_eq(const void *const a, const void *const b)
{
    return *(int *)a == *(int *)b;
}

size_t int_hash(const void *const value)
{
    return *(int *)value;
}

static int ZERO = 0;

int main(void)
{
    VecInt v1 = vec_new();
    HashMapIntInt freq_map = hmap_new(int_eq, int_hash);

    int n = -1;

    while (true) {
        BREAK_EOF(scanf("%i", &n));
        vec_push(&v1, &n);

        BREAK_EOF(scanf("%i", &n));

        int cur_val = *hmap_get_or(&freq_map, &n, &ZERO);
        ++cur_val;

        hmap_insert(&freq_map, &n, &cur_val);
    }

    int sum = 0;

    vec_for(&v1, x)
    {
        sum += *x * *hmap_get_or(&freq_map, x, &ZERO);
    }

    printf("%i\n", sum);

    vec_destroy(&v1);
    hmap_destroy(&freq_map);

    return 0;
}
