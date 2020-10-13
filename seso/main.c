#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/cpu.h"
#include "workers/process_gen.h"

extern char *optarg;
extern int optind, optopt;
pthread_mutex_t mutex;
sem_t clock_sem, timer_sem;

/**
 * Variables globales para guardar la informacion
 * de los parametros del sistema
 **/
int freq_clock = 100;       // MHz
int cycles_per_ms = 100000; // (freq_clock) MHz * 1000 = cycles_per_ms; 1MHz = 1000 cycles_per_ms
int period_timer = 1;       // ms
int cycles_timer = 100000;  // timer wake up every 100000 cycles (100000 cycles_per_ms * 1ms)
int freq_process_generator = 1;
system_cpus_struct system_cpu;
cpu_struct cpus;
core_struct cores;

void init_system_defaults();
void get_system_params();
void print_system_params();
void create_system_structure();
void *clock_worker();
void *timer_worker();

int main(int argc, char **argv)
{
    Queue *process_queue = make_queue();
    pthread_t clock, timer, process_gen, sched;

    init_system_defaults();
    get_system_params(argc, argv);
    print_system_params();
    create_system_structure();

    pthread_mutex_init(&mutex, NULL);
    sem_init(&clock_sem, 0, 0);
    sem_init(&timer_sem, 0, 1);

    pthread_create(&clock, NULL, clock_worker, NULL);
    pthread_create(&timer, NULL, timer_worker, NULL);
    pthread_create(&process_gen, NULL, process_generator, (void *)process_queue);

    pthread_join(clock, NULL);
    pthread_join(timer, NULL);
    pthread_join(process_gen, NULL);

    // Make space free again
    q_destroy(process_queue);
}

void *clock_worker()
{
    int i;
    int cycles = cycles_timer;
    
    while (1)
    {
        sleep(0.10);
        sem_wait(&timer_sem);
        pthread_mutex_lock(&mutex); // TODO: necesario mutex???

        for (i = 0; i < cycles; i++)
        {
            sleep(0.01);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&clock_sem);
    }
}

void *timer_worker()
{
    while (1)
    {
        sleep(0.10);
        sem_wait(&clock_sem);
        pthread_mutex_lock(&mutex); // TODO: cambiar esto

        printf("INFO: Signal del timer!\n");

        pthread_mutex_unlock(&mutex);
        sem_post(&timer_sem);
    }
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
    while (1)
    {
        char c;

        c = getopt(argc, argv, "c:t:p:C:O:T:");
        if (c == -1)
        {
            /* We have finished processing all the arguments. */
            break;
        }
        switch (c)
        {
        case 'c':
            printf("Frecuencia del clock (MHz) => -c %s.\n", optarg);
            freq_clock = atoi(optarg);
            cycles_per_ms = freq_clock * 1000; // 1MHz = 1000 cycles_per_ms => X MHz * 1000 = X cycles_per_ms
            break;
        case 't':
            printf("Periodo de tiempo del timer (ms) => -t %s.\n", optarg);
            period_timer = atoi(optarg);
            cycles_timer = period_timer * cycles_per_ms;
            break;
        case 'p':
            printf("Frecuencia process generator => -p %s.\n", optarg);
            freq_process_generator = atoi(optarg);
            break;
        case 'C':
            printf("Numero CPUs => -C %s.\n", optarg);
            system_cpu.n_cpus = atoi(optarg);
            break;
        case 'O':
            printf("Numero de cores => -O %s.\n", optarg);
            cpus.n_cores = atoi(optarg);
            break;
        case 'T':
            printf("Numero de threads => -T %s.\n", optarg);
            cores.n_threads = atoi(optarg);
            break;
        case '?':
        default:
            printf("Usage: %s [-c <...>] [-t <...>] [-p <...>] [-C <...>] [-O <...>] [-T <...>]\n", argv[0]);
        }
    }

    // Set argc and argv after process options
    argc -= optind;
    argv += optind;

    // Remaining arguments
    if (argc > 0)
    {
        printf("There are %d command-line arguments left to process:\n", argc);
        for (i = 0; i < argc; i++)
        {
            printf("    Argument %d: '%s'\n", i + 1, argv[i]);
        }
    }
}

void print_system_params()
{
    printf("---------------------------\n");
    printf(" => freq_clock: %d MHz\n", freq_clock);
    printf(" => period_timer: %d ms\n", period_timer);
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