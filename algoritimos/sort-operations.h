#ifndef SORT_OPERATIONS_H
#define SORT_OPERATIONS_H
#include "algorithmpack.h"

static inline void sort_swap(int a[], int x, int y, algorithmpack *p) {
    int value = a[x];
    a[x] = a[y];
    a[y] = value;
    p->x = x;
    p->y = y;
    p->changed = 1;
    p->movements += 2;
}

static inline void sort_compare_swap(int a[], int x, int y, algorithmpack *p) {
    p->x = x;
    p->y = y;
    p->changed = 0;
    p->comparisons++;
    if (a[x] > a[y])
        sort_swap(a, x, y, p);
}
#endif
