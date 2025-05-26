#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define GPR_QTY 15

typedef struct stack{
    uint64_t gpr[GPR_QTY];
    uint64_t rip;
    uint64_t cs; //0x8
    uint64_t rflags; //0x202
    uint64_t rsp;
    uint64_t ss;  //0x0
}stack;

uint64_t loadStack(uint64_t rip, uint64_t rsp);

#endif