#ifndef CLOCK_H
#define CLOCK_H

#include "../include/helpers.h"
#include "../include/cpu.h"

extern void clockphase_cpu();

int TIMER_T = 100;

extern sem_t cpu_sem, timer_sem;

void *clock_worker()
{
    int c = 0;
    while (1)
    {
        do
        {
            sem_down_t(&cpu_sem);
            clock_phase_cpu();
            sem_up_t(&cpu_sem);
            c += 1;
        } while (c < TIMER_T);
        sem_up_t(&timer_sem);
        c = 0;
    }
}

#endif