#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rev(int n);

int main(void)
{
    int x, y;
    scanf("%d%d", &x, &y);

    printf("%d\n", rev(rev(x) + rev(y)));
    return 0;
}

int rev(int n) {
    char s[100];
    sprintf(s, "%d", n);
    int len = strlen(s);

    for (int i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }


    return strtol(s, NULL, 10);
}
