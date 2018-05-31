/*
  [编程题] 末尾0的个数

  时间限制：1秒

  空间限制：32768K
  输入一个正整数n,求n!(即阶乘)末尾有多少个0？ 比如: n = 10; n! = 3628800,所以答案为2
  输入描述:

  输入为一行，n(1 ≤ n ≤ 1000)



  输出描述:

  输出一个整数,即题目所求


  输入例子1:

  10


  输出例子1:

  2



 */



#include<stdio.h>

void question5();
int main(int argc, char *argv[])
{

    question5();
    return 0;
}

void convert(int n, int base, char* result) {
    static char num_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    int size = 0;
    while (n != 0) {
        result[size++] = num_table[n % base];
        n /= base;
    }

    for (int i = 0; i < size / 2; i++) {
        char tmp = result[i];
        result[i] = result[size - i - 1];
        result[size - i - 1] = tmp;
    }
    result[size] = '\0';

}

void question5() {
    int m, n;
    scanf("%d%d", &m, &n);

    char result[32 * 8 + 1];
    convert(m > 0 ? m : -m, n, result);
    printf("%s%s\n", m > 0 ? "" : "-", result);
}

void question4() {
    int n;
    scanf("%d", &n);

    int count = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i; j % 5 == 0; j /= 5) {
            count++;
        }
    }

    printf("%d\n", count);
}
