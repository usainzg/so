#ifndef P_GEN_H
#define P_GEN_H


#define QUANTUM 5

pcb_struct generate_pcb(int last_pid);
void *process_generator(void *queue);

#endif