#include "radixsort.h"

#include <limits.h>
#include <string.h>

#define RADIX 256u
typedef enum { HISTOGRAM, PREFIX, DISTRIBUTE, COPY_BACK } RadixPhase;

static struct {
    int offsets[RADIX], aux[SORT_CAPACITY];
    unsigned int divisor;
    int i, prefix;
    RadixPhase phase;
    algorithmpack p;
} s;

static unsigned int digit(int value) {
    /* Unsigned arithmetic maps INT_MIN to zero without signed overflow. */
    unsigned int key = (unsigned int)value - (unsigned int)INT_MIN;
    return key / s.divisor % RADIX;
}

algorithmpack RadixSort(int a[], int n, int reset) {
    if (reset) {
        memset(&s, 0, sizeof(s));
        s.divisor = 1;
        s.p = (algorithmpack){.x = -1, .y = -1, .sorted = n < 2 || n > SORT_CAPACITY};
    }
    if (s.p.sorted)
        return s.p;

    s.p.x = s.i;
    s.p.y = -1;
    s.p.changed = 0;
    if (s.phase == HISTOGRAM) {
        s.offsets[digit(a[s.i])]++;
        if (++s.i == n) {
            s.i = 0;
            s.phase = PREFIX;
        }
        return s.p;
    }

    if (s.phase == PREFIX) {
        /* One bounded metadata pass; the next element operation stays visible. */
        for (unsigned int bucket = 0; bucket < RADIX; bucket++) {
            int frequency = s.offsets[bucket];
            s.offsets[bucket] = s.prefix;
            s.prefix += frequency;
        }
        s.phase = DISTRIBUTE;
    }
    if (s.phase == DISTRIBUTE) {
        s.p.y = s.offsets[digit(a[s.i])]++;
        s.aux[s.p.y] = a[s.i];
        if (++s.i == n) {
            s.i = 0;
            s.phase = COPY_BACK;
        }
    } else {
        a[s.i] = s.aux[s.i];
        if (++s.i == n) {
            if (s.divisor > UINT_MAX / RADIX) {
                s.p.sorted = 1;
            } else {
                s.divisor *= RADIX;
                memset(s.offsets, 0, sizeof(s.offsets));
                s.i = s.prefix = 0;
                s.phase = HISTOGRAM;
            }
        }
    }
    s.p.changed = 1;
    s.p.movements++;
    return s.p;
}
