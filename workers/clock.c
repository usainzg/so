#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include "clock.h"
#include "../include/helpers.h"
#include "../include/cpu.h"

extern sem_t cpu_sem, timer_sem;

int c = 100;

void *clock_worker()
{
    while (1)
    {
        do
        {
            sem_down_t(&cpu_sem);
            clock_phase_cpu();
            sem_up_t(&cpu_sem);
            c += 1;
        } while (c < 100);
        sem_up_t(&timer_sem);
        c = 0;
    }
}