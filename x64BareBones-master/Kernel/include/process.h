#ifndef PROCESS_H
#define PROCESS_H

#include <memoryManager.h>
#include <stack.h>
#include <stdint.h>
#include <scheduler.h>
#include <PCB.h>
#include <shared_libc.h>
#include <shared_defs.h>
#include <kernel_libc.h>
#include <keyboardDriver.h>
#include <drivers_info.h>
#include <time.h>
#include <kernel.h>

#define PCB_AMOUNT 100


typedef int(*main_function)(char ** my_argv, uint64_t my_argc); //creo que se puede sacar

int64_t newProcess(main_function rip, tPriority priority,uint8_t killable,char ** my_argv, uint64_t my_argc, int64_t fds[]); 
PCB * getPcb(tPid pid);
void listProcesses();
int64_t killProcessPcb(PCB * pcb);
int64_t killProcess(tPid pid);
tPid wait (tPid pid, int64_t * ret);
int8_t getStatus(tPid pid);
process_info_list * ps();
void freePs(process_info_list * ps);
void closeFds(PCB * pcb);
int64_t makeMeZombie(int64_t ret);
void ctrlcHandler();


#endif