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

/**
 * Cantidad maxima/minima de procesos generados de una vez.
 **/
int GEN_MAX = 20;
int GEN_MIN = 10;

/**
 * Tiempo maximo/minimo de esperar tras generar proceso(s).
 **/
int SLP_MAX = 3;
int SLP_MIN = 1;

int created_threads = 0;

extern int PAGESIZE;
extern sem_t q_sem;

/**
 * Funcion encargada de generar el proceso (tarea).
 **/
void generate_task(Task *t)
{
    t->pid = 1 + rand() % 2500;
    t->priority = 1 + rand() % 100;
    t->mm.pgb = generate_pgb();
    gettimeofday(&t->start_time, 0);
}

/**
 * Funcion encargada de generar el programa.
 **/
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
        
        printf("[P_G] => Carga el programa %s con el id %lu\n", rom, t->pid);
        
        // Calcular direcciones de .text y .data.
        // Tambien asignamos las direcciones.
        t->mm.code = (rom[5] << 2) + (rom[6] << 1) + rom[7];
        t->mm.data = (rom[13] << 2) + (rom[14] << 1) + rom[15];

        // Calcular numero de lineas (se usa luego) y de paginas.
        lines = size - 16;
        pages = (lines / (int) PAGESIZE) + ((lines % (int) PAGESIZE > 0) ? 1 : 0);

        Pages pg;

        for (int i = 0; i < pages; i++)
        {
            pg.virtual_d = t->mm.code + (i * 265);
            aloc = alloc();
            if (aloc < 0)
            {
                printf("[P_G] => No hay espacio en memoria, no se puede crear proceso. \n");
                remove_pgb(&(t->mm.pgb));
                t->pid = 0;
                return;
            }
            pg.physical_d = (unsigned int) aloc;
            insert_pgb(&(t->mm.pgb), pg);
        }

        for (int i = 0; i < lines; i++)
        {
            set(i, rom[i + 16], &(t->mm.pgb));
        }
        
        // Tiempo de vida = numero de lineas.
        t->life = lines;
    }
    else
    {
        printf("[P_G] => Error: fichero erroneo! \n");
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
        printf("[P_G] => Error: Directorio no existe!\n");
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
        printf("[P_G] => Error: Directorio no existe!\n");
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
    printf("[P_G] => hay %d programas para cargar. \n", n_progs);
    int f;
    char *f_c;
    int aux = 0;

    while (1)
    {
        // Cuantos procesos a generar
        aux = GEN_MIN + rand() % (GEN_MAX - GEN_MIN);
        printf("[P_G] => procesos a crear: %d\n", aux);
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
        printf("[P_G] => tiempo a dormir: %d\n", aux);
        sleep(aux);
    }
}

#endif