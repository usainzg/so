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

int cpu_id = 0;

double max_time = 0.0;
double min_time = 9999999999.0;

int finished_threads = 0;

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
    int nice, q;
    int n = 0;

    struct timeval ended_at;
    double t_lived;

    Task_list_cpu tlc;
    Task_cpu tcpu;

    Task t;
    Task_cpu *tlc_2;
    Context ctxt;

    for (int i = 0; i < CPUS; i++)
    {
        sem_down_t(&cpu_sem);
        tlc = sched_cpu(&system_cpus);
        sem_up_t(&cpu_sem);

        while (tlc.task_arr_size > 0)
        {
            tcpu = tlc.task_arr[tlc.task_arr_size - 1];

            if (tcpu.state != STOPPED_TASK && tcpu.task.life > 0)
            {
                sem_down_t(&q_sem);
                priority_q_insert(tcpu.task, tcpu.ctxt, tcpu.task.priority + tcpu.quantum);
                sem_up_t(&q_sem);
            }
            else
            {
                pgb_delete(&tcpu.task.mm.pgb);

                gettimeofday(&ended_at, 0);
                t_lived = (ended_at.tv_sec - tcpu.task.start_time.tv_sec) + (ended_at.tv_usec - tcpu.task.start_time.tv_usec) / 1e6;
                if (min_time > t_lived)
                    min_time = t_lived;
                if (max_time < t_lived)
                    max_time = t_lived;

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
        if (cpu_slot_available(system_cpus + cpu_id))
        {
            n = 0;
            sem_down_t(&q_sem);
            t = priority_q_delete_first(&ctxt, &nice);
            sem_up_t(&q_sem);

            q = nice * 1000;
            if (q > t.life)
                q = t.life;

            tlc_2 = malloc(sizeof(Task_cpu));
            tlc_2->task = t;
            tlc_2->ctxt = ctxt;
            tlc_2->quantum = q;
            tlc_2->state = WORKING_TASK;

            sem_down_t(&cpu_sem);
            cpu_insert(system_cpus + cpu_id, tlc_2);
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
            scheduler();
        }
    }
}