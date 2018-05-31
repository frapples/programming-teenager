#ifndef _TIMER_H_
#define _TIMER_H_
#include<time.h>

typedef struct stru_timer {
    clock_t sumDiff;
    clock_t start;
    char isStop;
}simpletimer_t;

#define timer_reset(timer) timer_start(timer)
   void timer_start(simpletimer_t *timer);
   void timer_stop(simpletimer_t *timer);
   void timer_continue(simpletimer_t *timer);
clock_t timer_get_elapsed(simpletimer_t *timer);

   void my_sleep(unsigned long ms);


#endif
