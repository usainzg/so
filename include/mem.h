#ifndef MEM_H
#define MEM_H

/**
 * Para relacionar una direccion virtual con una fisica.
 **/
typedef struct
{
    int physical_d;
    int virtual_d;
} Pages;

/**
 * Count: cuantas paginas.
 * Size: tamanio de la lista reservada.
 * Data: lista.
 **/
typedef struct
{
    Pages *data;
    int count;
    int size;
} PGB;

/**
 * Memory Mapping de un proceso.
 * Code: direccion memoria virtual de code.
 * Data: direccion memoria virtual de data.
 **/
typedef struct
{
    int code;
    int data;
    PGB pgb;
} MM;

#endif