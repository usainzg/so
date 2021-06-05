#ifndef SCHED_H
#define SCHED_H

#include <unistd.h>
#include <pthread.h>

#include "timer.h"

#include "../include/cpu.h"
#include "../include/queue.h"
#include "../include/prio_q.h"
#include "../include/helpers.h"

double max_time = 0.0;
double min_time = 9999999999.0;

int finished_threads = 0;

void* sched_worker();
void scheduler();
void* dispatcher();

pthread_t dispatch_worker;

extern sem_t q_sem, cpu_sem;
extern Cpu *system_cpus;
extern int CPUS, t;

int cpu_id = 0;

void *dispatcher()
{
    Context ctxt;
    Task task;

    while (1)
    {
        if (!q_is_empty())
        {
            sem_down_t(&q_sem);
            task = q_delete_node();
            sem_up_t(&q_sem);

            printf("[SCD] => (%ld) entra lista espera: | %d, %d | \n", task.pid, task.life, task.priority);
            ctxt.pc = task.mm.code;

            sem_down_t(&ord_q_sem);
            priority_q_insert(task, ctxt, task.priority);
            sem_up_t(&ord_q_sem);
        }
    }
}

void scheduler()
{
    int nice, q;
    int n = 0;

    struct timeval ended_at;
    double t_lived;

    Task_list_cpu tlc;
    Task_cpu tcpu;

    Task t;
    Task_cpu *tcpu2;
    Context ctxt;

    for (int i = 0; i < CPUS; i++)
    {
        sem_down_t(&cpu_sem);
        tlc = sched_cpu(&system_cpus[i]);
        sem_up_t(&cpu_sem);

        while (tlc.task_arr_size > 0)
        {
            tcpu = tlc.task_arr[tlc.task_arr_size - 1];

            if (tcpu.state != STOPPED_TASK && tcpu.task.life > 0)
            {
                printf("[SCD] => (%ld) a lista de espera, nice: %d\n", tcpu.task.pid, tcpu.quantum);
                sem_down_t(&q_sem);
                priority_q_insert(tcpu.task, tcpu.ctxt, tcpu.task.priority + tcpu.quantum);
                sem_up_t(&q_sem);
            }
            else
            {
                remove_pgb(&tcpu.task.mm.pgb);

                gettimeofday(&ended_at, 0);
                t_lived = (ended_at.tv_sec - tcpu.task.start_time.tv_sec) + (ended_at.tv_usec - tcpu.task.start_time.tv_usec) / 1e6;
                if (min_time > t_lived)
                    min_time = t_lived;
                if (max_time < t_lived)
                    max_time = t_lived;
                
                printf("[SCD] => (%ld) termina en %f segundos\n", tcpu.task.pid, t_lived);
                finished_threads += 1;
            }

            free(tlc.task_arr);
            tlc.task_arr_size -= 1;
        }
    }

    if (!priority_q_is_empty())
    {
        priority_q_normalize();
    }

    while (!priority_q_is_empty() && n < CPUS)
    {
        if (is_full_cpu(system_cpus + cpu_id))
        {
            n = 0;
            sem_down_t(&q_sem);
            t = priority_q_delete_first(&ctxt, &nice);
            sem_up_t(&q_sem);

            q = nice * 1000;
            if (q > t.life)
                q = t.life;

            tcpu2 = malloc(sizeof(Task_cpu));
            tcpu2->task = t;
            tcpu2->ctxt = ctxt;
            tcpu2->quantum = q;
            tcpu2->state = WORKING_TASK;

            printf("[SCD] => (%ld) va a entrar en CPU: %d, quantum: %d\n", tcpu2->task.pid, cpu_id, tcpu2->quantum);

            sem_down_t(&cpu_sem);
            insert_task_cpu(tcpu2, system_cpus + cpu_id);
            sem_up_t(&cpu_sem);
        }
        else
        {
            n += 1;
        }

        if (cpu_id + 1 < CPUS)
        {
            cpu_id += 1;
        }
        else
        {
            cpu_id = 0;
        }
    }
}

void *sched_worker()
{
    pthread_create(&dispatch_worker, NULL, dispatcher, NULL);
    while (1)
    {
        if (t >= 1)
        {
            t = 0;
            printf("[SCD] => llamando a scheduler() \n");
            scheduler();
        }
    }
}

#endif