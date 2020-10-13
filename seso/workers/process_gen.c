#include "process_gen.h"
#include "../../include/queue.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pcb_struct generate_pcb(int last_pid)
{
    pcb_struct new_pcb;
    last_pid += 1;
    new_pcb.pid = last_pid;
    new_pcb.life_time = rand() % 10000;
    return new_pcb;
}

void *process_generator(void *queue)
{
    printf("INFO: Entrando al thread creador de procesos...\n");
    Queue *q;
    q = (Queue*) queue;
    pcb_struct new_pcb;
    int pid = 2;
    while(1) {
        sleep(1.0);
        new_pcb = generate_pcb(pid);
        pid += 1;
        q_insert(new_pcb, q);
        printf("new_pid: %d, life_time: %d, q_len: %d\n", new_pcb.pid, new_pcb.life_time, q->len);
    }
}