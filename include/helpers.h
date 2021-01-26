#ifndef HELPERS_H
#define HELPERS_H

#include <semaphore.h>

/**
 * Definicion de todos los semaforos necesarios.
 **/
sem_t timer_sem, cpu_sem, ram_sem, pageT_sem, q_sem, oQ_sem;

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

#endif