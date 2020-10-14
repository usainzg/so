#include <stdio.h>
#include <semaphore.h>
#include "sched.h"

void *sched_worker()
{
    extern sem_t sched_sem;
    
    while(1) 
    {
        sem_wait(&sched_sem);
        printf(" => INFO: scheduler alertado desde el timer!\n");
    }
}