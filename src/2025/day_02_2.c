#include "lib/macros.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

bool is_id_valid(long long id)
{
    static const size_t BUF_SIZE = 12;

    char num_str[BUF_SIZE];

    int size = snprintf(num_str, BUF_SIZE, "%lli", id);
    PANIC_IF(size == -1, "could not snprintf");

    for (int i = 1; i <= size / 2; ++i) {
        if ((size % i) != 0)
            continue;

        bool valid = false;

        for (int j = 0; j < i; ++j) {
            for (int k = 1; k < size / i; ++k) {
                if (num_str[j] != num_str[(i * k) + j]) {
                    valid = true;
                    goto skip;
                }
            }
        }

skip:
        if (!valid)
            return false;
    }

    return true;
}

int main(void)
{
    long long first = -1;
    long long last = -1;

    long long sum = 0;

    while (true) {
        BREAK_EOF(scanf("%lli-%lli", &first, &last));
        scanf(",");

        for (long long id = first; id <= last; ++id) {
            if (!is_id_valid(id))
                sum += id;
        }
    }

    printf("%lli\n", sum);

    return 0;
}
