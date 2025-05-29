#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <process.h>
#include <stack_registers.h>

#define GPR_QTY 15

typedef struct {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} gpr;  

typedef struct stack{
    //uint64_t gpr[GPR_QTY];
    gpr regs;
    uint64_t rip;
    uint64_t cs; //0x8
    uint64_t rflags; //0x202
    uint64_t rsp;
    uint64_t ss;  //0x0
}stack;

uint64_t loadStack(uint64_t rip, uint64_t rsp, char ** my_argv, uint64_t my_argc, uint64_t pid);

#endif