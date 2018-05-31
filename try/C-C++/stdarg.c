#include<stdarg.h>
#include<stdio.h>

const char* printf_(const char* fmt, ...);

int main(void)
{
    puts(printf_("%s%d%g", "fdjkl", 100, 10.3));
    return 0;
}


/* 其实拿vsprintf写更简单点 */
const char* printf_(const char* fmt, ...)
{
    static char res[2048];
    va_list ap;
    va_start(ap, fmt);

    char* p_res = res;
    for (const char* p_fmt = fmt; *p_fmt != '\0'; p_fmt++) {
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
