#include <stdio.h>
#include <unistd.h>
#include "../include/pcb.h"
#include "../include/queue.h"
#include "../include/cpu.h"

int main(int argc, char **argv)
{
    int i;
    extern char *optarg;
    extern int optind, optopt;

    while (1) {
        char c;

        c = getopt (argc, argv, "c:t:p:C:O:T:");
        if (c == -1) {
            /* We have finished processing all the arguments. */
            break;
        }
        switch (c) {
        case 'c':
            printf ("User has invoked with -c %s.\n", optarg);
            break;
        case 't':
            printf ("User has invoked with -t %s.\n", optarg);
            break;
        case 'p':
            printf ("User has invoked with -p %s.\n", optarg);
            break;
        case 'C':
            printf ("User has invoked with -C %s.\n", optarg);
            break;
        case 'O':
            printf ("User has invoked with -O %s.\n", optarg);
            break;
        case 'T':
            printf ("User has invoked with -T %s.\n", optarg);
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