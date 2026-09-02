#include "algorithmpack.h"

typedef struct
{
    int round, pos, sorted, pos_menor;
} selectionsortstate;

static selectionsortstate state = {0,1,0,0};

algorithmpack SelectionSort(int arr[], int size){

    algorithmpack pack = {0};

    int holder;

    if (state.round < size - 1 && state.sorted == 0)
    {
        if (state.pos < size)
        {
            pack.x = state.pos_menor;
            pack.y = state.pos;
            pack.changed = 0;

            if (arr[state.pos] < arr[state.pos_menor])
            {
                state.pos_menor = state.pos;
            }

            state.pos += 1;
        }
        else
        {
            pack.x = state.round;
            pack.y = state.pos_menor;
            pack.changed = 1;

            holder = arr[state.pos_menor];
            arr[state.pos_menor] = arr[state.round];
            arr[state.round] = holder;

            state.round += 1;
            state.pos_menor = state.round;
            state.pos = state.round + 1;
        }
    }
    else
    {
        state.sorted = 1;
        pack.sorted = 1;
    }
    

    return pack;
}