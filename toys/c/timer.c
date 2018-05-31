#include"timer.h"


//貌似timer_t被用了。。。

//备注：似乎clock()是不计入usleep()的时间的

void timer_start(simpletimer_t *timer)
{
    timer->sumDiff = 0;
    timer->start = clock();
    timer->isStop = 0;
}

void timer_stop(simpletimer_t *timer)
{
    timer->sumDiff += clock() - timer->start;
    timer->isStop = 1;
}

void timer_continue(simpletimer_t *timer)
{
    timer->start = clock();
    timer->isStop = 0;
}


clock_t timer_get_elapsed(simpletimer_t *timer)
{
    clock_t t = timer->sumDiff;
    if(!timer->isStop)
        t += clock() - timer->start;

    return t * 1000 / CLOCKS_PER_SEC;
}

void my_sleep(unsigned long ms)
{
    simpletimer_t timer;
    timer_start(&timer);
    while(timer_get_elapsed(&timer) < ms)
        ;
}
