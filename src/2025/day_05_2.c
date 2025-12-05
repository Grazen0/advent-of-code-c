#include "lib/macros.h"
#include "lib/vec.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t min;
    size_t max;
} Range;

typedef Vec(Range) VecRange;

int main(void)
{
    VecRange ranges = vec_new();

    FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size)
    {
        FILE *const fline = fmemopen(line, len, "r");

        Range r1;
        BREAK_EOF(sscanf(line, "%zu-%zu", &r1.min, &r1.max))

        VecRange new_ranges = vec_new();

        for (size_t i = 0; i < vec_size(&ranges); ++i) {
            Range r2 = *vec_get(&ranges, i);

            if (r1.max >= r2.min && r1.min <= r2.max) {
                r1.min = MIN(r1.min, r2.min);
                r1.max = MAX(r1.max, r2.max);
            } else {
                vec_push(&new_ranges, &r2);
            }
        }

        vec_push(&new_ranges, &r1);

        vec_destroy(&ranges);
        ranges = new_ranges;

        fclose(fline);
    }
    FOR_EACH_LINE_END(line);

    size_t sum = 0;

    for (size_t i = 0; i < vec_size(&ranges); ++i) {
        const Range *range = vec_get(&ranges, i);
        sum += range->max - range->min + 1;
    }

    printf("%zu\n", sum);

    return 0;
}
