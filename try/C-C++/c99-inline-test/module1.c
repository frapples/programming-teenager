#include <stdio.h>
#include <string.h>
#include "compare.h"



#define FS_MAX_FILE_FD 1024
struct {
    char* file;
    char drive_letter;
} g_fs_files[FS_MAX_FILE_FD] = {{.file = NULL}};

int main()
{
    max_int(10, 20);

    const char* s = "test";
    char str[3];
    strncpy(str, s, sizeof(str));
    printf("%c%c%c\n", str[0], str[1], str[2]);
    return 0;
}
