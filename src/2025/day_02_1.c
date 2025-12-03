#include "lib/macros.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

bool is_id_valid(long long id)
{
    static constexpr size_t BUF_SIZE = 12;

    char buf[BUF_SIZE];
    buf[BUF_SIZE - 1] = '\0';

    size_t size = 0;
    char *num_str = &buf[BUF_SIZE - 1];

    long long n = id;

    if (n == 0) {
        *(--num_str) = '0';
        ++size;
    } else {
        while (n > 0) {
            *(--num_str) = '0' + (n % 10);
            n /= 10;
            ++size;
        }
    }

    if ((size % 2) != 0)
        return true;

    for (size_t i = 0; i < size / 2; ++i) {
        if (num_str[i] != num_str[(size / 2) + i])
            return true;
    }

    return false;
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
