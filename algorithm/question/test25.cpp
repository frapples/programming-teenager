/*
  [编程题] 数字和为sum的方法数

  时间限制：1秒

  空间限制：32768K
  给定一个有n个正整数的数组A和一个整数sum,求选择数组A中部分数字和为sum的方案数。
  当两种选取方案有一个数字的下标不一样,我们就认为是不同的组成方案。
  输入描述:

  输入为两行:

  第一行为两个正整数n(1 ≤ n ≤ 1000)，sum(1 ≤ sum ≤ 1000)

  第二行为n个正整数A[i](32位整数)，以空格隔开。



  输出描述:

  输出所求的方案数


  输入例子1:

  5 15
  5 5 10 2 3


  输出例子1:

  4


 */



#include<stdio.h>
#include<stdlib.h>

int cal(int nums[], int n, int start, int sum) {
    if (start == n) {
        if (sum == 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return cal(nums, n, start + 1, sum - nums[start]) + cal(nums, n, start + 1, sum);
    }
}


long cal_dp(int nums[], int n, int sum) {
    long* table = (long *)malloc(sizeof(long) * (n + 1) * (sum + 1));
#define GET(i, j) (table[(i) * (sum + 1) + (j)])
    /* 注意点1:二维数组的宏定义不能写成(table[(i) * (n + 1) + (j)]) */
    /* n注意点2: 小心溢出 */

    for (int i = n; i >= 0; i--) {
        for (int j = 0; j <= sum; j++) {
            if (i == n) {
                if (j == 0) {
                    GET(i, j) = 1;
                } else {
                    GET(i, j) = 0;
                }
            } else {
                GET(i, j) = GET(i + 1, j);
                int t = j - nums[i];
                if (t >= 0) {
                    GET(i, j) += GET(i + 1, t);
                }
            }

        }
    }

    return GET(0, sum);
}

int main(int argc, char *argv[])
{

    int n, sum;
    scanf("%d%d", &n, &sum);
    int* nums = (int *)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }

    // printf("%d\n", cal(nums, n, 0, sum));
    printf("%ld\n", cal_dp(nums, n, sum));

    return 0;
}
