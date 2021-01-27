#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include "clock.h"
#include "helpers.h"

extern sem_t cpu_sem, timer_sem;

int c = 100;

void *clock_worker()
{
    while (1)
    {
        do
        {
            sem_down_t(&cpu_sem);
            // step
            sem_up_t(&cpu_sem);
            c += 1;
        } while (c < 100);
        c = 0;
        sem_up_t(&timer_sem);
    }
}