#include "process_gen.h"
#include "../../include/queue.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pcb_struct generate_pcb(int last_pid)
{
    pcb_struct new_pcb;
    new_pcb.pid = last_pid + 1;

    return new_pcb;
}