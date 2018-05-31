#include <stdio.h>

int int_min(int a, int b) {
    return a < b ? a : b;
}

int water_count(int arr[], int start, int end)
{
    int count = 0;
    if (end - start > 1) {
        int div = int_min(arr[start], arr[end]);
        for (int i = start + 1; i < end; i++) {
            count += div - arr[i];
        }
    }
    return count;
}

int main(void)
{
    int arr [] = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    int count = 0;
    int first_i = 0;
    int end_i = 0;
    int min = 0;
    for (int i = 0; i < (int)(sizeof(arr) / sizeof(*arr)) /* arr.length */ ; i++) {
        if (arr[i] > min) {
            end_i = i;
        }

        if (arr[i] >= arr[first_i]) {
            count += water_count(arr, first_i, i);
            min = arr[i];
            first_i = i;
        } else {
        }

        min = int_min(min, arr[i]);
    }
    count += water_count(arr, first_i, end_i);

    printf("%d\n", count);
    return 0;
}
