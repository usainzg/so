#include <stdio.h>
#include <semaphore.h>
#include "sched.h"
#include "../../include/helpers.h"

#define QUANTUM_COUNT 2

void *sched_worker()
{
    extern sem_t sched_sem;
    int quantum_count = QUANTUM_COUNT;
    
    while(1) 
    {
        sem_wait(&sched_sem);
        
        print_info("scheduler alertado desde el timer!", 1);
        
        quantum_count -= 1;

        if (quantum_count == 0) {
            print_info("quantum acabado", 0);
            quantum_count = 2;
            print_info("meter otro proceso a ejecutar!", 1);
        }
        
    }
}

void dispatcher() 
{

}

void scheduler()
{

}