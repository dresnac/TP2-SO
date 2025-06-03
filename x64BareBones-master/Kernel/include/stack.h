#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <process.h>
//#include <syscallDispatcher.h>
#include <shared_defs.h>

typedef struct stack{
    stack_registers regs;
    uint64_t rip;
    uint64_t cs; //0x8
    uint64_t rflags; //0x202
    uint64_t rsp;
    uint64_t ss;  //0x0
}stack;

uint64_t loadStack(uint64_t rip, uint64_t rsp, char ** my_argv, uint64_t my_argc, tPid pid);

#endif