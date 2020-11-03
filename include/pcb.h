#ifndef PCB_H
#define PCB_H

typedef struct
{
    int pid;
    enum Status { idle, ready, running, preempted, waiting } status;

    int quantum;
    int finishing_time;
    int curr_cpu_time_run;
    int curr_waiting_time;
    int curr_cpu_ramaining_burst_time;
    int total_cpu_remaining_time;
} pcb_struct;

#endif