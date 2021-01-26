#ifndef QUEUE_H
#define QUEUE_H

#include "task.h"

typedef struct
{
    int head, tail;
    int len, max_len;
    Task *task;
} Queue;

void make_queue();
int q_size();
int q_is_empty();
int q_is_full();
Task q_peek();
void q_insert(Task t);
Task q_delete_node();

#endif