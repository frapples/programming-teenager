#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>

/*


  时间限制：1秒

  空间限制：32768K
  小易是一个数论爱好者，并且对于一个数的奇数约数十分感兴趣。一天小易遇到这样一个问题： 定义函数f(x)为x最大的奇数约数，x为正整数。 例如:f(44) = 11.
  现在给出一个N，需要求出 f(1) + f(2) + f(3).......f(N)
  例如： N = 7
  f(1) + f(2) + f(3) + f(4) + f(5) + f(6) + f(7) = 1 + 1 + 3 + 1 + 5 + 3 + 7 = 21
  小易计算这个问题遇到了困难，需要你来设计一个算法帮助他。
  输入描述:

  输入一个整数N (1 ≤ N ≤ 1000000000)



  输出描述:

  输出一个整数，即为f(1) + f(2) + f(3).......f(N)


  输入例子1:

7


输出例子1:

21
 */

/*
  看数据规模就知道：1000000000
  起码要logN*N规模的。

  经过分析发现：f(奇数)的值就是本身，因此奇数直接用等差数列公式算（考虑溢出）

  那么问题是剩下的偶数部分：2 4 6 8 10 ..
  我们发现f(2 * N) = f(N)

  偶数部分化归为： 1 2 3 4 5 ... N / 2
  递归即可在对数时间内解决。

 */
unsigned long cal(int n) {
    if (n == 1) {
        return 1;
    }

    unsigned long sum = 0;
    int max_odd = (n % 2 == 0) ? n - 1 : n;
    sum += (unsigned long)(max_odd + 1) * (unsigned long)(max_odd + 1) / 2 / 2;

    // for (int i = 2; i <= n; i+= 2) {
    //     int n = i;
    //     while (n % 2 == 0) {
    //         n /= 2;
    //     }
    //     sum += n;
    // }
    sum += cal((n % 2 == 0 ? n : n - 1) / 2);

    return sum;
}

int main(void)
{
    int N;
    scanf("%d", &N);


    printf("%lu\n", cal(N));
    return 0;
}
