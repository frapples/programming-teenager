#include<stdlib.h>
#include<stdio.h>

#ifdef _WIN32
#include<conio.h>
#endif

int getch_noecho()
{
#ifdef linux
    system("stty raw");
    int ch = getchar();
    printf("\b \b");
    system("stty cooked");
    return ch;
#endif

#ifdef _WIN32
    return getch();
#endif
}

int main(void)
{
#ifdef linux
    printf("linux OS\n");
#endif

#ifdef unix
    printf("unix OS\n");
#endif

#ifdef _WIN32
    printf("win32 os\n");
#endif
    printf("test\n");
    int ch = getch_noecho();
    printf("%d", ch);

    return 0;
}
