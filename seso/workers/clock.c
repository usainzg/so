#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include "clock.h"

void *clock_worker(void *cycles)
{
    int c = *(int*)cycles;
    extern sem_t clock_sem, timer_sem;

    while (1)
    {
        sleep(0.10);
        sem_wait(&timer_sem);
        
        sleep(0.0001 * c);
        
        sem_post(&clock_sem);
    }
}