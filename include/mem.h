#ifndef MEM_H
#define MEM_H

typedef struct
{
    int physical_d;
    int virtual_d;
} Pages;

typedef struct
{
    Pages *data;
    int count;
    int size;
} PGB;

typedef struct
{
    int code;
    int data;
    PGB pgb;
} MM;

#endif