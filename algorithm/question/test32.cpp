
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int n;
    scanf("%d", &n);
    
    int* a = new int[n];
    int* b = new int[n];
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &a[i], &b[i]);
    }

    return 0;
}
