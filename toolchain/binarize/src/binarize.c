#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// Funcion auxiliar para obtener el nombre del
// fichero incluido en un path.
char *get_file_from_path(char *path)
{
    for (size_t i = strlen(path) - 1; i; i--)
    {
        if (path[i] == '/')
        {
            return &path[i + 1];
        }
    }
    return path;
}

// Funcion auxiliar para obtener hex de un char.
uint8_t get_hex(char c)
{
    uint8_t ret;
    switch (c)
    {
    case '0':
        ret = 0x00;
        break;
    case '1':
        ret = 0x01;
        break;
    case '2':
        ret = 0x02;
        break;
    case '3':
        ret = 0x03;
        break;
    case '4':
        ret = 0x04;
        break;
    case '5':
        ret = 0x05;
        break;
    case '6':
        ret = 0x06;
        break;
    case '7':
        ret = 0x07;
        break;
    case '8':
        ret = 0x08;
        break;
    case '9':
        ret = 0x09;
        break;
    case 'A':
        ret = 0x0A;
        break;
    case 'B':
        ret = 0x0B;
        break;
    case 'C':
        ret = 0x0C;
        break;
    case 'D':
        ret = 0x0D;
        break;
    case 'E':
        ret = 0x0E;
        break;
    case 'F':
        ret = 0x0F;
        break;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int i;

    if (argc == 1)
    {
        printf("=> Error: no hay fichero de entrada! \n");
        return 0;
    }

    FILE *pf = fopen(argv[1], "r");
    
    if (NULL != pf)
    {
        // Calculamos size del fichero, lo leemos y cerramos.
        fseek(pf, 0, SEEK_END);
        int f_size = ftell(pf);
        char *code = malloc(sizeof(char) * f_size);
        fseek(pf, 0, SEEK_SET);
        fread(code, f_size, 1, pf);
        fclose(pf);

        // Nombre del nuevo fichero binario.
        char *bin_filename = get_file_from_path(argv[1]);
        FILE *f_bin = fopen(bin_filename, "wb"); // nuevo fichero binario.
        
        uint8_t datum;

        // 0 - 5 => .text 2E74657874
        datum = 0x2E;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x74;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x65;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x78;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x74;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);

        // 6 - 11 => code[i]
        for (i = 6; i < 12; i += 2)
        {
            datum = get_hex(code[i]);
            datum <<= 4;
            datum += get_hex(code[i + 1]);
            fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        }

        // 7 - 7 => \n

        // 8 - 13 => .data 2E64617661
        datum = 0x2E;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x64;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x61;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x74;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        datum = 0x61;
        fwrite(&datum, 1, sizeof(uint8_t), f_bin);

        // 18 - 14 => code[i]
        for (i = 19; i < 25; i += 2)
        {
            datum = get_hex(code[i]);
            datum <<= 4;
            datum += get_hex(code[i + 1]);
            fwrite(&datum, 1, sizeof(uint8_t), f_bin);
        }

        datum = 0;
        int par = 1;
        for (i = 25; i < f_size; i++)
        {
            if (code[i] != '\n')
            {
                datum += get_hex(code[i]);
                if (par % 2 == 0)
                {
                    fwrite(&datum, 1, sizeof(uint8_t), f_bin);
                    datum = 0x00;
                }
                else
                {
                    datum <<= 4;
                }

                par++;
            }
        }

        if (par % 2 != 0)
        {
            datum <<= 4; // datum = datum << 1;
            datum += 0x00;
        }

        fclose(f_bin);
    }

    return 1;
}