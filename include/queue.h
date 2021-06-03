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

int Q_MAX = 25;

Queue q;

void make_queue()
{
    q.task = malloc(sizeof(Task) * Q_MAX);
    q.head = 0;
    q.tail = -1;
    q.len = 0;
    q.max_len = Q_MAX;
    print_info("Init de Q.", 1);
}

int q_size()
{
    return q.len;
}

int q_is_empty()
{
    return (q.len == 0) ? 1 : 0;
}

int q_is_full()
{
    return (q.len == q.max_len) ? 1 : 0;
}

Task q_peek()
{
    return q.task[q.head];
}

void q_insert(Task t)
{
    if (!q_is_full())
    {
        if (q.max_len == q.tail+1) q.tail = -1;
        q.tail += 1;
        q.task[q.tail] = t;
        q.len += 1;
    }
}
Task q_delete_node()
{
    Task t;
    t.pid = -1;
    if (q.len > 0)
    {
        t = q.task[q.head];
        q.head += 1;

        if (q.head == q.max_len) q.head = 0;

        q.len -= 1;
    }
    return t;
}

#endif