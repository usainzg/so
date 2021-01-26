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

#endif