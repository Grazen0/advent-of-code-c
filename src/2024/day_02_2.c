#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int sum = 0;

    char *line = nullptr;

    while (true) {
        size_t alloc_size = 0;
        const ssize_t len = getline(&line, &alloc_size, stdin);

        if (len == -1)
            break;

        FILE *const fline = fmemopen(line, len, "r");

        int dir = 0;
        int n = -1;
        int last_n = -1;
        bool removed_level = false;

        while (fscanf(fline, "%i", &n) != EOF) {
            if (last_n == -1) {
                last_n = n;
                continue;
            }

            const int prev_dir = dir;
            if (dir == 0)
                dir = n - last_n;

            const int diff = abs(n - last_n);

            const bool dir_check = dir > 0 ? n > last_n : n < last_n;
            const bool diff_check = diff >= 1 && diff <= 3;

            if (!dir_check || !diff_check) {
                if (removed_level)
                    goto skip;

                dir = prev_dir;
                removed_level = true;
                continue;
            }

            last_n = n;
        }

        ++sum;
skip:
        fclose(fline);
    }

    free(line);

    printf("%i\n", sum);
    return 0;
}
