#include "shellsort.h"
#include "sort-operations.h"

/* Insertion by exchanges within each gap, then halve the gap. */
static struct {
    int gap, next, i;
    algorithmpack p;
} s;

algorithmpack ShellSort(int a[], int n, int reset) {
    if (reset) {
        s.gap = n > 1 && n <= SORT_CAPACITY ? n / 2 : 0;
        s.i = s.next = s.gap;
        s.p = (algorithmpack){.x = -1, .y = -1, .sorted = s.gap == 0};
    }
    if (s.p.sorted)
        return s.p;

    sort_compare_swap(a, s.i - s.gap, s.i, &s.p);
    if (s.p.changed)
        s.i -= s.gap;
    if (!s.p.changed || s.i < s.gap) {
        if (++s.next == n) {
            s.gap /= 2;
            s.next = s.gap;
        }
        s.i = s.next;
        s.p.sorted = s.gap == 0;
    }
    return s.p;
}
