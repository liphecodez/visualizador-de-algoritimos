#include "cocktailsort.h"
#include "sort-operations.h"

static struct {
    int left, right, i, direction, swapped;
    algorithmpack p;
} s;

algorithmpack CocktailSort(int a[], int n, int reset) {
    if (reset) {
        s.left = s.i = s.swapped = 0;
        s.right = n > 1 && n <= SORT_CAPACITY ? n - 1 : 0;
        s.direction = 1;
        s.p = (algorithmpack){.x = -1, .y = -1, .sorted = s.right == 0};
    }
    if (s.p.sorted)
        return s.p;

    sort_compare_swap(a, s.i, s.i + 1, &s.p);
    s.swapped |= s.p.changed;
    s.i += s.direction;
    if (s.i == s.right || s.i < s.left) {
        if (s.direction > 0) {
            s.right--;
            s.i = s.right - 1;
        } else {
            s.left++;
            s.i = s.left;
        }
        s.p.sorted = !s.swapped || s.left >= s.right;
        s.direction = -s.direction;
        s.swapped = 0;
    }
    return s.p;
}
