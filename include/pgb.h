#ifndef PGB_H
#define PGB_H

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

PGB generate_pgb();
void check_pgb(PGB *pgb);
void insert_pgb(PGB *pgb, Pages page);
int get_pgb(PGB *pgb, int virtual_d);
void remove_pgb(PGB *pgb);
void free_pgb(int physical_d);

#endif
