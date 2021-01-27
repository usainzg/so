#include "process_gen.h"
#include "../../include/queue.h"
#include "../../include/helpers.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pcb_struct generate_pcb(int last_pid)
{
    pcb_struct new_pcb;
    last_pid += 1;
    new_pcb.pid = last_pid;
    new_pcb.finishing_time = rand() % 10; // max time to finish: 10 x QUANTUM
    return new_pcb;
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