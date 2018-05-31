#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <assert.h>
/*
  跳石板

  时间限制：1秒

  空间限制：32768K
  小易来到了一条石板路前，每块石板上从1挨着编号为：1、2、3.......
  这条石板路要根据特殊的规则才能前进：对于小易当前所在的编号为K的 石板，小易单次只能往前跳K的一个约数(不含1和K)步，即跳到K+X(X为K的一个非1和本身的约数)的位置。 小易当前处在编号为N的石板，他想跳到编号恰好为M的石板去，小易想知道最少需要跳跃几次可以到达。
  例如：
  N = 4，M = 24：
  4->6->8->12->18->24
  于是小易最少需要跳跃5次，就可以从4号石板跳到24号石板
  输入描述:

  输入为一行，有两个整数N，M，以空格隔开。
  (4 ≤ N ≤ 100000)
  (N ≤ M ≤ 100000)



  输出描述:

  输出小易最少需要跳跃的步数,如果不能到达输出-1


  输入例子1:

  4 24


  输出例子1:

  5


 */

int* cache;

std::vector<int>* fac_table;

int jump(int start, int dest) {
    if (start == dest) {
        return 0;
    }

    if (start > dest) {
        return -1;
    }

    #define INF 0xfffff
    if (cache[start] == -2) {
        int min = INF;
        for (std::vector<int>::iterator it = fac_table[start].begin(); it != fac_table[start].end(); it++) {
            int a = jump(start + *it, dest);
            if (a >= 0) {
                min = a < min ? a : min;
            }
        }

        if (min < INF) {
            cache[start] = min + 1;
        } else {
            cache[start] = -1;
        }
    }

    return cache[start];
}

int main(void)
{
    int N, M;
    scanf("%d%d", &N, &M);

    cache = new int[M + 1];
    fac_table = new std::vector<int>[M + 1];
    for (int i = 0; i < M + 1; i++) {
        cache[i] = -2;
    }


    for (int i = 2; i <= M; i++) {
        for (int j = i; j <= M; j++) {
            long result = (long)i * (long)j;
            if (result > M) {
                break;
            } else {
                fac_table[i * j].push_back(i);
                if (i != j) {
                    fac_table[i * j].push_back(j);
                }
            }
        }
    }

    printf("%d\n", jump(N, M));

    return 0;
}
