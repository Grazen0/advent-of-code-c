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
    bool first_half = true;

    VecRange ranges = vec_new();

    size_t sum = 0;

    FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size)
    {
        FILE *const fline = fmemopen(line, len, "r");

        if (first_half) {
            Range range;

            if (sscanf(line, "%zu-%zu", &range.min, &range.max) == EOF)
                first_half = false;

            vec_push(&ranges, &range);
        } else {
            size_t n;
            sscanf(line, "%zu", &n);

            for (size_t i = 0; i < vec_size(&ranges); ++i) {
                const Range *range = vec_get(&ranges, i);

                if (n >= range->min && n <= range->max) {
                    ++sum;
                    break;
                }
            }
        }

        fclose(fline);
    }
    FOR_EACH_LINE_END(line);

    printf("%zu\n", sum);

    return 0;
}
