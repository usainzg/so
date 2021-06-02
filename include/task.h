#ifndef TASK_H
#define TASK_H

#include <sys/time.h>
#include "mem.h"

/**
 * Para los datos correspondientes al proceso
 * pid, prioridad, tiempo de vida, hora de inicio,
 * memory mapping.
 **/
typedef struct
{
    struct timeval start_time;
    long int pid;
    int priority;
    int life;
    MM mm;
} Task;

/**
 * Para almacenar el contexto de un proceso.
 * Pc: pc del programa.
 * Rg[16]: los 16 registros.
 * Cc: registro para comparaciones.
 **/
typedef struct
{
    int pc;
    int rg[16];
    int cc;
} Context;

#endif