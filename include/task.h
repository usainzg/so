#ifndef TASK_H
#define TASK_H

#include <sys/time.h>
#include "mem.h"

#define WORKING_TASK 0
#define STOPPED_TASK 1

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
    int cmp;
} Context;

#endif