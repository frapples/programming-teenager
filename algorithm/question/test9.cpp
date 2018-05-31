#include <stdio.h>
#include <stdlib.h>

int abs(int a) {
    return a > 0 ? a : -a;
}

int int_max(int a, int b) {
    return a > b ? a : b;
}

void int_swap(int *a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int total_max = 0;
int max_chaos_value(int nums[], int arr[], int n, int chaos) {
    if (n == 1) {
        // int value = 0;
        // for (int i = 0; i < (arr + n - nums); i++) {
        //     if ( i > 0) {
        //         value += abs(nums[i] - nums[i - 1]);
        //     }
        // }
        // total_max = int_max(total_max, value);
        return 0;
    }

    int max_chaos = 0;
    for (int i = 0; i < n; i++) {
        int_swap(&arr[0], &arr[i]);

        int sub_chaos = max_chaos_value(nums, arr + 1, n - 1, chaos + abs(arr[0] - arr[1]));
        max_chaos = int_max(max_chaos, sub_chaos);

        int_swap(&arr[0], &arr[i]);
    }
    return max_chaos;
}

int main(void)
{
    int n;
    scanf("%d", &n);

    int* arr = (int *)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    total_max = max_chaos_value(arr, arr, n, 0);
    printf("%d\n", total_max);
    return 0;
}
