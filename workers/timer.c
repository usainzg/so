#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "timer.h"
#include "helpers.h"

extern sem_t timer_sem;

int t;

void *timer_worker()
{
    while (1)
    {
        sem_down_t(&timer_sem);
        t += 1;
    }
}