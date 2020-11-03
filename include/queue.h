#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

#define Q_MAX_LEN 20

typedef struct node Node;
typedef struct queue Queue;

struct node
{
    pcb_struct pcb;
    struct node *next;
};

struct queue
{
    Node *head, *tail;
    int len;
};

Queue *make_queue();
void q_insert(pcb_struct pcb, Queue *q);
pcb_struct q_delete_node(Queue *q);
pcb_struct q_peek(Queue *q);
void q_destroy(Queue *q);
int q_is_empty(Queue *q);

#endif