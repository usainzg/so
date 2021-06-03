#ifndef P_GEN_H
#define P_GEN_H


#define QUANTUM 5

char *FLD_PROGS = "programs/";

int GEN_MAX = 20;
int GEN_MIN = 10;
int SLP_MAX = 3;
int SLP_MIN = 1;

int created_threads = 0;
extern int PAGESIZE;

void generate_task(Task *t);
void new_program(Task *t, const char *file);
void *process_generator_worker();
int n_programs();
char* obten_prog(int id);


#endif