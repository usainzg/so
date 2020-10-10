#include "process_gen.h"

pcb_struct generate_pcb(unsigned int last_pid)
{
    pcb_struct new_pcb;
    new_pcb.pid = last_pid + 1;

    return new_pcb;
}

void process_generator()
{
    
}