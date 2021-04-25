#ifndef TASK_H
#define TASK_H

#include <sys/time.h>
#include "mem.h"

typedef struct
{
    struct timeval start_time;
    long int pid;
    int priority;
    int life;
    MM mm;
} Task;

typedef struct
{
    int pc;
    int rg[16];
    int cc;
} Context;

#endif