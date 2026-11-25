#include "countingsort.h"

#include <string.h>

static struct {
    int buckets[COUNTING_MAX + 1];
    int i, value, writing;
    algorithmpack p;
} s;

/* Reconstruct from the histogram. Frequencies are metadata, not element writes. */
algorithmpack CountingSort(int a[], int n, int reset) {
    if (reset) {
        memset(&s, 0, sizeof(s));
        s.p = (algorithmpack){.x = -1, .y = -1, .sorted = n == 0,
                             .invalid = n < 0 || n > SORT_CAPACITY};
    }
    if (s.p.sorted || s.p.invalid)
        return s.p;

    s.p.x = s.i;
    if (!s.writing) {
        const int value = a[s.i];
        if (value < 0 || value > COUNTING_MAX) {
            s.p.invalid = 1;
            return s.p;
        }
        s.buckets[value]++;
        if (++s.i == n) {
            s.i = 0;
            s.writing = 1;
        }
        return s.p;
    }

    while (!s.buckets[s.value])
        s.value++;
    a[s.i++] = s.value;
    s.buckets[s.value]--;
    s.p.changed = 1;
    s.p.movements++;
    s.p.sorted = s.i == n;
    return s.p;
}
