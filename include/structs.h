#ifndef STRCTS_H
#define STRCTS_H

#include <sys/time.h>
#include <stdint.h>

/* ======== MEMORIA ======== */
/**
 * Para relacionar una direccion virtual con una fisica.
 **/
typedef struct
{
    int physical_d;
    int virtual_d;
} Pages;

/**
 * Count: cuantas paginas.
 * Size: tamanio de la lista reservada.
 * Data: lista.
 **/
typedef struct
{
    Pages *data;
    int count;
    int size;
} PGB;

/**
 * Memory Mapping de un proceso.
 * Code: direccion memoria virtual de code.
 * Data: direccion memoria virtual de data.
 **/
typedef struct
{
    int code;
    int data;
    PGB pgb;
} MM;


/* ======== CPU ======== */
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

/* ======== PRIORITY QUEUE ======== */
/**
 * Un nodo parte de una lista dinamica de nodos ordenados por
 * prioridades.
 * Context: contexto del proceso.
 * Task: proceso.
 * Next_node: siguiente nodo en la lista.
 * Order_info: variable que se usa para ordenar la lista.
 **/
struct Node
{
    Context context;
    Task task;
    struct Node *next_node;
    int order_info;
};

/* ======== QUEUE ======== */
typedef struct
{
    int head, tail;
    int len, max_len;
    Task *task;
} Queue;

/* ======== RAM ======== */
/**
 * PageAmount: cuantas paginas tiene la memoria.
 * Reserved: cuanta memoria reservada para tabla de paginas.
 **/
typedef struct
{
    int pageAmount;
    int reserved;
} Kernel_data;

typedef uint8_t Word;
typedef struct
{
    Word *data;
    int size;
} RAM;

#endif