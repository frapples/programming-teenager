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
        Error(rprintf("����: û�ҵ�lua�����ļ���\n"
                      "�鷳���һ�£� %s\n", lua_start_file));
    }

    int is_ok;
    char command[MAX_PATH];
    sprintf(command, "starter\\lua\\wlua5.1.exe %s %s", lua_start_file, name);
    /* int is_ok = load_config(config_path, command, MAX_PATH); */
    /* if (!is_ok) { */
    /*     Error(rprintf("����: û�ҵ������ļ���\n" */
    /*                   "�鷳���һ�£� %s\n", config_path)); */
    /*     return 0; */
    /* } */

    is_ok = create_child_process(command, NULL);
    if (!is_ok) {
        Error(rprintf("������������(%s)ʧ�ܡ�\n"
                      "���ţ�ע�����·�������������exe�ļ��ġ�\n", command));
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
    MessageBox(NULL, TEXT(error_str), TEXT("����"), MB_OK);
}

/*
  ����һ�����̡�
  cmd_str �������̵������������·����Ҫע������Ա�����starter����ǰĿ¼��·����
  current_directory �����ӽ��̵ĵ�ǰĿ¼��NULL��̳и����̵����á�
*/
BOOL create_child_process(const char *cmd_str, const char* current_directory)
{
    STARTUPINFO start_info;
    PROCESS_INFORMATION process_info;
 
    ZeroMemory(&start_info, sizeof(start_info)); // ����Ϣ�ṹ���� ( �൱�� memset 0, ����Ч�ʸ��� )
    start_info.cb = sizeof(start_info); // ���ýṹ��С��cb����ӦΪ�ṹ�Ĵ�С
    ZeroMemory(&process_info, sizeof(process_info)); // ��������Ϣ�ṹ����
 
    // תΪ����·��
    char path[MAX_PATH];
    GetFullPathName(current_directory, MAX_PATH, path, NULL);

    // ��˵CreateProcess�ĵڶ�������������ֻ���ڴ�
    char* cmd_str_ = malloc(strlen(cmd_str) + 1);
    strcpy(cmd_str_, cmd_str);
    BOOL is_ok = CreateProcess(
                               NULL,           // ��������·��, ʹ��������
                               cmd_str_,        // ����������
                               NULL,           // ���̳н��̾��(Ĭ��)
                               NULL,           // ���̳��߳̾��(Ĭ��)
                               FALSE,          // ���̳о��(Ĭ��)
                               0,              // û�д�����־(Ĭ��)
                               NULL,           // ʹ��Ĭ�ϻ�������
                               current_directory == NULL? NULL : path, // ����ַ��������Ǵ��������ŵľ���·�������ΪNULLʹ�ø����̵�Ŀ¼
                               &start_info,    // STARTUPINFO �ṹ
                               &process_info );// PROCESS_INFORMATION ���������Ϣ

    free(cmd_str_);

    if (!is_ok)
        {
            // ����ʧ��
            /* printf( "Error: ����δ�ҵ� (%d).\n", GetLastError() ); */
            return FALSE;
        }

    // �ȴ��ӽ��̽���
    // ʹ�õ���ͨ�� PROCESS_INFORMATION �ṹ���ȡ�ӽ��̵ľ�� hProcess
    WaitForSingleObject(process_info.hProcess, INFINITE);
    // �رս��̾�����߳̾��
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

// û���������������
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

/* �ο����ϣ�
   ���·��ת��Ϊ����·����http://blog.csdn.net/nocky/article/details/6056717
   ��δ����ӽ��̣�http://www.lellansin.com/windows-api%E6%95%99%E7%A8%8B%EF%BC%88%E5%9B%9B%EF%BC%89-%E8%BF%9B%E7%A8%8B%E7%BC%96%E7%A8%8B.html
*/
