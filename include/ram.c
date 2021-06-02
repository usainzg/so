#include "ram.h"
#include "helpers.h"

RAM ram_mem;
Kernel_data kernel_data;

void init()
{
    ram_mem.size = 2 << RAM_SIZE;
    kernel_data.pageAmount = ram_mem.size / PAGESIZE;
    kernel_data.reserved = kernel_data.pageAmount / 8;
    if (kernel_data.pageAmount % 8 > 0) kernel_data.reserved++;
    
    ram_mem.data = malloc(sizeof(Word) * (ram_mem.size + kernel_data.reserved));
    memset(ram_mem.data, 0x00, ram_mem.size + kernel_data.reserved);
}

/**
 * Busca en la tabla de paginas una pagina libre,
 * devuelve la direccion a la primera posicion de la tabla.
 * Modifica la tabla de paginas ocupando esa pagina.
 **/
int alloc()
{
    int cB = -1;
    int dF = -1;

    SemaforoDown(&pageT_sem);
    for (int i = 0; i < kernel_data.reserved && dF == -1; i++)
    {
        cB = check(sizeof(uint8_t), &ram_mem.data[ram_mem.size + i]);
        if (cB > -1)
        {
            dF = 8 * i + cB;
            uint8_t suma = 0x01 << cB;
            ram_mem.data[ram_mem.size + i] = ram_mem.data[ram_mem.size + i] | suma;
        }
    }

    SemaforoUp(&pageT_sem);
    return (unsigned int) dF * PAGESIZE;
}

/**
 * Devuelve la posicion del bit mas alto con valor 0. 
 * Se usa para no ocupar toda una casilla con la tabla de paginas,
 * solo dedicamos un bit.
 **/
int check(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            if (!byte)
            {
                return j;
            }
        }
    return -1;
}

/**
 * Busca la pagina que comienza por dF y la cambia a disponible.
 **/
void free(int dF)
{
    int df = dF / PAGESIZE;
    int cB = df % 8;
    int i = (df - cB) / 8;

    SemaforoDown(&pageT_sem);
    uint8_t slot = ram_mem.data[ram_mem.size + i];
    uint8_t resta = 0x01 << cB;
    if (slot != 0)
    {
        ram_mem.data[ram_mem.size + i] = slot ^ resta;
    }
    SemaforoUp(&pageT_sem);
}

void set(int addr, uint8_t val, PGB *pgb)
{
    SemaforoDown(&ram_sem);
    ram_mem.data[get_pgb(pgb, addr)] = val;
    SemaforoUp(&ram_sem);
}

uint8_t get(int addr, PGB *pgb)
{
    uint8_t ret;
    SemaforoDown(&ram_sem);
    ret = ram_mem.data[get_pgb(pgb, addr)];
    SemaforoUp(&ram_sem);
    return ret;
}

void set_word(int addr, int val, PGB *pgb)
{
    int value = val;
    int v = val;

    for (int i = 3; i >= 0; i--)
    {
        v = (value >> 8 * i);
        value = value - (v << 8 * i);
        ram_mem.data[get_pgb(pgb, addr + (3 - i))] = v;
    }
}

unsigned int get_word(int addr, PGB *pgb)
{
    unsigned int word = 0;
    for (int i = 0; i < 4; i++)
    {
        word <<= 8;
        word += Get(addr + i, pgb);
    }
    return word;
}

void print(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *) ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    puts("");
}