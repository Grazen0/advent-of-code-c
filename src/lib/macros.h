#ifndef AOC_LIB_MACROS_H
#define AOC_LIB_MACROS_H

#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define PANIC(...)                                              \
    do {                                                        \
        fprintf(stderr, "PANIC (%s:%d)\n", __FILE__, __LINE__); \
        __VA_OPT__(fprintf(stderr, __VA_ARGS__);)               \
        __VA_OPT__(fprintf(stderr, "\n");)                      \
        exit(1);                                                \
    } while (0)

#define PANIC_IF(cond, ...)     \
    do {                        \
        if (cond) {             \
            PANIC(__VA_ARGS__); \
        }                       \
    } while (0)

#define BREAK_EOF(expr) \
    if ((expr) == EOF)  \
        break;

#define FOR_EACH_LINE_BEGIN(file, line, len, alloc_size) \
    do {                                                 \
        char *line = NULL;                               \
        size_t alloc_size = 0;                           \
        ssize_t len = -1;                                \
        while ((len = getline(&(line), &(alloc_size), (file))) != -1)

#define FOR_EACH_LINE_END(line) \
    free(line);                 \
    }                           \
    while (0)

#endif
