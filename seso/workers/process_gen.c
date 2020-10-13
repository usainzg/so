#include "process_gen.h"
#include "../../include/queue.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pcb_struct generate_pcb(int last_pid)
{
    pcb_struct new_pcb;
    new_pcb.pid = last_pid + 1;

    return new_pcb;
}

void *process_generator(void *queue)
{
    printf("Entrando al thread creador de procesos...\n");
    Queue *q;
    q = (Queue*) queue;
    printf("q len_init: %d\n", q->len);
    pcb_struct new_pcb;
    int pid = 2;
    while(1) {
        sleep(1.0);
        new_pcb = generate_pcb(pid);
        pid += 1;
        q_insert(new_pcb, q);
        printf("New: %d, Peek: %d, Length: %d\n", new_pcb.pid, q_peek(q).pid, q->len);
    }
}