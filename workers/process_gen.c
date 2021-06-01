#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/queue.h"
#include "../include/task.h"
#include "../include/pgb.h"
#include "../include/ram.h"

extern sem_t queue_access;

char *FLD_PROGS = "programs/";

int GEN_MAX = 20;
int GEN_MIN = 10;
int SLP_MAX = 3;
int SLP_MIN = 1;

int created_threads = 0;
extern int PAGESIZE;

void generate_task(Task *t)
{
    t->pid = 1 + rand() % 2500;
    t->priority = 1 + rand() % 100;
    t->mm.pgb = generate_pgb();
    gettimeofday(&t->start_time, 0);
}

void new_program(Task *t, const char *file)
{
    int lines, pages, aloc;

    FILE *f = fopen(file, "r");
    if (f != NULL)
    {
        fseek(f, 0, SEEK_END);
        int size = ftell(f);

        Word *rom = malloc(size);
        fseek(f, 0, SEEK_SET);
        fread(rom, size, 1, size); // leer la rom
        fclose(f);

        generate_task(t);
        
        // Calcular direcciones de .text y .data.
        // Tambien asignamos las direcciones.
        t->mm.code = (rom[5] << 2) + (rom[6] << 1) + rom[7];
        t->mm.data = (rom[13] << 2) + (rom[14] << 1) + rom[15];

        // Calcular numero de lineas (se usa luego) y de paginas.
        lines = size - 16;
        pages = (lines / (int) PAGESIZE) + ((lines % (int) PAGESIZE > 0) ? 1 : 0); // TODO: evitar el modulo???

        Pages pg;

        for (int i = 0; i < pages; i++)
        {
            pg.virtual_d = t->mm.code + (i * 265);
            aloc = alloc();
            if (aloc < 0)
            {
                remove_pgb(&(t->mm.pgb));
                t->pid = 0;
                return;
            }
            pg.physical_d = (unsigned int) aloc; // TODO: hace falta casting???
            insert_pgb(&(t->mm.pgb), pg);
        }

        for (int i = 0; i < lines; i++)
        {
            set(i, rom[i + 16], &(t->mm.pgb));
        }
        t->life = lines; // TODO: preguntar iñaki!
    }
    else
    {
        printf("=> Error: fichero erroneo! \n");
        exit(1);
    }
}

void *process_generator(void *queue)
{
    FILE *f = fopen()
}