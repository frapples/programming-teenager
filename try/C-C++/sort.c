#include<time.h>
#include<stdlib.h>
#include<stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

void insert_sort(int unsorted[], size_t size);
void fill_random(int arr[], size_t size);
void array_print(int arr[], size_t size);

int main(void)
{
    srand(time(NULL));

    int arr[10];
    fill_random(arr, ARRAY_SIZE(arr));
    array_print(arr, ARRAY_SIZE(arr));

    insert_sort(arr, ARRAY_SIZE(arr));
    array_print(arr, ARRAY_SIZE(arr));

    return 0;
}

void insert_sort(int unsorted[], size_t size)
{
    for (size_t i = 1; i < size; i++) {
        int insert = unsorted[i];

        size_t j = i;
        while (j > 0 && unsorted[j - 1] > insert) {
            unsorted[j] = unsorted[j - 1];
            j--;
       }
       unsorted[j] = insert;
    }
}

void fill_random(int arr[], size_t size)
{
    for (size_t i = 0; i < size; i++)
        arr[i] = rand() % 200;
}

void array_print(int arr[], size_t size)
{
    for (size_t i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
