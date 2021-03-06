#include <stdio.h>
#include <stdlib.h>
#include "prio_q.h"

struct Node *head, *cur;

void priority_q_insert(Task t, Context ctxt, int ord_info)
{
    struct Node *n = (struct Node*) malloc(sizeof(struct Node));
    n->task = t;
    n->order_info = ord_info;
    n->context = ctxt;

    n->next_node = head;
    head = n;

    priority_q_sort();
}

Task priority_q_delete_first(Context *ctxt, int *nice)
{
    struct Node *tmp = head;
    head = head->next_node;
    (*nice) = tmp->order_info;
    (*ctxt) = tmp->context;
    return tmp->task;
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
    struct Node *n;
    int i, tmp, len;

    n = head;
    len = priority_q_len();

    for (i = 0; i < len-1; i++) 
    {
        n = n->next_node;
    }

    if (n->order_info < 0) 
    {
        tmp = -(n->order_info + (n->order_info/2));
    }
    
    n = head;
    for (i = 0; i < len; i++)
    {
        n->order_info += tmp;
        n = n->next_node;
    }
}

void priority_q_sort()
{
    struct Node *next, *cur;
    Task tmpT;
    Context tmpCtxt;
    int i, j, i_m, ord_info, len;

    len = priority_q_len();
    i_m = len;

    for (i = 0; i < len-1; i++, i_m--)
    {
        cur = head;
        next = head->next_node;

        for (j = 1; j < i_m; j++)
        {
            if (cur->order_info < next->order_info)
            {
                // swap de order_info cur <-> next
                ord_info = cur->order_info;
                cur->order_info = next->order_info;
                next->order_info = ord_info;

                // swap de context cur <-> next
                tmpCtxt = cur->context;
                cur->context = next->context;
                next->context = tmpCtxt;

                // swap de task cur <-> next
                tmpT = cur->task;
                cur->task = next->task;
                next->task = tmpT;
            }
            cur = cur->next_node;
            next = next->next_node;
        }
    }
}

void priority_q_print()
{
    int i = 0, pid;
    struct Node *c = head;
    int len = priority_q_len();
    printf("[ ");
    while (i < len)
    {
        pid = c->task.pid;
        printf("%d\t", pid);
        c = c->next_node;
        i += 1;
    }
    printf(" ]\n");
}