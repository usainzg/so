#include <unistd.h>
#include "pcb.h"
#ifndef QUEUE_H
#define QUEUE_H

typedef struct
{
    struct pcb_queue_node *next, *prev;
    pcb_struct pcb;
} pcb_queue_node;

typedef struct 
{
    pcb_queue_node *head, *tail;
    unsigned int len;
} pcb_queue;

pcb_queue *new_pcb_list();
void display_queue();

#endif