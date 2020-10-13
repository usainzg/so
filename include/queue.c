#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *make_queue()
{
    Queue *queue = malloc(sizeof(Queue));
    if (!queue)
        return NULL;
    queue->head = NULL;
    queue->len = 0;
    return queue;
}

int q_is_empty(Queue *q)
{
    return (q->head == NULL) ? 1 : 0;
}

void q_insert(pcb_struct pcb, Queue *q)
{
    Node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    if (temp == NULL)
    {
        printf("Queue is not Allocated\n");
        return;
    }
    temp->pcb = pcb;
    temp->next = NULL;
    if (q->head == NULL)
    {
        q->head = temp;
    }
    else
    {
        q->tail->next = temp;
    }
    q->tail = temp;
    q->len++;
}

pcb_struct q_delete_node(Queue *q)
{
    Node *temp;
    pcb_struct pcb;
    if (q_is_empty(q))
    {
        printf("Queue is Empty\n");
        exit(1);
    }
    temp = q->head;
    pcb = temp->pcb;
    q->head = q->head->next;
    free(temp);
    q->len--;
    return pcb;
}

pcb_struct q_peek(Queue *q)
{
    if (q_is_empty(q))
    {
        printf("Queue is Empty\n");
        exit(1);
    }
    return q->head->pcb;
}

void q_destroy(Queue *queue)
{
    Node *current = queue->head;
    Node *next = current;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}