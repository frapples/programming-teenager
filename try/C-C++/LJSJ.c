#include<stdarg.h>
#include<setjmp.h>
#include<stdio.h>
#include<assert.h>

#define debug_do(a) assert(((a), 1))

jmp_buf _try_jmpbuf;
    int _try_code;

#ifndef NDEBUG
    int _did_try = 0;
#define debug_print(a, b) if(!(a))fprintf(stderr, "在函数%s中: %s\n",__FUNCTION__, b)
#else
#define debug_print(a, b)
#endif

#define THROW(mes) \
{ \
    debug_print(_did_try, "可能产生异常的代码未置入try中！"); \
    assert(_did_try);\
    longjmp(_try_jmpbuf, mes); \
}

#define TRY \
    debug_do(_did_try = 1); \
    _try_code = setjmp(_try_jmpbuf); \
    if(_try_code == 0)

#define CATCH(mes) \
    else if(_try_code == mes)



enum {
       EXCEPTION_TEST, 
       EXCEPTION_FIND, 
};

void test(int a)
{
    THROW(EXCEPTION_TEST);
}

int main(void)
{
    TRY{
        test(1);
    }
    CATCH(EXCEPTION_TEST)
    {
        printf("error:1");
    }
    CATCH(EXCEPTION_FIND)
    {
        printf("error:2");
    }

    return 0;
}
