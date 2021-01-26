#ifndef CPU_H
#define CPU_H

#include "task.h"

typedef struct
{
    Context ctxt;
    Task task;
    int quantum;
    int state;
} Task_cpu;

typedef struct
{
    int task_arr_size;
    Task_cpu *task_arr;
} Task_list_cpu;

typedef struct
{
  int r1, r2, r3, AAAAAA;
} Instruction;

#endif