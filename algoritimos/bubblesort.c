void BubbleSort(int arr[], int size){

    int holder, i;
    int sorted = 0;

    while (!sorted)
    {
        sorted = 1;
        for (i = 0; i < size-1; i++)
        {
            if (arr[i] > arr[i+1])
            {
                holder = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = holder;
                sorted = 0;
            }
        }

        size -= 1;
    }
}