/*
  [编程题] 城市网络

  时间限制：2秒

  空间限制：262144K
  有一个树状的城市网络（即 n 个城市由 n-1 条道路连接的连通图），首都为 1 号城市，每个城市售卖价值为 a_i 的珠宝。
  你是一个珠宝商，现在安排有 q 次行程，每次行程为从 u 号城市前往 v 号城市（走最短路径），保证 v 在 u 前往首都的最短路径上。 在每次行程开始时，你手上有价值为 c 的珠宝（每次行程可能不同），并且每经过一个城市时（包括 u 和 v ），假如那个城市中售卖的珠宝比你现在手上的每一种珠宝都要优秀（价值更高，即严格大于），那么你就会选择购入。
  现在你想要对每一次行程，求出会进行多少次购买事件。
  输入描述:

  第一行，两个正整数 n , q (2 ≤ n ≤ 10^5 , 1 ≤ q ≤ 10^5)。
  第二行，n 个正整数 a_i (1 ≤ a_i ≤ 10^5) 描述每个城市售卖的珠宝的价值。
  接下来 n-1 行，每行描述一条道路 x , y (1 ≤ x,y ≤ n)，表示有一条连接 x 和 y 的道路。
  接下来 q 行，每行描述一次行程 u , v , c (1 ≤ u,v ≤ n , 1 ≤ c ≤ 10^5)。



  输出描述:

  对于每次行程输出一行，为所购买次数。


  输入例子1:

  5 4
  3 5 1 2 4
  1 2
  1 3
  2 4
  3 5
  4 2 1
  4 2 2
  4 2 3
  5 1 5


  输出例子1:

  2
  1
  1
  0




 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int n, q;
    scanf("%d%d", &n, &q);

    int* values = new int[n + 1];
    int* cost = new int[(n + 1) * (n + 1)];
#define GET(i, j) (cost[(i) * (n + 1) + (j)])

    for (int i = 1; i <= n; i++) {
        scanf("%d", &values[i]);
    }

    for (int i = 0 ; i < n - 1; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        GET(x, y) = 1;
    }

    for (int i = 0; i < q; i ++) {
        int u, v, c;
        scanf("%d%d%d", &u, &v, &c);


    }

    return 0;
}