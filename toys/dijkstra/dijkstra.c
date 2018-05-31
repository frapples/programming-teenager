#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* 算法参考《计算机网络自顶向下方法》中的网络层一章（解释的非常好） */
typedef struct{
    int size;
    bool undirected;
    double *cost;

}Grph;

/*
  图使用从0开始的连续整数标示节点，max_node_no为节点的最大编号。
  undirected表示此图是无向图
  权值必须是正数
*/

void grph_init(Grph* grph, int max_node_no, bool undirected);
double grph_cost(Grph* grph, int u, int v);
void grph_set_cost(Grph* grph, int u, int v, double cost);

/*******/

void dijkstra(Grph* grph, int start, int prev[], double D[]);
double dijkstra_min_cost(Grph* grph, int start, int end);
int dijkstra_shortest_road(Grph* grph, int start, int end, int road[]);


void reverse_array(int arr[], size_t size);

int main(void)
{
    Grph grph;
    int max_no = 10;
    grph_init(&grph, max_no, true);
    grph_set_cost(&grph, 0, 1, 6);
    grph_set_cost(&grph, 0, 2, 3);
    grph_set_cost(&grph, 1, 2, 2);

    int *road = malloc(sizeof(*road) * (max_no + 1));
    int size = dijkstra_shortest_road(&grph, 0, 1, road);
    printf("%lf\n", dijkstra_min_cost(&grph, 0, 1));

    for (int i = 0; i < size; i++)
        printf("%d%s", road[i], i == size - 1 ? "\n" : " -> ");

    return 0;
}



void grph_init(Grph* grph, int max_node_no, bool undirected)
{
    grph->size = max_node_no - 1;
    grph->undirected = undirected;
    grph->cost = malloc(sizeof(*(grph->cost)) * (grph->size * grph->size));
    for (int i = 0; i < grph->size; i++)
        for (int j = 0; j < grph->size; j++)
            if (i == j)
                grph->cost[i * grph->size + j] = 0;
            else
                grph->cost[i * grph->size + j] = INFINITY;
}

double grph_cost(Grph* grph, int u, int v)
{
    return grph->cost[u * grph->size + v];
}

void grph_set_cost(Grph* grph, int u, int v, double cost)
{
    grph->cost[u * grph->size + v] = cost;
    if (grph->undirected)
        grph->cost[v * grph->size + u] = cost;
}


void dijkstra(Grph* grph, int start, int prev[], double D[])
{
    bool *selected = malloc(sizeof(*selected) * grph->size);
    int selected_size = 0;

    for (int node = 0; node < grph->size; node++) {
        D[node] = grph_cost(grph, start, node);
        if (D[node] != INFINITY)
            prev[node] = start;

        selected[node] = false;
    }

    selected[start] = true;
    selected_size++;

    while (selected_size < grph->size)
    {
        #define NONE -1
        int w = NONE;
        for (int node = 0; node < grph->size; node++) {
            if (!selected[node]) {
                if (w == NONE)
                    w = node;
                if (D[node] < D[w])
                    w = node;
            }
        }
        #undef NONE

        selected[w] = true;
        selected_size++;

        for (int node = 0; node < grph->size; node++) {
            if (!selected[node]) {
                double new_cost = D[w] + grph_cost(grph, w, node);
                if (new_cost < D[node]) {
                    D[node] = new_cost;
                    prev[node] = w;
                }
            }
        }
    }

    free(selected);
}

double dijkstra_min_cost(Grph* grph, int start, int end)
{
    int *prev = malloc(sizeof(*prev) * grph->size);
    double *D = malloc(sizeof(*D) * grph->size);

    dijkstra(grph, start, prev, D);
    double min_cost = D[end];

    free(prev);
    free(D);

    return min_cost;
}

int dijkstra_shortest_road(Grph* grph, int start, int end, int road[])
{
    int *prev = malloc(sizeof(*prev) * grph->size);
    double *D = malloc(sizeof(*D) * grph->size);

    dijkstra(grph, start, prev, D);

    int size = 0;
    road[size++] = end;
    for (int i = 0; i < grph->size - 1; i++) {
        road[i + 1] = prev[road[i]];
        size++;

        if (road[i + 1] == start)
            break;
    }

    reverse_array(road, size);


    free(prev);
    free(D);

    return size;
}



void reverse_array(int arr[], size_t size)
{
    for (size_t i = 0; i < size / 2; i++) {
        int tmp = arr[i];
        arr[i] = arr[size - i - 1];
        arr[size - i - 1] = tmp;
    }
}
