#include <stdio.h>

/*
时间限制：1秒

空间限制：32768K
小易去附近的商店买苹果，奸诈的商贩使用了捆绑交易，只提供6个每袋和8个每袋的包装(包装不可拆分)。 可是小易现在只想购买恰好n个苹果，小易想购买尽量少的袋数方便携带。如果不能购买恰好n个苹果，小易将不会购买。
输入描述:

输入一个整数n，表示小易想购买n(1 ≤ n ≤ 100)个苹果



输出描述:

输出一个整数表示最少需要购买的袋数，如果不能买恰好n个苹果则输出-1


输入例子1:

20


输出例子1:

3

*/

int cal(int N) {
    if (N < 6) {
        return -1;
    } else if (N == 6 || N == 8) {
        return 1;
    } else {
        int r1 = cal(N - 6);
        int r2 = cal(N - 8);

        if (r1 == -1 && r2 == -1) {
            return -1;
        } else if (r1 == -1 || r2 == -1) {
            return (r1 != -1 ? r1 : r2) + 1;
        } else {
            return (r1 < r2 ? r1 : r2) + 1;
        }
    }
}

int main(void)
{
    int N;
    scanf("%d", &N);
    printf("%d\n", cal(N));
    return 0;
}

