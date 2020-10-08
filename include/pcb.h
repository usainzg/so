#ifndef PCB_H
#define PCB_H

typedef struct
{
    unsigned int pid;

    int prio;
    int state; // new, ready, waiting, executing, terminated, blocked, suspended...
    unsigned int life_time;
} pcb_struct;

#endif