#include "selectionsort.h"
typedef struct {
    int round, pos, pos_menor;
    algorithmpack pack;
} selectionsortstate;
static selectionsortstate state;
algorithmpack SelectionSort(int arr[], int size, int reset) {
    if (reset)
        state = (selectionsortstate){.pos = 1, .pack = {.x = -1, .y = -1}};
    state.pack.changed = 0;
    if (state.pack.sorted)
        return state.pack;
    if (state.round >= size - 1) {
        state.pack.sorted = 1;
        return state.pack;
    }
    state.pack.x = state.pos_menor;
    if (state.pos < size) {
        state.pack.y = state.pos;
        state.pack.comparisons++;
        if (arr[state.pos] < arr[state.pos_menor])
            state.pos_menor = state.pos;
        state.pos++;
    } else {
        state.pack.y = state.round;
        if (state.pos_menor != state.round) {
            int temp = arr[state.round];
            arr[state.round] = arr[state.pos_menor];
            arr[state.pos_menor] = temp;
            state.pack.movements += 2;
            state.pack.changed = 1;
        }
        state.round++;
        state.pos_menor = state.round;
        state.pos = state.round + 1;
    }
    return state.pack;
}
