#include "insts.h"
#include <stdio.h>
#include <stdint.h>
#include "cpu.h"
#include "ram.h"

typedef void (*Instruction_Table)(Task_cpu *task, Instruction inst);

/* ld */
void inst_0(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = get_word(inst.AAAAAA, &(task->task.mm.pgb));
}

/* st */
void inst_1(Task_cpu *task, Instruction inst)
{
    set_word(inst.AAAAAA, task->ctxt.rg[inst.r1], &(task->task.mm.pgb));
}

/* add */
void inst_2(Task_cpu *task, Instruction inst)
{
    //add
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] + task->ctxt.rg[inst.r3];
}

/* sub */
void inst_3(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] - task->ctxt.rg[inst.r3];
}

/* mul */
void inst_4(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] * task->ctxt.rg[inst.r3];
}

/* div */
void inst_5(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.rg[inst.r3] != 0)
    {
        task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] / task->ctxt.rg[inst.r3];
    }
}

/* and */
void inst_6(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] & task->ctxt.rg[inst.r3];
}

/* or */
void inst_7(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] | task->ctxt.rg[inst.r3];
}

/* xor */
void inst_8(Task_cpu *task, Instruction inst)
{
    //xor
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2] ^ task->ctxt.rg[inst.r3];
}

/* mov */
void inst_9(Task_cpu *task, Instruction inst)
{
    task->ctxt.rg[inst.r1] = task->ctxt.rg[inst.r2];
}

/* cmp */
void inst_A(Task_cpu *task, Instruction inst)
{
    task->ctxt.cc = task->ctxt.rg[inst.r1] - task->ctxt.rg[inst.r2];
}

/* b */
void inst_B(Task_cpu *task, Instruction inst)
{
    task->ctxt.pc = inst.AAAAAA;
}

/* beq */
void inst_C(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.cc == 0)
        task->ctxt.pc = inst.AAAAAA;
}

/* bgt */
void inst_D(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.cc > 0)
        task->ctxt.pc = inst.AAAAAA;
}

/* blt */
void inst_E(Task_cpu *task, Instruction inst)
{
    if (task->ctxt.cc < 0)
        task->ctxt.pc = inst.AAAAAA;
}

/* exit */
void inst_F(Task_cpu *task, Instruction inst)
{
    task->task.life = 0;
}

void execute(Task_cpu *t)
{
    int inst = get_word(t->ctxt.pc, &(t->task.mm.pgb));
    t->ctxt.pc += 4;

    Instruction i;
    uint8_t C = inst >> 28;
    i.r1 = (inst & 0x0FFFFFFF) >> 24;
    i.r2 = (inst & 0x00FFFFFF) >> 20;
    i.r3 = (inst & 0x000FFFFF) >> 16;
    i.AAAAAA = inst & 0x00FFFFFF;

    insts[C](t, i);
}