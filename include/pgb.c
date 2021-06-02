#include "pgb.h"
#include <stdio.h>
#include <stdlib.h>

PGB generate_pgb()
{
    PGB pgb;
    pgb.data = malloc(sizeof(Pages) * 2);
    pgb.size = 2;
    pgb.count = 0;
    return pgb;
}

void check_pgb(PGB *pgb)
{
    if (pgb->count == pgb->size)
    {
        Pages *tmp = malloc(sizeof(Pages) * (pgb->size << 1));
        for (int i = 0; i < pgb->size; i++)
        {
            tmp[i] = pgb->data[i];
        }

        free(pgb->data);
        pgb->data = tmp;
        pgb->size *= 2;
    }
}

void insert_pgb(PGB *pgb, Pages page)
{
    check_pgb(pgb);
    pgb->data[pgb->count] = page;
    pgb->count++;
}

int get_pgb(PGB *pgb, int virtual_d)
{
    int i = virtual_d >> 8;
    int dir_p = i << 8;

    for (i = 0; i < pgb->count; i++)
    {
        if (pgb->data[i].virtual_d == dir_p)
        {
            break;
        }
    }

    return pgb->data[i].physical_d + (virtual_d - dir_p);
}

void free_pgb(int physical_d);

void remove_pgb(PGB *pgb)
{
    for (int i = 0; i < pgb->count; i++)
    {
        Free(pgb->data[i].physical_d);
    }
}
