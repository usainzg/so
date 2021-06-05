#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

int Q_MAX = 25;

Queue q;

void make_queue()
{
    q.task = malloc(sizeof(Task) * Q_MAX);
    q.head = 0;
    q.tail = -1;
    q.len = 0;
    q.max_len = Q_MAX;
    printf("[QUE] => init de queue con %d max\n", Q_MAX);
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