#ifndef ALGORITHMPACK_H
#define ALGORITHMPACK_H
#include <stdint.h>
#define SORT_CAPACITY 95

typedef struct {
    /* Indices refer to the main array; -1 means no highlighted element. */
    int x, y, changed, sorted;
    /* Counting Sort reports unsupported input before modifying the array. */
    int invalid;
    /* Cumulative value comparisons and writes to main/auxiliary arrays. */
    uint64_t comparisons, movements;
} algorithmpack;

#endif
