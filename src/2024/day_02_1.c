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

        while (fscanf(fline, "%i", &n) != EOF) {
            if (last_n != -1) {
                if (dir == 0)
                    dir = n - last_n;

                if (dir > 0 ? n < last_n : n > last_n)
                    goto skip;

                const int diff = abs(last_n - n);
                if (diff < 1 || diff > 3)
                    goto skip;
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
