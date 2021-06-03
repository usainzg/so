#ifndef SCHED_H
#define SCHED_H

double max_time = 0.0;
double min_time = 9999999999.0;

int finished_threads = 0;

void* sched_worker();
void scheduler();
void* dispatcher();

#endif