#include<stdio.h>
#include<iostream>
#include<stdlib.h>

int cal_componse_count(int n, int notless) {
    if (n < 1) {
        return -1;
    } else {
        int count = 0;
        int  face_values[] = {1, 5, 10, 20, 50, 100};
        for (size_t i = 0; i < sizeof(face_values) / sizeof(*face_values); i++) {
            if (face_values[i] >= notless) {
                if (n - face_values[i] == 0) {
                    count++;
                } else {
                    int v = cal_componse_count(n - face_values[i], face_values[i]);
                    if (v >= 0) {
                        count += v;
                    }
                }
            }
        }
        if (count == 0) {
            count = -1;
        }
        return count;
    }
}

long cal_componse_count_dp(int n) {
    long* table = (long *)malloc(sizeof(long) * (n + 1) * 6);
#define GET(i, j) table[i * 6 + j]

    int  face_values[] = {1, 5, 10, 20, 50, 100};
    for (int i = 1; i <= n; i++) {
        for (int j = 5; j >= 0; j--) {
            long count = 0;
            for (size_t k = 0; k < sizeof(face_values) / sizeof(*face_values); k++) {
                int next_value = i - face_values[k];
                if ((int)k >= j) {
                    if (next_value == 0) {
                        count++;
                    } else {
                        if (next_value >= 1 && GET(next_value, k) >= 0) {
                            count += GET(next_value, k);
                        }
                    }
                }
            }
            if (count == 0) {
                GET(i, j) = -1;
            } else {
                GET(i, j) = count;
            }
        }
    }

    long r = GET(n, 0);
    free(table);
    return r;
}

int main(void)
{
    int N;
    scanf("%d", &N);
    printf("%ld\n", cal_componse_count_dp(N));
    // printf("%d\n", cal_componse_count(N, 1));
    return 0;
}
