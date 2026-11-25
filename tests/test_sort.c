#include "algoritimos/bubblesort.h"
#include "algoritimos/insertionsort.h"
#include "algoritimos/mergesort.h"
#include "algoritimos/quicksort.h"
#include "algoritimos/selectionsort.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x)                                                                                   \
    do {                                                                                           \
        if (!(x)) {                                                                                \
            fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #x);                                   \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)
typedef algorithmpack (*Sort)(int *, int, int);
static Sort sorts[] = {BubbleSort, SelectionSort, InsertionSort, MergeSort, QuickSort};
static int compare(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}
static int cases;
static void run(Sort sort, const int *input, int n) {
    int a[SORT_CAPACITY + 2], expected[SORT_CAPACITY];
    a[0] = 123456;
    a[n + 1] = 654321;
    memcpy(a + 1, input, (size_t)n * sizeof(int));
    memcpy(expected, input, (size_t)n * sizeof(int));
    qsort(expected, n, sizeof(int), compare);
    algorithmpack p = {0}, prev = {0};
    int steps = 0;
    do {
        prev = p;
        p = sort(a + 1, n, steps == 0);
        CHECK(p.comparisons >= prev.comparisons);
        CHECK(p.movements >= prev.movements);
        CHECK(p.comparisons - prev.comparisons <= 1);
        CHECK(p.movements - prev.movements <= 2);
        CHECK(p.x >= -1 && p.x < n);
        CHECK(p.y >= -1 && p.y < n);
        CHECK(++steps < 50000);
    } while (!p.sorted);
    CHECK(memcmp(a + 1, expected, (size_t)n * sizeof(int)) == 0);
    CHECK(a[0] == 123456 && a[n + 1] == 654321);
    prev = sort(a + 1, n, 0);
    CHECK(prev.sorted);
    CHECK(prev.comparisons == p.comparisons && prev.movements == p.movements);
    CHECK(memcmp(a + 1, expected, (size_t)n * sizeof(int)) == 0);
    cases++;
}
int main(void) {
    srand(42);
    int a[SORT_CAPACITY] = {0};
    for (int s = 0; s < 5; s++) {
        for (int n = 0; n <= SORT_CAPACITY; n++) {
            for (int mode = 0; mode < 6; mode++) {
                for (int i = 0; i < n; i++)
                    a[i] = mode == 0   ? i
                           : mode == 1 ? n - i
                           : mode == 2 ? 7
                           : mode == 3 ? rand() % 7 - 3
                           : mode == 4 ? (i % 2 ? INT_MIN : INT_MAX)
                                       : rand() % 10000 - 5000;
                run(sorts[s], a, n);
            }
        }
        /* Abort a run, then reset with a new input and size. */
        for (int i = 0; i < 20; i++)
            a[i] = 20 - i;
        sorts[s](a, 20, 1);
        sorts[s](a, 20, 0);
        run(sorts[s], a, 7);
        int pair[] = {2, 1};
        run(sorts[s], pair, 2);
        algorithmpack p = sorts[s](pair, 2, 1);
        CHECK(p.comparisons <= 1);
    }
    int pair[] = {2, 1};
    algorithmpack p = BubbleSort(pair, 2, 1);
    CHECK(p.comparisons == 1 && p.movements == 2);
    int ordered[] = {1, 2, 3};
    p = SelectionSort(ordered, 3, 1);
    while (!p.sorted) {
        p = SelectionSort(ordered, 3, 0);
    }
    CHECK(p.comparisons == 3 && p.movements == 0);
    printf("PASS: %d cases; 5 algorithms; sorting, permutation, bounds, reset, "
           "counters, incremental limits, completion.\n",
           cases);
    return 0;
}
