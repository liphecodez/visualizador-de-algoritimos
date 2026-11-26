#include "heapsort.h"
#include "sort-operations.h"

typedef enum { NEXT_ROOT, COMPARE_CHILDREN, COMPARE_ROOT } HeapPhase;

static struct {
    int next_root, end, root, child;
    HeapPhase phase;
    algorithmpack p;
} s;

/* Build a max heap bottom up, then extract one maximum at a time. */
algorithmpack HeapSort(int a[], int n, int reset) {
    if (reset) {
        s.next_root = n / 2 - 1;
        s.end = n;
        s.phase = NEXT_ROOT;
        s.p = (algorithmpack){.x = -1, .y = -1, .sorted = n < 2 || n > SORT_CAPACITY};
    }
    if (s.p.sorted)
        return s.p;

    if (s.phase == NEXT_ROOT) {
        if (s.next_root >= 0) {
            s.root = s.next_root--;
            s.phase = COMPARE_CHILDREN;
        } else {
            sort_swap(a, 0, --s.end, &s.p);
            s.root = 0;
            s.phase = COMPARE_CHILDREN;
            s.p.sorted = s.end == 1;
            return s.p;
        }
    }

    if (s.phase == COMPARE_CHILDREN) {
        s.child = 2 * s.root + 1;
        s.phase = COMPARE_ROOT;
        if (s.child + 1 < s.end) {
            s.p.x = s.child;
            s.p.y = s.child + 1;
            s.p.changed = 0;
            s.p.comparisons++;
            if (a[s.child] < a[s.child + 1])
                s.child++;
            return s.p;
        }
    }

    sort_compare_swap(a, s.child, s.root, &s.p);
    if (s.p.changed && 2 * s.child + 1 < s.end) {
        s.root = s.child;
        s.phase = COMPARE_CHILDREN;
    } else {
        s.phase = NEXT_ROOT;
    }
    return s.p;
}
