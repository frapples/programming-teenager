#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<string.h>
#include<stdarg.h>


BOOL create_child_process(const char *cmd_str, const char* current_directory);
BOOL load_config(const char* path, char* command, size_t size);
void Error(const char* error_str);
void basename(const char *path, char *name);
void basename_noext(const char *path, char *name);
BOOL file_exists(const char *path);

const char* rprintf(const char* fmt, ...);


int main(int argc, char *argv[])
{
    char name[FILENAME_MAX];
    basename_noext(argv[0], name);

    const char *lua_start_file = "starter\\start.lua";

    if (!file_exists(lua_start_file)) {
        Error(rprintf("错误: 没找到lua配置文件。\n"
                      "麻烦检查一下： %s\n", lua_start_file));
    }

    int is_ok;
    char command[MAX_PATH];
    sprintf(command, "starter\\lua\\wlua5.1.exe %s %s", lua_start_file, name);
    /* int is_ok = load_config(config_path, command, MAX_PATH); */
    /* if (!is_ok) { */
    /*     Error(rprintf("错误: 没找到配置文件。\n" */
    /*                   "麻烦检查一下： %s\n", config_path)); */
    /*     return 0; */
    /* } */

    is_ok = create_child_process(command, NULL);
    if (!is_ok) {
        Error(rprintf("错误：运行命令(%s)失败。\n"
                      "亲呐，注意相对路径是相对启动器exe文件的。\n", command));
    }
    return 0;
}


BOOL load_config(const char* path, char* command, size_t size)
{
    FILE* config_file = fopen(path, "r");
    if (config_file == NULL) {
        command[0] = '\0';
        return FALSE;
    }
    fgets(command, size, config_file);
    if (command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = '\0';
    return TRUE;
}

void Error(const char* error_str)
{
    MessageBox(NULL, TEXT(error_str), TEXT("错误"), MB_OK);
}

/*
  创建一个进程。
  cmd_str 启动进程的命令，如果是相对路径，要注意是相对本程序（starter）当前目录的路径。
  current_directory 设置子进程的当前目录。NULL则继承父进程的设置。
*/
BOOL create_child_process(const char *cmd_str, const char* current_directory)
{
    STARTUPINFO start_info;
    PROCESS_INFORMATION process_info;
 
    ZeroMemory(&start_info, sizeof(start_info)); // 将信息结构清零 ( 相当于 memset 0, 不过效率更高 )
    start_info.cb = sizeof(start_info); // 设置结构大小，cb属性应为结构的大小
    ZeroMemory(&process_info, sizeof(process_info)); // 将进程信息结构清零
 
    // 转为绝对路径
    char path[MAX_PATH];
    GetFullPathName(current_directory, MAX_PATH, path, NULL);

    // 据说CreateProcess的第二个参数不能是只读内存
    char* cmd_str_ = malloc(strlen(cmd_str) + 1);
    strcpy(cmd_str_, cmd_str);
    BOOL is_ok = CreateProcess(
                               NULL,           // 不传程序路径, 使用命令行
                               cmd_str_,        // 命令行命令
                               NULL,           // 不继承进程句柄(默认)
                               NULL,           // 不继承线程句柄(默认)
                               FALSE,          // 不继承句柄(默认)
                               0,              // 没有创建标志(默认)
                               NULL,           // 使用默认环境变量
                               current_directory == NULL? NULL : path, // 这个字符串必须是带驱动器号的绝对路径。如果为NULL使用父进程的目录
                               &start_info,    // STARTUPINFO 结构
                               &process_info );// PROCESS_INFORMATION 保存相关信息

    free(cmd_str_);

    if (!is_ok)
        {
            // 创建失败
            /* printf( "Error: 命令未找到 (%d).\n", GetLastError() ); */
            return FALSE;
        }

    // 等待子进程结束
    // 使用到了通过 PROCESS_INFORMATION 结构体获取子进程的句柄 hProcess
    WaitForSingleObject(process_info.hProcess, INFINITE);
    // 关闭进程句柄和线程句柄
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);
 
    return TRUE;
}

void basename(const char *path, char *name)
{
    int name_top = 0;
    for (int i = (int)strlen(path) - 1; i >= 0 && path[i] != '/' && path[i] != '\\'; i--) {
        if (path[i] != '\0' ) {
            name[name_top++] = path[i];
        }
    }

    int name_size = name_top;
    for (int i = 0; i < name_size / 2; i++) {
        char tmp = name[i];
        name[i] = name[name_size - i - 1];
        name[name_size - i - 1] = tmp;
    }
    name[name_size] = '\0';
}

void basename_noext(const char *path, char *name)
{
    basename(path, name);
    int i;
    for (i = strlen(name) - 1; i >= 0 && name[i] != '.'; i--) {
        name[i] = '\0';
    }

    if (i >= 0 && name[i] == '.')
        name[i] = '\0';
}

BOOL file_exists(const char *path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return FALSE;
    } else {
        fclose(fp);
        return TRUE;
    }
}

// 没做缓冲区溢出处理
const char* rprintf(const char* fmt, ...)
{
    static char res[2048];
    va_list ap;
    va_start(ap, fmt);

    char* p_res = res;
    const char* p_fmt;
    for (p_fmt = fmt; *p_fmt != '\0'; p_fmt++) {
        if (*p_fmt != '%') {
            *(p_res++) =  *p_fmt;
        } else {
            size_t size;
            int int_val;
            double double_val;
            char* str_val;
            p_fmt++;
            switch (*p_fmt) {
            case 'd':
                int_val = va_arg(ap, int);
                size = sprintf(p_res, "%d", int_val);
                p_res += size;
                break;
            case 'g':
                double_val = va_arg(ap, double);
                size = sprintf(p_res, "%g", double_val);
                p_res += size;
                break;
            case 's':
                str_val = va_arg(ap, char*);
                size = sprintf(p_res, "%s", str_val);
                p_res += size;
                break;
            case '%':
            default:
                *(p_res++) = *p_fmt;
                break;
            }
        }
    }

    *(p_res++) = '\0';
    va_end(ap);

    return res;
}

/* 参考资料：
   相对路径转换为绝对路径：http://blog.csdn.net/nocky/article/details/6056717
   如何创建子进程：http://www.lellansin.com/windows-api%E6%95%99%E7%A8%8B%EF%BC%88%E5%9B%9B%EF%BC%89-%E8%BF%9B%E7%A8%8B%E7%BC%96%E7%A8%8B.html
*/
