#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "timer.h"

void *timer_worker()
{
    extern sem_t clock_sem, timer_sem, sched_sem;

    while (1)
    {
        sleep(0.1);
        sem_wait(&clock_sem);

        printf("INFO: Signal del timer!\n");
        sem_post(&timer_sem);
        sem_post(&sched_sem);
    }
}