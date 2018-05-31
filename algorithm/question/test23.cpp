/*
  [编程题] 地下迷宫

  时间限制：1秒

  空间限制：32768K
  小青蛙有一天不小心落入了一个地下迷宫,小青蛙希望用自己仅剩的体力值P跳出这个地下迷宫。为了让问题简单,假设这是一个n*m的格子迷宫,迷宫每个位置为0或者1,0代表这个位置有障碍物,小青蛙达到不了这个位置;1代表小青蛙可以达到的位置。小青蛙初始在(0,0)位置,地下迷宫的出口在(0,m-1)(保证这两个位置都是1,并且保证一定有起点到终点可达的路径),小青蛙在迷宫中水平移动一个单位距离需要消耗1点体力值,向上爬一个单位距离需要消耗3个单位的体力值,向下移动不消耗体力值,当小青蛙的体力值等于0的时候还没有到达出口,小青蛙将无法逃离迷宫。现在需要你帮助小青蛙计算出能否用仅剩的体力值跳出迷宫(即达到(0,m-1)位置)。
  输入描述:

  输入包括n+1行:

  第一行为三个整数n,m(3 <= m,n <= 10),P(1 <= P <= 100)

  接下来的n行:

  每行m个0或者1,以空格分隔



  输出描述:

  如果能逃离迷宫,则输出一行体力消耗最小的路径,输出格式见样例所示;如果不能逃离迷宫,则输出"Can not escape!"。
  测试数据保证答案唯一


  输入例子1:

  4 4 10
  1 0 0 1
  1 1 0 1
  0 1 1 1
  0 0 1 1


  输出例子1:

  [0,0],[1,0],[1,1],[2,1],[2,2],[2,3],[1,3],[0,3]


 */
/*
  感悟：
  牛客网的输出一律用printf, puts会默认输出换行导致出错

  图遍历的重要一点：标记已经遍历过的

  不要犯复制粘贴的低级错误
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>

struct Node {
    bool is_open;
    bool is_traced;
};
Node* tables;
int n, m;
#define GET(i, j) (tables[(i) * m + (j)])

struct Point {
    int n;
    int m;

    Point(int n, int m): n(n), m(m) {}
};

int max_p = -1;
std::vector<Point> min_trace;
void dfs(int start_n, int start_m, int P, std::vector<Point>& trace) {
    if (start_n == 0 && start_m == m - 1 && P >= 0) {
        if (P > max_p) {
            max_p = P;
            min_trace = trace;
            min_trace.push_back(Point(start_n, start_m));
        }
        return;
    }

    if (P < 0 || !GET(start_n, start_m).is_open || GET(start_n, start_m).is_traced) {
        return;
    }

    trace.push_back(Point(start_n, start_m));
    GET(start_n, start_m).is_traced = true;

    if (start_n > 0) {
        dfs(start_n - 1, start_m, P - 3, trace);
    }

    if (start_n < n - 1) {
        dfs(start_n + 1, start_m, P, trace);
    }

    if (start_m > 0) {
        dfs(start_n, start_m - 1, P - 1, trace);
    }

    if (start_m < m - 1) {
        dfs(start_n, start_m + 1, P - 1, trace);
    }

    trace.pop_back();
    GET(start_n, start_m).is_traced = false;
}


int main(int argc, char *argv[])
{
    int P;
    scanf("%d%d%d", &n, &m, &P);

    tables = (Node*)malloc(sizeof(Node) * n * m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int is_open;
            scanf("%d", &is_open);
            GET(i, j).is_open = is_open == 1;
            GET(i, j).is_traced = false;
        }
    }

    std::vector<Point> trace;
    dfs(0, 0, P, trace);

    if (min_trace.size() == 0) {
        printf("Can not escape!");
    } else {
        for (size_t i = 0; i < min_trace.size(); i++) {
            printf("[%d,%d]", min_trace[i].n, min_trace[i].m);
            if (i < min_trace.size() - 1) {
                printf(",");
            }
        }
    }

    return 0;
}

