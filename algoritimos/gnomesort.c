#include "gnomesort.h"
#include "sort-operations.h"

static int i;
static algorithmpack p;

algorithmpack GnomeSort(int a[], int n, int reset) {
    if (reset) {
        i = 1;
        p = (algorithmpack){.x = -1, .y = -1, .sorted = n < 2 || n > SORT_CAPACITY};
    }
    if (p.sorted)
        return p;

    sort_compare_swap(a, i - 1, i, &p);
    if (!p.changed)
        i++;
    else if (i > 1)
        i--;
    p.sorted = i == n;
    return p;
}
