#include "lib/macros.h"
#include <stdio.h>

int main(void)
{

    int sum = 0;
    int dial = 50;

    FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size)
    {
        FILE *const fline = fmemopen(line, len, "r");

        char dir = '\0';
        int n = -1;
        fscanf(fline, "%c%i", &dir, &n);

        if (dir == 'L') {
            while (n-- > 0) {
                if (--dial < 0)
                    dial += 100;

                if (dial == 0)
                    ++sum;
            }
        } else {
            while (n-- > 0) {
                if (++dial >= 100)
                    dial -= 100;

                if (dial == 0)
                    ++sum;
            }
        }

        fclose(fline);
    }
    FOR_EACH_LINE_END(line);

    printf("%i\n", sum);

    return 0;
}
