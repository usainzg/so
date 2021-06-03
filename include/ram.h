#ifndef RAM_H
#define RAM_H

#include "pgb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

int RAM_SIZE = 16;
int PAGESIZE = 256;

/**
 * PageAmount: cuantas paginas tiene la memoria.
 * Reserved: cuanta memoria reservada para tabla de paginas.
 **/
typedef struct
{
    int pageAmount;
    int reserved;
} Kernel_data;

typedef uint8_t Word;
typedef struct
{
    Word *data;
    int size;
} RAM;


void init_ram();
int alloc();
int check(size_t const size, void const *const ptr);
void free_pgb(int physical_d);
void set(int addr, uint8_t val, PGB *pgb);
uint8_t get(int addr, PGB *pgb);
void set_word(int addr, int val, PGB *pgb);
unsigned int get_word(int addr, PGB *pgb);
void print(size_t const size, void const *const ptr);

#endif
