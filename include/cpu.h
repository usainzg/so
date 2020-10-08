#ifndef CPU_H
#define CPU_H

/**
 * Estructura para guardar la informacion de los threads de un core.
 * - thread_id: id del thread dentro de un core.
 **/
typedef struct
{
    unsigned int thread_id;
} thread_struct;

/**
 * Estructura para guardar la informacion de los cores de una cpu.
 * - n_threads: numero de threads en el core.
 * - threads_arr: array de threads (thread_struct)
 **/
typedef struct 
{
    unsigned int n_threads;
    thread_struct *threads_arr;
} core_struct;

/**
 * Estructura para guardar la informacion de las cpus.
 * - n_cores: numero de cores en la cpu.
 * - cores_arr: array de los cores (core_struct).
 **/
typedef struct  
{
    unsigned int n_cores;
    core_struct *cores_arr;
} cpu_struct;

/**
 * Estructura para guardar la informacion de las cpu/s del sistema.
 * - n_cpus: numero de cpus en el sistema
 * - cpus_arr: array de las cpu (cpu_struct)
 **/
typedef struct
{
    unsigned int n_cpus;
    cpu_struct *cpus_arr;
} system_cpus_struct;

#endif