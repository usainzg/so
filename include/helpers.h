#ifndef HELPERS_H
#define HELPERS_H

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Definicion de todos los semaforos necesarios.
 **/
sem_t timer_sem, cpu_sem, ram_sem, pageT_sem, q_sem, ord_q_sem;

/**
 * Funcion para imprimir info por pantalla
 **/
void print_info(char *str, int arrow);

/**
 * Funcion que inicializa los semaforos
 **/
void init_sems();

/**
 * Funcion que levanta los semaforos, necesaria porque puede
 * haber problemas a la hora de levantarlos.
 **/
void sem_up_t(sem_t *opt);

/**
 * Funcion para bajar los semaforos, no es necesaria pero se
 * ha incluido por claridad.
 **/
void sem_down_t(sem_t *opt);

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

    if (sem_init(&ord_q_sem, 0, 1) == -1)
    {
        print_info("No se puede crear ord_q_sem", 1);
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

#endif