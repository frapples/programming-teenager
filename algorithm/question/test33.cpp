
/*
  时间限制：1秒

  空间限制：262144K
  美团点评是综合性生活服务平台，覆盖吃喝玩乐。在休闲娱乐版块，有很多轰趴、桌游、密室逃脱类的项目，适合多人一起玩。下面就是出自团队游戏场景中的一个问题。
  有 n 个人排成一排，一开始全部面向前方，然后大家一起转身，随机朝左或是朝右转。
  转身后，不断检查队列，如果存在两个面对面的相邻的人，则将这两个人从队列中消除；直到不存在两个面对面的相邻的人。
  例如 > 表示向右， < 表示向左
  队列“>>><<<”的消除过程为，“>>><<<”到“>><<”到“><”到“”（每次去除一对），最后剩下人数为0。
  队列“>><><<<>”的消除过程为，“>><><<<>”到“>><<<>”到“><<>”到“<>”（每次去除一对)，最后剩下人数为2
  求最后剩下人数的期望值。
  输入描述:

  一行一个正整数 n (1 ≤ n ≤ 2000)。



  输出描述:

  一行一个实数，表示剩下人数的期望值，四舍五入保留三位小数。


  输入例子1:

  10


  输出例子1:

  4.168

 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int n;
    scanf("%d", &n);

    int E = 0;
    if (n == 2) {
        E = 1.0 / 4.0 * 0 + 1.0 / 4.0 * 2;
    }

    return 0;
}
