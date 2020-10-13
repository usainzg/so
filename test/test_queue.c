#include <stdio.h>
#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/cpu.h"

int main(int argc, char **argv)
{
    pcb_struct pcb1, pcb2, pcb3;
    pcb1.pid = 1;
    pcb2.pid = 2;
    pcb3.pid = 3;

    Queue *q = make_queue();

    q_insert(pcb1, q);
    q_insert(pcb2, q);
    q_insert(pcb3, q);

    Node *ptr = q->head;

    while(ptr != NULL) {
        printf("PID: %d\n", ptr->pcb.pid);
        ptr = ptr->next;
    }
    
    printf("Eliminamos el primero que ha entrado!\n");
    
    q_delete_node(q);
    ptr = q->head;
    while(ptr != NULL) {
        printf("PID: %d\n", ptr->pcb.pid);
        ptr = ptr->next;
    }

    printf("Peek PID: %d\n", q_peek(q).pid);
    
    q_delete_node(q);
    q_delete_node(q);
    q_delete_node(q);
}