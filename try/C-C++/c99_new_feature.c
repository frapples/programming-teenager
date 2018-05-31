#include <stdio.h>

int main(void)
{
    /* compound literal 值得一提的是，这玩意具有左值语义 */
    int *p = (int [2]) {1, 2};
    printf("%d %d\n", *p, *(p + 1));


    struct node
    {
        int a;
        int b;
    };
    struct node *st = (struct node[2]){{1, 2}, {3, 4}};
    printf("%d\n", st[1].b);

    return 0;
}
