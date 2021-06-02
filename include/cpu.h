#ifndef CPU_H
#define CPU_H

#include "task.h"

#define WORKING_TASK 0
#define STOPPED_TASK 1

/**
 * Informacion necesaria para la ejecucion del
 * proceso en la cpu.
 * Quantum: numero de ciclos que le quedan en la cpu.
 * State: estao de ejecucion (WORKING/STOPPED).
 **/
typedef struct
{
    Context ctxt;
    Task task;
    int quantum;
    int state;
} Task_cpu;

/**
 * Lista de procesos que han terminado.
 * Task_arr_size: tamanio de la lista.
 * Task_arr: lista de procesos.
 **/
typedef struct
{
    int task_arr_size;
    Task_cpu *task_arr;
} Task_list_cpu;

typedef Task_cpu *Thread;
typedef Thread *Core;

typedef struct
{
    int n_threads_per_core;
    Core *n_cores_per_cpu;
} Cpu;

/**
 * Estructura para almacenar la informacion de las
 * instrucciones.
 **/
typedef struct
{
    int r1, r2, r3, AAAAAA;
} Instruction;

void make_cpus();
void clock_phase_cpu();
void insert_task_cpu(Task_cpu *t, Cpu *cpu);
Task_list_cpu sched_cpu(Cpu *cpu);
int is_full_cpu(Cpu *cpu);

#endif