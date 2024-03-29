#ifndef RAM_H
#define RAM_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "pgb.h"
#include "helpers.h"
#include "structs.h"

int VERBOSE_MEM = 0;
int RAM_SIZE = 16;
int PAGESIZE = 256;

RAM ram_mem;
Kernel_data kernel_data;

void init_ram()
{
    ram_mem.size = 2 << RAM_SIZE;
    kernel_data.pageAmount = ram_mem.size / PAGESIZE;
    kernel_data.reserved = kernel_data.pageAmount / 8;
    if (kernel_data.pageAmount % 8 > 0) kernel_data.reserved++;
    
    ram_mem.data = malloc(sizeof(Word) * (ram_mem.size + kernel_data.reserved));
    memset(ram_mem.data, 0x00, ram_mem.size + kernel_data.reserved);

    printf("[RAM] => init RAM %d posiciones (%d páginas)\n", ram_mem.size, kernel_data.pageAmount);
}

/**
 * Devuelve la posicion del bit mas alto con valor 0. 
 * Se usa para no ocupar toda una casilla con la tabla de paginas,
 * solo dedicamos un bit.
 **/
int check(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            if (!byte)
            {
                return j;
            }
        }
    return -1;
}

void print(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *) ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    puts("");
}

/**
 * Busca en la tabla de paginas una pagina libre,
 * devuelve la direccion a la primera posicion de la tabla.
 * Modifica la tabla de paginas ocupando esa pagina.
 **/
int alloc()
{
    int cB = -1;
    int dF = -1;

    sem_down_t(&pageT_sem);
    for (int i = 0; i < kernel_data.reserved && dF == -1; i++)
    {
        cB = check(sizeof(uint8_t), &ram_mem.data[ram_mem.size + i]);
        if (cB > -1)
        {
            dF = 8 * i + cB;
            uint8_t suma = 0x01 << cB;
            ram_mem.data[ram_mem.size + i] = ram_mem.data[ram_mem.size + i] | suma;
        }
    }

    sem_up_t(&pageT_sem);

    if (VERBOSE_MEM)
    {
        printf("[RAM] (Alloc)\n");
        for(int i = 0; i < kernel_data.reserved; i++)
        {
            print(sizeof(uint8_t), &ram_mem.data[ram_mem.size + i]);
        }
        printf("\n");
    }

    return (unsigned int) dF * PAGESIZE;
}

/**
 * Busca la pagina que comienza por dF y la cambia a disponible.
 **/
void free_pgb(int dF)
{
    int df = dF / PAGESIZE;
    int cB = df % 8;
    int i = (df - cB) / 8;

    sem_down_t(&pageT_sem);
    uint8_t slot = ram_mem.data[ram_mem.size + i];
    uint8_t resta = 0x01 << cB;
    if (slot != 0)
    {
        ram_mem.data[ram_mem.size + i] = slot ^ resta;
    }
    sem_up_t(&pageT_sem);

    if (VERBOSE_MEM)
    {
        printf("[RAM] (Free)\n");
        for(int i = 0; i < kernel_data.reserved; i++)
        {
            print(sizeof(uint8_t), &ram_mem.data[ram_mem.size + i]);
        }
        printf("\n");
    }
}

/**
 * Accede a la direccion indicada y escribe el valor (val) indicado.
 **/
void set(int addr, uint8_t val, PGB *pgb)
{
    sem_down_t(&ram_sem);
    ram_mem.data[get_pgb(pgb, addr)] = val;
    sem_up_t(&ram_sem);
}

/**
 * Accede a la direccion indicada y devuelve su contenido.
 **/
uint8_t get(int addr, PGB *pgb)
{
    uint8_t ret;
    sem_down_t(&ram_sem);
    ret = ram_mem.data[get_pgb(pgb, addr)];
    sem_up_t(&ram_sem);
    return ret;
}

/**
 * Accede a la direccion indicada y a las tres direcciones siguientes y escribe
 * el valor indicado.
 **/
void set_word(int addr, int val, PGB *pgb)
{
    int value = val;
    int v = val;

    for (int i = 3; i >= 0; i--)
    {
        v = (value >> 8 * i);
        value = value - (v << 8 * i);
        ram_mem.data[get_pgb(pgb, addr + (3 - i))] = v;
    }
}

/**
 * Accede a la direccion indicada y a las tres direcciones siguientes y
 * devuelve el contenido.
 **/
unsigned int get_word(int addr, PGB *pgb)
{
    unsigned int word = 0;
    for (int i = 0; i < 4; i++)
    {
        word <<= 8;
        word += get(addr + i, pgb);
    }
    return word;
}

#endif
