#include <stdio.h>
#include <semaphore.h>
#include "sched.h"
#include "../../include/helpers.h"
#include "../../include/queue.h"

#define QUANTUM_COUNT 2

void scheduler(Queue *q, int quantum_count);
void dispatcher();

pcb_struct exec_p;

void *sched_worker(void *queue)
{
    extern sem_t sched_sem;
    int quantum_count = QUANTUM_COUNT;
    Queue *q;
    q = (Queue *)queue;

    exec_p.pid = -1;

    while (1)
    {
        sem_wait(&sched_sem);

        print_info("scheduler alertado desde el timer!", 1);
        quantum_count -= 1;
        printf(" => ...q_restante: %d...\n", quantum_count);
        if (quantum_count == 0)
        {
            print_info("quantum acabado, llamando a scheduler...", 1);
            quantum_count = 2;
            scheduler(q, quantum_count);
        }
    }
}

void scheduler(Queue *q, int quantum_count)
{
    if (!q_is_empty(q))
    {
        pcb_struct last_p = exec_p;

        exec_p = q_delete_node(q);
        printf("SCHED: next process_pid exec: %d\n", exec_p.pid);

        // mutex
        if (last_p.pid != -1)
            q_insert(last_p, q);
    }

    dispatcher();
}

void dispatcher()
{
    print_info("DISP: mete proceso a ejecutar", 1);
}