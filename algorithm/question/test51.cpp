#include <stdio.h>

int cal(int nums[], int n, int start, int k, int mod) {
    if (start == n - 1) {
        if (nums[start] % k == mod) {
            return 1;
        } else {
            return 0;
        }
    } else {
        int new_mod = ((nums[start] / k) * (k + 1) + mod - nums[start]) % k;
        return cal(nums, n, start + 1, k, new_mod) + 1;
    }


}

int cal_dp(int nums[], int n, int k) {
    // int* table = new int[n * k];
// #define GET(i, j) (table[(i) * (k) + (j)])
    int* table = new int[2 * k];
#define GET(i, j) (table[((i) % 2) * (k) + (j)])

    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < k; j++) {
            if (i == n - 1) {
                if (nums[i] % k == j) {
                    GET(i, j) = 1;
                } else {
                    GET(i, j) = 0;
                }
            } else {
                int new_mod = ((long)(nums[i] / k + 1) * k + j - nums[i]) % k;
                GET(i, j) = GET(i + 1, new_mod) + 1;
            }
        }
    }


    return GET(0, 0);
}


int main(int argc, char *argv[])
{
    int n;
    scanf("%d", &n);

    int* nums = new int[n];

    for (int i = 0; i < n; i++) {
        scanf("%d", nums + i);
    }

    int k;
    scanf("%d", &k);


    // printf("%d\n", cal(nums, n, 0, k, 0));
    printf("%d\n", cal_dp(nums, n, k));

    delete[] nums;
    return 0;
}
