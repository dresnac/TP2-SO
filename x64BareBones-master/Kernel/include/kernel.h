#ifndef KERNEL_H
#define KERNEL_H


#include <stdint.h>
#include <string.h>
#include <module_loader.h>
#include <video_driver.h>
#include <idt_loader.h>
#include <syscalls.h>
#include <keyboard_driver.h>
#include <memory_manager.h>
#include <shared_libc.h>
#include <process.h>
#include <scheduler.h>
#include <pipe.h>
#include <time.h>
MemoryManagerADT getUserlandMem();
MemoryManagerADT getKernelMem();
int main();

#endif