#ifndef TIMER_H
#define TIMER_H

#include "../include/helpers.h"

extern sem_t timer_sem;

int t;

void *timer_worker()
{
    t = 0;
    while (1)
    {
        sem_down_t(&timer_sem);
        t += 1;
    }
}

#endif
