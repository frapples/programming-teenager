#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char *dirname(char *dest, const char *src);

int main(int argc, char *argv[])
{
    char dir[FILENAME_MAX];
    dirname(dir, argv[0]);
    puts(dir);
    return 0;
}

char *dirname(char *dest, const char *src)
{
    int i;
    for(i = strlen(strcpy(dest, src)); i >= 0; i--)
        if(dest[i] == '/' || dest[i] == '\\') {
            dest[i] = '\0';
            break;
        }

    return dest;
}
