#include "algorithmpack.h"

typedef struct
{
    int round, pos, changed, sorted;
} bubblesortstate;

static bubblesortstate state;

algorithmpack BubbleSort(int arr[], int size, int reset){

    if (reset == 1)
    {
        state.round = 1;
        state.pos = 0;
        state.changed = 0;
        state.sorted = 0;
    }
    

    algorithmpack pack;
    int holder;

    pack.x = state.pos;
    pack.y = state.pos + 1;
    pack.changed = 0;
    pack.sorted = 0;

    if (state.pos < size - state.round && state.sorted == 0)
    {
        if (arr[state.pos] > arr[state.pos+1])
        {
            holder = arr[state.pos];
            arr[state.pos] = arr[state.pos+1];
            arr[state.pos+1] = holder;

            state.changed = 1;
            pack.changed = 1;
        }
        
        state.pos += 1;
    }
    else
    {
        if (state.changed == 1)
        {
            state.pos = 0;
            state.changed = 0;
            state.round += 1;
        }
        else
        {
            state.sorted = 1;
            pack.sorted = 1;
        }
    }

    return pack;
}