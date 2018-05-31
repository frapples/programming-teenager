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
void max_chaos_value(int nums[], int arr[], int n) {
    if (n == 1) {
        int value = 0;
        for (int i = 0; i < (arr + n - nums); i++) {
            if ( i > 0) {
                value += abs(nums[i] - nums[i - 1]);
            }
        }
        total_max = int_max(total_max, value);
        return;
    }

    for (int i = 0; i < n; i++) {
        int_swap(&arr[0], &arr[i]);

        max_chaos_value(nums, arr + 1, n - 1);

        int_swap(&arr[0], &arr[i]);
    }
}

int main(void)
{
    int n;
    scanf("%d", &n);

    int* arr = (int *)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    max_chaos_value(arr, arr, n);
    printf("%d\n", total_max);
    return 0;
}

