#ifndef INSTS_H
#define INSTS_H

#include <stdio.h>
#include "structs.h"
#include "ram.h"

typedef void (*Instruction_Table)(Task_cpu *task, Instruction inst);

/* ld */
void inst_0(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = get_word(inst.AAAAAA, &(task->task.mm.pgb));
    printf("[INS] => (%ld) ld %d %X\n", task->task.pid, inst.r1, inst.AAAAAA);
}

/* st */
void inst_1(Task_cpu *task, Instruction inst)
{
    set_word(inst.AAAAAA, task->ctxt.rg[inst.r1], &(task->task.mm.pgb));
    printf("[INS] => (%ld) st %d %X\n", task->task.pid, inst.r1, inst.AAAAAA);
}

/* add */
void inst_2(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] + task->ctxt.rg[inst.r3];
    printf("[INS] => (%ld) add %d %d %d\n", task->task.pid, inst.r1, inst.r2, inst.r3);
}

/* sub */
void inst_3(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] - task->ctxt.rg[inst.r3];
    printf("[INS] => (%ld) sub %d %d %d\n", task->task.pid, inst.r1, inst.r2, inst.r3);
}

/* mul */
void inst_4(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] * task->ctxt.rg[inst.r3];
    printf("[INS] => (%ld) mul %d %d %d\n", task->task.pid, inst.r1, inst.r2, inst.r3);
}

/* div */
void inst_5(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.rg[inst.r3] != 0)
    {
        task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] / task->ctxt.rg[inst.r3];
    }
    printf("[INS] => (%ld) div %d %d %d\n", task->task.pid, inst.r1, inst.r2, inst.r3);
}

/* and */
void inst_6(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] & task->ctxt.rg[inst.r3];
    printf("[INS] => (%ld) and %d %d %d\n", task->task.pid, inst.r1, inst.r2, inst.r3);
}

/* or */
void inst_7(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] | task->ctxt.rg[inst.r3];
    printf("[INS] => (%ld) or %d %d %d\n", task->task.pid, inst.r1, inst.r2, inst.r3);
}

/* xor */
void inst_8(Task_cpu *task, Instruction inst)
{
    //xor
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] ^ task->ctxt.rg[inst.r3];
    printf("[INS] => (%ld) xor %d %d %d\n", task->task.pid, inst.r1, inst.r2, inst.r3);
}

/* mov */
void inst_9(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2];
    printf("[INS] => (%ld) mov %d %d\n", task->task.pid, inst.r1, inst.r2);
}

/* cmp */
void inst_A(Task_cpu *task, Instruction inst)
{
    task->ctxt.cc = task->ctxt.rg[inst.r1] - task->ctxt.rg[inst.r2];
    printf("[INS] => (%ld) cmp %d %d\n", task->task.pid, inst.r1, inst.r2);
}

/* b */
void inst_B(Task_cpu *task, Instruction inst)
{
    task->ctxt.pc = inst.AAAAAA;
    printf("[INS] => (%ld) b %d\n", task->task.pid, inst.AAAAAA);
}

/* beq */
void inst_C(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.cc == 0)
        task->ctxt.pc = inst.AAAAAA;
    printf("[INS] => (%ld) beq %d\n", task->task.pid, inst.AAAAAA);
}

/* bgt */
void inst_D(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.cc > 0)
        task->ctxt.pc = inst.AAAAAA;
    printf("[INS] => (%ld) bgt %d\n", task->task.pid, inst.AAAAAA);
}

/* blt */
void inst_E(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.cc < 0)
        task->ctxt.pc = inst.AAAAAA;
    printf("[INS] => (%ld) blt %d\n", task->task.pid, inst.AAAAAA);
}

/* exit */
void inst_F(Task_cpu *task, Instruction inst)
{
    task->task.life = 0;
    printf("[INS] => (%ld) exit\n", task->task.pid);
}

/**
 * Array de funciones para poder facilitar la llamada.
 **/
Instruction_Table insts[16] = {
    &inst_0, &inst_1, &inst_2, &inst_3, &inst_4, &inst_5, &inst_6, &inst_7, &inst_8, &inst_9, 
    &inst_A, &inst_B, &inst_C, &inst_D, &inst_E, &inst_F
};

/**
 * Funcion para obtener el pc contenido en t.pc, calcular el nuevo pc,
 * generar la estructura Instruction con todos sus campos y llama
 * a la instruccion correspondiente.
 **/
void execute(Task_cpu *t)
{
    unsigned int inst = get_word(t->ctxt.pc, &(t->task.mm.pgb));
    t->ctxt.pc += 4;

    Instruction i;
    uint8_t C = inst >> 28;
    i.r1 = (inst & 0x0FFFFFFF) >> 24;
    i.r2 = (inst & 0x00FFFFFF) >> 20;
    i.r3 = (inst & 0x000FFFFF) >> 16;
    i.AAAAAA = inst & 0x00FFFFFF;

    insts[C](t, i);
}

#endif