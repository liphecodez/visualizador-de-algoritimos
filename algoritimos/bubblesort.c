typedef struct
{
    int round, pos, changed, sorted;
} bubblesortstate;

static bubblesortstate state = {1,0,0,0};

void BubbleSort(int arr[], int size){

    int holder;

    if (state.pos < size - state.round && state.sorted == 0)
    {
        if (arr[state.pos] > arr[state.pos+1])
        {
            holder = arr[state.pos];
            arr[state.pos] = arr[state.pos+1];
            arr[state.pos+1] = holder;

            state.changed = 1;
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
        }
    }
}