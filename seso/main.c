#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/cpu.h"
#include "workers/process_gen.h"

extern char *optarg;
extern int optind, optopt;

/**
 * Variables globales para guardar la informacion
 * de los parametros del sistema
 **/
int freq_clock = 1000; // MHz => 1 GHz
int period_timer = 1; // periodo de 1ns, 1 ciclo por cada 1ns
int freq_process_generator = 1;
system_cpus_struct system_cpu;
cpu_struct cpus;
core_struct cores;

void init_system_defaults();
void get_system_params();
void print_system_params();
void create_system_structure();

int main(int argc, char **argv)
{
    init_system_defaults();
    get_system_params(argc, argv);
    print_system_params();
    create_system_structure();
}

void init_system_defaults()
{
    system_cpu.n_cpus = 1;
    cpus.n_cores = 1;
    cores.n_threads = 1;
}

void get_system_params(int argc, char **argv)
{
    int i;
     while (1) {
        char c;

        c = getopt (argc, argv, "c:t:p:C:O:T:");
        if (c == -1) {
            /* We have finished processing all the arguments. */
            break;
        }
        switch (c) {
        case 'c':
            printf ("Frecuencia del clock => -c %s.\n", optarg);
            freq_clock = atoi(optarg);
            break;
        case 't':
            printf ("Periodo de tiempo del timer => -t %s.\n", optarg);
            period_timer = atoi(optarg);
            break;
        case 'p':
            printf ("Frecuencia process generator => -p %s.\n", optarg);
            freq_process_generator = atoi(optarg);
            break;
        case 'C':
            printf ("Numero CPUs => -C %s.\n", optarg);
            system_cpu.n_cpus = atoi(optarg);
            break;
        case 'O':
            printf ("Numero de cores => -O %s.\n", optarg);
            cpus.n_cores = atoi(optarg);
            break;
        case 'T':
            printf ("Numero de threads => -T %s.\n", optarg);
            cores.n_threads = atoi(optarg);
            break;
        case '?':
        default:
            printf ("Usage: %s [-c <...>] [-t <...>] [-p <...>] [-C <...>] [-O <...>] [-T <...>]\n", argv[0]);
        }
    }

    // Set argc and argv after process options 
    argc -= optind;
    argv += optind;

    // Remaining arguments
    if (argc > 0) {
        printf ("There are %d command-line arguments left to process:\n", argc);
        for (i = 0; i < argc; i++) {
            printf ("    Argument %d: '%s'\n", i + 1, argv[i]);
        }
    }
}

void print_system_params()
{
    printf("---------------------------\n");
    printf(" => freq_clock: %d\n", freq_clock);
    printf(" => period_timer: %d\n", period_timer);
    printf(" => freq_proc_gen: %d\n", freq_process_generator);
    printf(" => CPUs: %d\n", system_cpu.n_cpus);
    printf(" => cores_per_cpu: %d\n", cpus.n_cores);
    printf(" => threads_per_core: %d\n", cores.n_threads);
    printf("---------------------------\n");
}

void create_system_structure()
{
    int i, j, z;
    system_cpu.cpus_arr = malloc(sizeof(cpu_struct) * system_cpu.n_cpus);

    for (i = 0; i < system_cpu.n_cpus; i++)
    {
        system_cpu.cpus_arr[i].n_cores = cpus.n_cores;
        system_cpu.cpus_arr[i].cores_arr = malloc(sizeof(core_struct) * cpus.n_cores);

        for (j = 0; j < cpus.n_cores; j++)
        {
            system_cpu.cpus_arr[i].cores_arr[j].n_threads = cores.n_threads;
            system_cpu.cpus_arr[i].cores_arr[j].threads_arr = malloc(sizeof(thread_struct) * cores.n_threads);

            for (z = 0; z < cores.n_threads; z++)
            {
                system_cpu.cpus_arr[i].cores_arr[j].threads_arr[z].thread_id = z;

                printf("CPU: %d, core: %d, thread: %d\n", i, j, z);
            }
        }
    }
}