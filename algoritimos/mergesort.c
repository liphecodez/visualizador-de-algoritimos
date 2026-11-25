#include "mergesort.h"
/* Bottom-up merge: each call writes at most one array element. */
static struct {
    int width, left, mid, right, i, j, k, phase;
    int aux[SORT_CAPACITY];
    algorithmpack p;
} s;
static int min(int a, int b) { return a < b ? a : b; }
algorithmpack MergeSort(int a[], int n, int reset) {
    if (reset) {
        s.width = 1;
        s.left = 0;
        s.phase = 0;
        s.p = (algorithmpack){.x = -1, .y = -1};
    }
    s.p.changed = 0;
    if (s.p.sorted)
        return s.p;
    if (n > SORT_CAPACITY || n < 2) {
        s.p.sorted = 1;
        return s.p;
    }
    if (!s.phase) {
        if (s.left + s.width >= n) {
            s.width *= 2;
            s.left = 0;
        }
        if (s.width >= n) {
            s.p.sorted = 1;
            return s.p;
        }
        s.mid = min(s.left + s.width, n);
        s.right = min(s.left + 2 * s.width, n);
        s.i = s.left;
        s.j = s.mid;
        s.k = s.left;
        s.phase = 1;
    }
    if (s.phase == 1) {
        int source;
        if (s.i < s.mid && s.j < s.right) {
            s.p.comparisons++;
            source = a[s.i] <= a[s.j] ? s.i++ : s.j++;
        } else
            source = s.i < s.mid ? s.i++ : s.j++;
        s.p.x = source;
        s.p.y = s.k;
        s.aux[s.k++] = a[source];
        s.p.movements++;
        s.p.changed = 1;
        if (s.k == s.right) {
            s.k = s.left;
            s.phase = 2;
        }
    } else {
        s.p.x = s.p.y = s.k;
        a[s.k] = s.aux[s.k];
        s.k++;
        s.p.movements++;
        s.p.changed = 1;
        if (s.k == s.right) {
            s.left += 2 * s.width;
            s.phase = 0;
        }
    }
    return s.p;
}
