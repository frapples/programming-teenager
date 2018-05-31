/*
  编程题] 餐馆

  时间限制：1秒

  空间限制：32768K
  某餐馆有n张桌子，每张桌子有一个参数：a 可容纳的最大人数； 有m批客人，每批客人有两个参数:b人数，c预计消费金额。 在不允许拼桌的情况下，请实现一个算法选择其中一部分客人，使得总预计消费金额最大
  输入描述:

  输入包括m+2行。
  第一行两个整数n(1 <= n <= 50000),m(1 <= m <= 50000)
  第二行为n个参数a,即每个桌子可容纳的最大人数,以空格分隔,范围均在32位int范围内。
  接下来m行，每行两个参数b,c。分别表示第i批客人的人数和预计消费金额,以空格分隔,范围均在32位int范围内。



  输出描述:

  输出一个整数,表示最大的总预计消费金额


  输入例子1:

  3 5
  2 4 2
  1 3
  3 5
  3 7
  5 9
  1 10


  输出例子1:

  20


 */

/*
  感想：1. 又是坑爹的整数溢出。。。
  2. 性能问题用优先队列解决(没怎么想明白)
  3. 用std::multiset解决。这个数据结构用来存储有序的，重复的set。
  std::multiset::lower_bound：返回set中第一个不小于给定参数的元素迭代器。（感觉应该是二叉搜索树）

  好吧。优化的关键其实在剪枝。
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>

typedef struct PersonGroup_s {
    int count;
    int pay;
}PersonGroup;

bool cmp(const PersonGroup& a, const PersonGroup& b) {
    if (a.pay == b.pay) {
        return a.count < b.count;
    } else {
        return a.pay > b.pay;
    }
}

int main(int argc, char *argv[])
{
    int n, m;
    scanf("%d%d", &n, &m);

    std::vector<int> tables;
    for (int i = 0; i < n; i++) {
        int num;
        scanf("%d", &num);
        tables.push_back(num);
    }

    std::sort(tables.begin(), tables.end());
    int max_desk = tables[tables.size() - 1];

    std::vector<PersonGroup> person_group;
    for (int i = 0; i < m; i++) {
        PersonGroup p;
        scanf("%d%d", &p.count, &p.pay);
        /* 剪枝 */
        if (p.count <= max_desk) {
            person_group.push_back(p);
        }
    }

    std::sort(person_group.begin(), person_group.end(), cmp);

    long sum = 0;
    int remain_desk_count = n; /* 剪枝 */
    for (size_t i = 0; i < person_group.size(); i++) {
        if (remain_desk_count <= 0) {
            break;
        }
        for (int j = 0; j < n; j++) {
            if (tables[j] >= person_group[i].count) {
                sum += person_group[i].pay;
                tables[j] = 0;
                remain_desk_count--;
                break;
            }
        }
    }

    printf("%ld\n", sum);

    return 0;
}
