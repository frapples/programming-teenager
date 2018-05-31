#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>

/*
  时间限制：1秒

  空间限制：32768K
  一个只包含'A'、'B'和'C'的字符串，如果存在某一段长度为3的连续子串中恰好'A'、'B'和'C'各有一个，那么这个字符串就是纯净的，否则这个字符串就是暗黑的。例如：
  BAACAACCBAAA 连续子串"CBA"中包含了'A','B','C'各一个，所以是纯净的字符串
  AABBCCAABB 不存在一个长度为3的连续子串包含'A','B','C',所以是暗黑的字符串
  你的任务就是计算出长度为n的字符串(只包含'A'、'B'和'C')，有多少个是暗黑的字符串。
  输入描述:

  输入一个整数n，表示字符串长度(1 ≤ n ≤ 30)



  输出描述:

  输出一个整数表示有多少个暗黑字符串


  输入例子1:

  2
  3


  输出例子1:

  9
  21

 */

/*
  感受：坑在于会溢出。。。
 */

int main(void)
{
    int N;
    scanf("%d", &N);

    long table[31];
    long end_diff_table[31];
    long end_same_table[31];
    table[2] = 9;
    end_diff_table[2] = 6;
    end_same_table[2] = 3;



    for (int i = 2; i < 30; i++) {
        table[i + 1] = end_same_table[i] * 3 + end_diff_table[i] * 2;


        end_same_table[i + 1] = table[i];

        end_diff_table[i + 1] = table[i + 1] - table[i];
    }

    printf("%ld\n", table[N]);

    return 0;
}
