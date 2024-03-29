#ifndef CPU_H
#define CPU_H

#include "insts.h"
#include "structs.h"

#include <stdlib.h>
#include <stdio.h>

#define WORKING_TASK 0
#define STOPPED_TASK 1

int CPUS = 4;
int CORES = 4;
int THREADS = 4;

Cpu *system_cpus;

/**
 * Inicializar las cpus del sistema.
 **/
void make_cpus()
{
    int i, j, k;
    system_cpus = malloc(sizeof(Cpu) * CPUS);

    for (i = 0; i < CPUS; i++)
    {
        system_cpus[i].n_cores_per_cpu = malloc(sizeof(Core) * CORES);
        system_cpus[i].n_threads_per_core = 0;
        for (j = 0; j < CORES; j++)
        {
            system_cpus[i].n_cores_per_cpu[j] = malloc(sizeof(Task_cpu) * THREADS);
            for (k = 0; k < THREADS; k++)
            {
                system_cpus[i].n_cores_per_cpu[j][k] = 0;
            }
        }
    }
    printf("[CPU] => init %d CPUs, %d cores, %d threads. \n", CPUS, CORES, THREADS);
}

/**
 * Cada paso (step) del reloj... todos los procesos
 * que esten en la cpu, no hayan agotado el tiempo y 
 * sigan trabajando (WORKING_TASK) => tienen que ejecutar el ciclo.
 * Esta funcion resta uno al quantum de estos procesos, y llama a execute(t)
 * para ejecutar la siguiente linea de codigo.
 **/
void clock_phase_cpu()
{
    int i, j, k;
    Task_cpu *t;

    for (i = 0; i < CPUS; i++)
        for (j = 0; j < CORES; j++)
            for (k = 0; k < THREADS; k++)
            {
                t = system_cpus[i].n_cores_per_cpu[j][k];
                
                if (t != 0 && t->state == WORKING_TASK)
                {
                    t->quantum--;
                    execute(t);

                    // No le queda tiempo... parar.
                    if (t->task.life < 1) t->state = STOPPED_TASK;
                    
                    system_cpus[i].n_cores_per_cpu[j][k] = t;
                }
            }
}

/**
 * Introduce el proceso (t) en la cpu indicada (cpu).
 **/
void insert_task_cpu(Task_cpu *t, Cpu *cpu)
{
    int i, j, k;
    
    for (i = 0; i < CPUS; i++)
        for (j = 0; j < CORES; j++)
            for (k = 0; k < THREADS; k++)
            {
                if (system_cpus[i].n_cores_per_cpu[j][k] == 0)
                {
                    system_cpus[i].n_threads_per_core += 1;
                    system_cpus[i].n_cores_per_cpu[j][k] = t;
                    return;
                }
            }
}

/**
 * Funcion que busca en la cpu indicada los procesos
 * que ya han finalizado, los introduce en la lista a devolver
 * y libera los threads correspondientes.
 **/
Task_list_cpu sched_cpu(Cpu *cpu)
{
    int i, j;
    Task_list_cpu list_cpu;
    Task_cpu *t;
    
    list_cpu.task_arr_size = 0;
    list_cpu.task_arr = malloc(sizeof(Task_cpu) * THREADS * CORES);

    for (i = 0; i < CORES; i++)
        for (j = 0; j < THREADS; j++)
        {
            t = cpu->n_cores_per_cpu[i][j];

            if (t != 0 && (t->quantum < 1 || t->state == STOPPED_TASK))
            {
                list_cpu.task_arr[list_cpu.task_arr_size] = (*t);
                list_cpu.task_arr_size += 1;

                cpu->n_cores_per_cpu[i][j] = 0;
                if (cpu->n_threads_per_core-1 < 0)
                {
                    cpu->n_threads_per_core = 0;
                } else
                {
                    cpu->n_threads_per_core -= 1;
                }
            }
        }
    return list_cpu;
}

/**
 * Para comprobar si una cpu tiene hueco.
 **/
int is_full_cpu(Cpu *cpu)
{
    return (cpu->n_threads_per_core < CORES * THREADS) ? 1 : 0;
}

#endif