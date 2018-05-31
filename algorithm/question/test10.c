#include <stdio.h>

int main(void) {
    int arr[] = {15, 67, 26, 43, 61, 25, 84, 80, 34, 70};
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 1; j < sizeof(arr) / sizeof(*arr); j++) {
            if (arr[j] < arr[j - 1]) {
                int tmp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = tmp;
            }
        }
    }

    for (j = 0; j < sizeof(arr) / sizeof(*arr); j++) {
        printf("%d ", arr[j]);
    }
    return 0;

}
