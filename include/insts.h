#ifndef INSTS_H
#define INSTS_H

#include <stdio.h>
#include "cpu.h"

void Execute(Task_cpu *t);

typedef void (*Instruction_Table) (Task_cpu *task, Instruction inst);

void inst_0(Task_cpu *task, Instruction inst);
void inst_1(Task_cpu *task, Instruction inst);
void inst_2(Task_cpu *task, Instruction inst);
void inst_3(Task_cpu *task, Instruction inst);
void inst_4(Task_cpu *task, Instruction inst);
void inst_5(Task_cpu *task, Instruction inst);
void inst_6(Task_cpu *task, Instruction inst);
void inst_7(Task_cpu *task, Instruction inst);
void inst_8(Task_cpu *task, Instruction inst);
void inst_9(Task_cpu *task, Instruction inst);
void inst_A(Task_cpu *task, Instruction inst);
void inst_B(Task_cpu *task, Instruction inst);
void inst_C(Task_cpu *task, Instruction inst);
void inst_D(Task_cpu *task, Instruction inst);
void inst_E(Task_cpu *task, Instruction inst);
void inst_F(Task_cpu *task, Instruction inst);

Instruction_Table insts[16] = {
    &inst_0, &inst_1, &inst_2, &inst_3, &inst_4, &inst_5, &inst_6, &inst_7, &inst_8, &inst_9, 
    &inst_A, &inst_B, &inst_C, &inst_D, &inst_E, &inst_F
};

#endif