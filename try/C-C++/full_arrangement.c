// vim: sts=4 sw=4 et
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

void select(int nums[], size_t size, int select_count, int result[], int *result_head, size_t result_size);

int main(int argc, char *argv[])
{
    printf("%ld\n%ld\n%ld\n", (long)sizeof(intptr_t), (long)sizeof(void *), (long)sizeof(uintmax_t));
    printf("%ld\n%ld\n", (long)sizeof(size_t), (long)sizeof(ptrdiff_t));
    printf("\n");

    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int result[5];
    select(nums, 10, 5, result, result, 5);
    return 0;
}


void array_dump(int array[], size_t size)
{
    size_t i;
    for (i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void select(int nums[], size_t size, int select_count, int result[], int *result_head, size_t result_size)
{
    if (select_count == 0) {
        array_dump(result_head, result_size);
    } else if (select_count > (int)size || size == 0) {

    } else {
        result[0] = nums[0];
        select(nums + 1, size - 1, select_count - 1, result + 1, result_head, result_size);

        select(nums + 1, size - 1, select_count, result, result_head, result_size);
    }
}
