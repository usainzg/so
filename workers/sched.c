#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include "sched.h"
#include "task.h"
#include "queue.h"
#include "helpers.h"
#include "cpu.h"
#include "prio_q.h"

pthread_t dispatch_worker;

extern sem_t q_sem, cpu_sem;
extern Cpu *system_cpus;
extern int CPUS, t;

void *dispatcher()
{
    Context ctxt;
    Task task;

    while(1)
    {
        if (!q_is_empty())
        {
            sem_down_t(&q_sem);
            task = q_delete_node();
            sem_up_t(&q_sem); // TODO: eliminar estos helpers???
            
            ctxt.pc = task.mm.code;
            
            sem_down_t(&ord_q_sem);
            priority_q_insert(task, ctxt, task.priority);
            sem_up_t(&ord_q_sem);
        }
    }
}

void scheduler()
{

}

void* sched_worker()
{
    pthread_create(&dispatch_worker, NULL, dispatcher, NULL);
    while (1)
    {
        if (t >= 1)
        {
            t = 0;
            scheduler();
        }
    }
}