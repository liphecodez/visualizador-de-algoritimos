#include "insertionsort.h"
static struct {
    int i, j, key, phase;
    algorithmpack p;
} s;
algorithmpack InsertionSort(int a[], int n, int reset) {
    if (reset) {
        s.i = 1;
        s.phase = 0;
        s.p = (algorithmpack){.x = -1, .y = -1};
    }
    s.p.changed = 0;
    if (s.p.sorted)
        return s.p;
    if (s.i >= n) {
        s.p.sorted = 1;
        return s.p;
    }
    if (!s.phase) {
        s.key = a[s.i];
        s.j = s.i - 1;
        s.phase = 1;
    }
    s.p.x = s.j;
    s.p.y = s.j + 1;
    if (s.j >= 0) {
        s.p.comparisons++;
        if (a[s.j] > s.key) {
            a[s.j + 1] = a[s.j];
            s.j--;
            s.p.movements++;
            s.p.changed = 1;
            return s.p;
        }
    }
    a[s.j + 1] = s.key;
    s.p.movements++;
    s.p.changed = 1;
    s.i++;
    s.phase = 0;
    return s.p;
}
