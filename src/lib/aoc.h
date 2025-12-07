#ifndef AOC_LIB_AOC_H
#define AOC_LIB_AOC_H

#include "lib/macros.h"
#include "lib/str.h"

#define read_into_lines(file, lines)                      \
    do {                                                  \
        FOR_EACH_LINE_BEGIN(stdin, line, len, alloc_size) \
        {                                                 \
            String s = str_from(line);                    \
            str_trim_end(&s);                             \
            vec_push(lines, &s);                          \
        }                                                 \
        FOR_EACH_LINE_END(line);                          \
    } while (0)

#endif
