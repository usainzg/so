#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "timer.h"
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