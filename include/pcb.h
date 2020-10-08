#include <unistd.h>
#ifndef PCB_H
#define PCB_H

typedef struct 
{
    pid_t pid;

    int prio;
    int state; // new, ready, waiting, executing, terminated, blocked, suspended...
    unsigned int life_time;
} pcb_struct;

#endif