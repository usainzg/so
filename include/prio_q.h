#ifndef PRIOQ_H
#define PRIOQ_H

#include "task.h"

struct Node
{
    Context context;
    Task task;
    struct Node *next_node;
    int order_info;
};

void priority_q_insert(Task t, Context ctxt, int info);
Task priority_q_delete_first(Context *ctxt, int *nice);
int priority_q_is_empty();
int priority_q_len();

void priority_q_normalize();
void priority_q_sort();

void priority_q_print();

#endif