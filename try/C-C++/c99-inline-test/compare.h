#ifndef __COMPARE_H_
#define __COMPARE_H_

static int max_int(int a, int b);

static inline int max_int(int a, int b)
{
    return a > b ? a : b;
}

#endif
