#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define GPR_QTY 10

typedef struct stack{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t gpr[GPR_QTY];
}stack;

void loadStack(uint64_t tip, uint64_t rsp);

#endif