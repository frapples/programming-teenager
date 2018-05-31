#include <stdio.h>

int main(int argc, char *argv[])
{
    int n;
    scanf("%d", &n);

    int* nums = new int[n];

    for (int i = 0; i < n; i++) {
        scanf("%d", nums + i);
    }

    int s = nums[0];
    bool yes = true;
    for (int i = 1; i < n; i++) {
        if (s >= nums[i]) {
            s -= nums[i];
        } else {
            yes = false;
        }
    }


    printf("Yes");
    return 0;
}
