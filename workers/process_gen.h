#ifndef P_GEN_H
#define P_GEN_H

#include <unistd.h>
#include <pthread.h>
#include <dirent.h>

#include "../include/ram.h"
#include "../include/pgb.h"
#include "../include/queue.h"
#include "../include/helpers.h"
#include "../include/structs.h"

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

extern sem_t q_sem;

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
        fread(rom, size, 1, f); // leer la rom
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

// Funcion auxiliar que cuenta los programas en la carpeta (FLD_PROGS)
int n_programs()
{
    int n = 0;
    DIR *dir = opendir(FLD_PROGS);
    struct dirent *dt;

    if (dir == NULL)
    {
        printf("=> Error: Directorio no existe!\n");
        exit(0);
    }

    while ((dt = readdir(dir)) != NULL)
    {
        if (strcmp(dt->d_name, ".") != 0 && strcmp(dt->d_name, "..") != 0)
        {
            n += 1;
        }
    }
    closedir(dir);
    return n;
}

// Funcion auxiliar para obtener el programa.
char* obten_prog(int id)
{
    DIR *dir = opendir(FLD_PROGS);
    struct dirent *dt;
    int id_aux = 0;
    char *result;
    
    if (dir == NULL)
    {
        printf("=> Error: Directorio no existe!\n");
        exit(0);
    }
    
    while ((dt = readdir(dir)) != NULL)
    {
        if (strcmp(dt->d_name, ".") != 0 && strcmp(dt->d_name, "..") != 0)
        {
            if (id_aux == id) break;
            else id_aux += 1;
        }
    }

    result = dt->d_name;
    closedir(dir);
    return result;
}

void *process_generator_worker()
{
    Task t;
    int n_progs = n_programs();
    int f;
    char *f_c;
    int aux = 0;

    while (1)
    {
        // Cuantos procesos a generar
        aux = GEN_MIN + rand() % (GEN_MAX - GEN_MIN);

        while (aux > 0)
        {
            sem_down_t(&q_sem);
            f = rand() % n_progs; // Al azar entre los que hay.
            f_c = malloc(sizeof(char) * (strlen(FLD_PROGS) + strlen(obten_prog(f)))); // Reservar espacio.

            strcpy(f_c, FLD_PROGS);
            strcat(f_c, obten_prog(f));
            
            new_program(&t, f_c);
            free(f_c);

            if (t.pid != 0 && !q_is_full())
            {
                q_insert(t);
                created_threads += 1;
            }
            else
            {
                sem_up_t(&q_sem);
                break;
            }

            sem_up_t(&q_sem);
            aux -= 1;
        }

        // Calcular tiempo para dormir (igual que para generar), y a dormir.
        aux = SLP_MIN + rand() % (SLP_MAX - SLP_MIN);
        sleep(aux);
    }
}

#endif