#include<stdio.h>
#include<iostream>
#include<stdlib.h>
/*
  给定一组非负整数组成的数组h，代表一组柱状图的高度，其中每个柱子的宽度都为1。 在这组柱状图中找到能组成的最大矩形的面积（如图所示）。 入参h为一个整型数组，代表每个柱子的高度，返回面积的值。 

  输入描述:

  输入包括两行,第一行包含一个整数n(1 ≤ n ≤ 10000)
  第二行包括n个整数,表示h数组中的每个值,h_i(1 ≤ h_i ≤ 1,000,000)



  输出描述:

  输出一个整数,表示最大的矩阵面积。


  输入例子1:

  6
  2 1 5 6 2 3


  输出例子1:

  10
*/

#include<stdio.h>
#include<iostream>
#include<stdlib.h>

static inline int int_min(int a, int b);
static inline int int_max(int a, int b);

int main(void)
{
    int n;
    scanf("%d", &n);

    int* highs = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", highs + i);
    }

    int max_area = 0;
    for (int i = 0; i < n; i++) {
        int min_highs = highs[i];
        for (int j = i; j < n; j++) {
            min_highs = int_min(min_highs, highs[j]);
            max_area = int_max(max_area, min_highs * ((j - i) + 1));
        }
    }

    printf("%d\n", max_area);
    return 0;
}

static inline int int_min(int a, int b) {
    return a < b ? a : b;
}

static inline int int_max(int a, int b) {
    return a > b ? a : b;
}
