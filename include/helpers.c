#include "helpers.h"
#include <stdio.h>

void print_info(char *str, int arrow)
{
    if (arrow == 0)
    {
        printf("INFO: %s\n", str);   
    }
    else
    {
        printf(" => INFO: %s\n", str);
    }
}

void init_sems()
{
    int flag_error = 0;
    if (sem_init(&timer_sem, 0, 0) == -1)
    {
        print_info("No se puede crear timer_sem", 1);
        flag_error = 1;
    }

    if (sem_init(&cpu_sem, 0, 1) == -1)
    {
        print_info("No se puede crear cpu_sem", 1);
        flag_error = 1;
    }

    if (sem_init(&ram_sem, 0, 1) == -1)
    {
        print_info("No se puede crear ram_sem", 1);
        flag_error = 1;
    }

    if (sem_init(&pageT_sem, 0, 1) == -1)
    {
        print_info("No se puede crear oageT_sem", 1);
        flag_error = 1;
    }

    if (sem_init(&q_sem, 0, 1) == -1)
    {
        print_info("No se puede crear q_sem", 1);
        flag_error = 1;
    }

    if (sem_init(&oQ_sem, 0, 1) == -1)
    {
        print_info("No se puede crear oQ_sem", 1);
        flag_error = 1;
    }

    if (flag_error == 1) exit(-1);
}

void sem_up_t(sem_t *opt)
{
    if (sem_post(opt) == -1) print_info("Error: sem_post(opt)", 1);
}

void sem_down_t(sem_t *opt)
{
    sem_wait(opt);
}