#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <syscallDispatcher.h>
#include <keyboardDriver.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <process.h>

//hace falta?

MemoryManagerADT getKernelMem();
MemoryManagerADT getUserlandMem();
int main();

#endif