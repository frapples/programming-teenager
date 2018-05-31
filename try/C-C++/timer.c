#ifndef __TIMER_H__
#define __TIMER_H__


#include<windows.h>

struct Timer {
    int _freq;
    LARGE_INTEGER _begin;
    LARGE_INTEGER _end;
};

void timer_init(Timer* timer);
void timer_start(Timer* timer);
double timer_cost(Timer* timer);

void timer_init(Timer* timer)
{
    LARGE_INTEGER tmp;
    QueryPerformanceFrequency(&tmp);
    timer->_freq = tmp.QuadPart;
}

void timer_start(Timer* timer)
{
    QueryPerformanceCounter(&(timer->_begin));
}

double timer_cost(Timer* timer)
{
    QueryPerformanceCounter(&(timer->_end));
    return ((timer->_end.QuadPart - timer->_begin.QuadPart)*1.0f / timer->_freq);
}


#endif
