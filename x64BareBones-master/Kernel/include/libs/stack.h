#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <process.h>
#include <shared_defs.h>


typedef struct stack {
	StackRegisters regs;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} stack;

uint64_t loadStack ( uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc, tPid pid );

#endif