#include "combsort.h"
#include "sort-operations.h"

static struct {
    int gap, i, swapped;
    algorithmpack p;
} s;

algorithmpack CombSort(int a[], int n, int reset) {
    if (reset) {
        s.gap = n;
        s.i = 0;
        s.swapped = 1;
        s.p = (algorithmpack){.x = -1, .y = -1, .sorted = n < 2 || n > SORT_CAPACITY};
    }
    if (s.p.sorted)
        return s.p;

    if (s.i == 0) {
        s.gap = s.gap * 10 / 13;
        if (s.gap < 1)
            s.gap = 1;
        s.swapped = 0;
    }
    sort_compare_swap(a, s.i, s.i + s.gap, &s.p);
    s.swapped |= s.p.changed;
    if (++s.i + s.gap == n) {
        s.i = 0;
        s.p.sorted = s.gap == 1 && !s.swapped;
    }
    return s.p;
}
