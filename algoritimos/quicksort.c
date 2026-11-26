#include "quicksort.h"
/* Lomuto partition, explicit pending ranges instead of blocking recursion. */
static struct {
    int lo, hi, i, j, pivot, active, top;
    int low[SORT_CAPACITY], high[SORT_CAPACITY];
    algorithmpack p;
} s;
static void swap(int a[], int x, int y) {
    if (x == y)
        return;
    int temp = a[x];
    a[x] = a[y];
    a[y] = temp;
    s.p.x = x;
    s.p.y = y;
    s.p.movements += 2;
    s.p.changed = 1;
}
algorithmpack QuickSort(int a[], int n, int reset) {
    if (reset) {
        s.active = 0;
        s.top = 0;
        s.p = (algorithmpack){.x = -1, .y = -1};
        if (n > 1 && n <= SORT_CAPACITY) {
            s.low[s.top] = 0;
            s.high[s.top++] = n - 1;
        }
    }
    s.p.changed = 0;
    if (s.p.sorted)
        return s.p;
    if (!s.active) {
        if (!s.top) {
            s.p.sorted = 1;
            return s.p;
        }
        s.lo = s.low[--s.top];
        s.hi = s.high[s.top];
        s.i = s.lo;
        s.j = s.lo;
        s.pivot = a[s.hi];
        s.active = 1;
    }
    if (s.j < s.hi) {
        s.p.x = s.j;
        s.p.y = s.hi;
        s.p.comparisons++;
        if (a[s.j] <= s.pivot) {
            swap(a, s.i, s.j);
            s.i++;
        }
        s.j++;
    } else {
        s.p.x = s.i;
        s.p.y = s.hi;
        swap(a, s.i, s.hi);
        if (s.lo < s.i - 1) {
            s.low[s.top] = s.lo;
            s.high[s.top++] = s.i - 1;
        }
        if (s.i + 1 < s.hi) {
            s.low[s.top] = s.i + 1;
            s.high[s.top++] = s.hi;
        }
        s.active = 0;
    }
    return s.p;
}
