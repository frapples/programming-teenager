#include <list>
#include <iostream>
using namespace std;


int walk(int n) {
    if (n < 1) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        int count = 1;
        for (int i = 1; i <= 6; i++) {
            count += walk(n - i);
        }
        return count;
    }
}

int main(void)
{
    int N;
    scanf("%d", &N);
    printf("%d", walk(N));
    return 0;
}

