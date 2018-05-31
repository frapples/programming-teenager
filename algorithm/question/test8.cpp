#include <stdio.h>
#include <stdlib.h>
#include <list>
using namespace std;

typedef list<int>::iterator int_iter;

int abs(int a) {
    return a > 0 ? a : -a;
}

int int_max(int a, int b) {
    return a > b ? a : b;
}

int main(void)
{
    int n;
    scanf("%d", &n);

    list<int> remain;
    for (int i = 0; i < n; i++) {
        int num;
        scanf("%d", &num);
        remain.push_back(num);
    }

    list<int> select;

    if (remain.size() <= 1) {
        printf("0\n");
        return 0;
    }

    int_iter max_i = remain.begin();
    int_iter max_j = remain.begin();
    for (int_iter it = remain.begin(); it != remain.end(); ++it) {
        for (int_iter jt = remain.begin(); jt != remain.end(); ++jt) {
            if (abs(*it - *jt) > abs(*max_j - *max_i)) {
                max_i = it;
                max_j = jt;
            }
        }
    }
    select.push_back(*max_i);
    select.push_back(*max_j);
    remain.erase(max_i);
    remain.erase(max_j);

    while (remain.size() > 0) {
        int max = 0;
        int_iter max_it = remain.begin();
        bool is_start = true;
        for (int_iter it = remain.begin(); it != remain.end(); ++it) {
            int_iter begin = remain.begin();
            int_iter end = remain.end();
            --end;

            if (abs(*it - *begin) > max) {
                max_it = it;
                is_start = true;
            }
            if (abs(*it - *end) > max) {
                max_it = it;
                is_start = false;
            }
        }

        if (is_start) {
            select.push_front(*max_it);
        } else {
            select.push_back(*max_it);
        }
        remain.erase(max_it);
    }

    int count = 0;
    for (int_iter it = select.begin(); it != select.end(); ++it) {
        if (it != select.begin()) {
            int_iter prev_it = it;
            --prev_it;
            count += abs(*it - *prev_it);
        }
    }

    printf("%d\n", count);
    return 0;
}
