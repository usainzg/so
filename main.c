#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "../include/queue.h"
#include "../include/cpu.h"
#include "workers/process_gen.h"
#include "workers/timer.h"
#include "workers/clock.h"
#include "workers/process_gen.h"
#include "workers/sched.c"

#define TIMER_I 0
#define CLOCK_I 1
#define SCHED_I 2
#define GENER_I 3
#define WORKERS 4

extern char *optarg;
extern int optind, optopt;

/**
 * Variables globales del sistema.
 **/
extern int GEN_MIN;
extern int GEN_MAX;
extern int SLP_MIN;
extern int SLP_MAX;
extern int TIMER_T;
extern int Q_MAX;
extern int CPUS;
extern int CORES;
extern int THREADS;
extern int RAM_SIZE;
extern char *FLD_PROGS;

extern double min_time, max_time;
extern int created_threads;
extern int finished_threads;

int exec_time = 0;

int main(int argc, char **argv)
{
    pthread_t workers[WORKERS];
    get_system_params(argc, argv);
    
    if (exec_time > 0) printf("=> Info: duracion maxima en segundos: %d", exec_time);
    else printf("=> Info: no has establecido duracion maxima en segundos.");

    /**
     * TODO: inicializar la estructura general.
     **/
    pthread_create(&workers[CLOCK_I], NULL, clock_worker, NULL);
    pthread_create(&workers[TIMER_I], NULL, timer_worker, NULL);
    pthread_create(&workers[GENER_I], NULL, process_generator_worker, NULL);
    pthread_create(&workers[SCHED_I], NULL, sched_worker, NULL);

    do 
    {
        sleep(1);
        exec_time -= 1;
    } while (exec_time != 0);

    printf("-----------------------------------------------------------------------\n");
    printf("=> Info: %d procesos creados\n", created_threads);
    printf("=> Info: %d tareas terminadas\n", finished_threads);
    printf("=> Info: proceso mas largo: %f segundos\n", max_time);
    printf("=> Info: proceso mas corto: %f segundos\n", min_time);
    printf("-----------------------------------------------------------------------\n");
    return 0;
}

void get_system_params(int argc, char **argv)
{
    int i;
    while (1)
    {
        char c;

        c = getopt(argc, argv, "e:c:C:t:q:k:p:P:s:S:r:h");
        if (c == -1)
        {
            /* Hemos terminado con todos los argumentos. */
            break;
        }

        switch (c)
        {
        case 'e':
            printf("=> Info: tiempo de ejecucion: %s \n", optarg);
            exec_time = atoi(optarg);
            break;
        case 'c':
            printf("=> Info: cores: %s.\n", optarg);
            CORES = atoi(optarg);
            break;
        case 'C':
            printf("=> Info: cpus: %s.\n", optarg);
            CPUS = atoi(optarg);
            break;
        case 't':
            printf("=> Info: threads: %s.\n", optarg);
            THREADS = atoi(optarg);
            break;
        case 'q':
            printf("=> Info: tam max queue: %s.\n", optarg);
            Q_MAX = atoi(optarg);
            break;
        case 'k':
            printf("=> Info: clock time trigger: %s.\n", optarg);
            TIMER_T = atoi(optarg);
            break;
        case 'p':
            printf("=> Info: generacion minima procesos: %s.\n", optarg);
            GEN_MIN = atoi(optarg);
            break;
        case 'P':
            printf("=> Info: generacion maxima procesos: %s.\n", optarg);
            GEN_MAX = atoi(optarg);
            break;
        case 's':
            printf("=> Info: sleep minimo: %s.\n", optarg);
            SLP_MIN = atoi(optarg);
            break;
        case 'S':
            printf("=> Info: sleep maximo: %s.\n", optarg);
            SLP_MAX = atoi(optarg);
            break;
        case 'r':
            printf("=> Info: tam de ram: %s.\n", optarg);
            RAM_SIZE = atoi(optarg);
            break;
        case 'h':
            printf("=> Info: HELP! Implementar! \n");
            printf("Modo de uso: ./main [opciones] \n");
            printf("Opciones: \n");
            printf("    -e, establece el tiempo en segundos que dura la ejecucion (0=infinito). \n");
            printf("    -c, establece el numero de cores. \n");
            printf("    -C, establece el numero de cpus. \n");
            printf("    -t, establece el numero de threads. \n");
            printf("    -q, establece el tam maximo de la cola. \n");
            printf("    -k, establece la cantidad de ciclos para activar timer. \n");
            printf("    -p, establece el minimo numero de procesos a generar. \n");
            printf("    -P, establece el maximo numero de procesos a generar. \n");
            printf("    -s, establece el minimo tiempo a dormir. \n");
            printf("    -S, establece el maximo tiempo a dormir. \n");
            printf("    -r, establece el tam maximo de la memoria. \n");
            printf("    -h, muestra la ayuda del sistema. \n");
            break;
        case '?':
        default:
            printf("Modo de uso: %s [opciones] \n", argv[0]);
        }
    }

    argc -= optind;
    argv += optind;

    // Argumentos que sobran
    if (argc > 0)
    {
        printf("Hay %d argumentos por procesar:\n", argc);
        for (i = 0; i < argc; i++)
        {
            printf("    Argumento %d: '%s'\n", i + 1, argv[i]);
        }
    }
}