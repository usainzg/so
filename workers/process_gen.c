#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/task.h"

extern sem_t queue_access;

char *FLD_PROGS = "programs/";

int GEN_MAX = 20;
int GEN_MIN = 10;
int SLP_MAX = 3;
int SLP_MIN = 1;

int created_threads = 0;
extern int PAGESIZE;

void generate_task(Task *t)
{
    t->pid = 1 + rand() % 2500;
    t->priority = 1 + rand() % 100;
    t->mm.pgb = generate_pgb();
    gettimeofday(&t->start_time, 0);
}

void *process_generator(void *queue)
{
    print_info("Entrando al thread creador de procesos...", 0);
    Queue *q;
    q = (Queue*) queue;
    pcb_struct new_pcb;
    int pid = 2;
    int gen_random_factor = 0;

    new_pcb = generate_pcb(pid);
    pid += 1;
    new_pcb.quantum = QUANTUM;
    q_insert(new_pcb, q);
    printf("new_pid: %d, life_time: %d, quantum: %d, q_len: %d\n", new_pcb.pid, new_pcb.finishing_time, new_pcb.quantum, q->len);

    while(1) {
        gen_random_factor = rand() % 10;
        sleep(1.0 + gen_random_factor); // entre 1-9s
        new_pcb = generate_pcb(pid);
        pid += 1;
        new_pcb.quantum = QUANTUM;
        q_insert(new_pcb, q);
        printf("new_pid: %d, life_time: %d, quantum: %d, q_len: %d\n", new_pcb.pid, new_pcb.finishing_time, new_pcb.quantum, q->len);
    }
}