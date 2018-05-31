/*


  时间限制：1秒

  空间限制：32768K
  一个数组有 N 个元素，求连续子数组的最大和。 例如：[-1,2,1]，和最大的连续子数组为[2,1]，其和为 3
  输入描述:

  输入为两行。
  第一行一个整数n(1 <= n <= 100000)，表示一共有n个元素
  第二行为n个数，即每个元素,每个整数都在32位int范围内。以空格分隔。



  输出描述:

  所有连续子数组中和最大的值。


  输入例子1:

  3
  -1 2 1


  输出例子1:

  3


 */

/*
  解法一：
  int maxSoFar = Integer.MIN_VALUE, maxCur = 0;
  for(i = 0; i < n; i++) {
    maxCur = Math.max(maxCur + arr[i], arr[i]);
    maxSoFar = Math.max(maxCur, maxSoFar);
  }

  解法二：DP思路
  设dp[i]表示以第 i个元素为结尾的连续子数组的最大和，则递推方程式为 dp[i]=max{dp[i-1]+a[i], a[i]};

  // 假设dp[n]表示以n为最后一个元素的子数组和的最大值，
  // 因此， dp[n] = max(dp[n-1],0)+num[n];
  // 当然实现的时候，没有必要设置一个数组保存所有的情况，因为只是用到了前一个位置的计算结果。
 */


#include<stdio.h>
#include<string.h>
#include<stdlib.h>

static inline int int_max(int a, int b);

int subseq_maxsum(int* nums, int n) {

    #define INF 0xffffff
    // int max = -INF;
    // for (int i = 0; i < n; i++) {
    //     int sum = 0;
    //     for (int j = i; j < n; j++) {
    //         sum += nums[j];
    //         max = int_max(max, sum);
    //     }
    // }

    int max = -INF;
    int i_when_max = 0;
    int sum = 0;
    for (int i = 0; i < n; i++) {
            sum += nums[i];
            if (sum >= max) {
                max = sum;
                i_when_max = i;
            }
    }


    if (i_when_max == 0) {
        max = -INF;
        for (int i = 0; i < n; i++) {
            max = int_max(max, nums[i]);
        }
    } else {
        sum = max;
        for (int i = 0; i < i_when_max; i++) {
            sum -= nums[i];
            max = int_max(max, sum);
        }
    }

    return max;
}

int main(int argc, char *argv[])
{
    int n;
    scanf("%d", &n);

    int* nums = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }

    printf("%d\n", subseq_maxsum(nums, n));
    return 0;
}


static inline int int_max(int a, int b) {
    return a > b ? a : b;
}
