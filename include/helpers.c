#include "helpers.h"
#include <stdio.h>

void print_info(char *str, int arrow)
{
    if (arrow == 0)
    {
        printf("INFO: %s\n", str);   
    }
    else
    {
        printf(" => INFO: %s\n", str);
    }
}