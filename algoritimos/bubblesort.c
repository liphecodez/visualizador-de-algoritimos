#include "bubblesort.h"
typedef struct {
    int round, pos, changed;
    algorithmpack pack;
} bubblesortstate;
static bubblesortstate state;
algorithmpack BubbleSort(int arr[], int size, int reset) {
    if (reset)
        state = (bubblesortstate){.round = 1, .pack = {.x = -1, .y = -1}};
    state.pack.changed = 0;
    if (state.pack.sorted)
        return state.pack;
    if (size < 2 || state.round >= size) {
        state.pack.sorted = 1;
        return state.pack;
    }
    if (state.pos >= size - state.round) {
        if (!state.changed) {
            state.pack.sorted = 1;
            return state.pack;
        }
        state.pos = 0;
        state.changed = 0;
        state.round++;
        if (state.round >= size) {
            state.pack.sorted = 1;
            return state.pack;
        }
    }
    int p = state.pos++;
    state.pack.x = p;
    state.pack.y = p + 1;
    state.pack.comparisons++;
    if (arr[p] > arr[p + 1]) {
        int temp = arr[p];
        arr[p] = arr[p + 1];
        arr[p + 1] = temp;
        state.changed = state.pack.changed = 1;
        state.pack.movements += 2;
    }
    return state.pack;
}
