#include "prio_q.h"
#include <stdio.h>
#include <stdlib.h>

struct Node *head, *cur;

void priority_q_insert(Task t, Context ctxt, int info)
{

}

void priority_q_delete_first(Context *ctxt, int *nice)
{

}

int priority_q_is_empty()
{
    return (head == NULL) ? 1 : 0;
}

int priority_q_len()
{
    int len = 0;
    struct Node *c = head;
    while(c != NULL)
    {
        len += 1;
        c = c->next_node;
    }

    return len;
}

void priority_q_normalize()
{

}
void priority_q_sort()
{

}

void priority_q_print()
{
    int i, pid;
    struct Node *c = head;
    int len = priority_q_len();
    printf("[ ");
    i = 0;
    while (i < len)
    {
        pid = c->task.pid;
        printf("%d\t", pid);
        c = c->next_node;
        i += 1;
    }
    printf(" ]\n");
}