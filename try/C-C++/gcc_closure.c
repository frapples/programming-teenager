#include<stdio.h>
#include<limits.h>

typedef int(*Func)(void);

Func get_f(void);
int main(void)
{
    printf("%d", get_f()());
    return 0;
}

Func get_f(void)
{
    int c = 10;
    int f(void)
    {
        return c;
    }
    return f;
}
