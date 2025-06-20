
#ifndef PROCESS_H
#define PROCESS_H

#include <memory_manager.h>
#include <stack.h>
#include <stdint.h>
#include <scheduler.h>
#include <PCB.h>
#include <shared_libc.h>
#include <shared_defs.h>
#include <semaphores.h>
#include <kernel_libc.h>
#include <drivers_information.h>
#include <pipe.h>
#include <keyboard_driver.h>
#include <time.h>
#include <kernel.h>

#define PCB_AMOUNT 100




typedef int ( *mainFunction ) ( char ** argv, uint64_t argc );

int64_t newProcess ( mainFunction rip, tPriority priority, uint8_t killable, char ** argv, uint64_t argc, tFd fds[] );
PCB * getPcb ( tPid pid );
//void listProcesses();
int64_t killProcess ( tPid pid );
int64_t killProcessPcb ( PCB * pcb );
tPid wait ( tPid pid, int64_t * ret );
ProcessInfoList * ps();
int8_t getStatus ( tPid pid );
void freePs ( ProcessInfoList * ps );
void closeFds ( PCB * pcb );
void ctrlcHandler();
int64_t makeMeZombie ( int64_t retval );
#endif
