
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

int64_t new_process ( mainFunction rip, tPriority priority, uint8_t killable, char ** argv, uint64_t argc, tFd fds[] );
PCB * get_pcb ( tPid pid );
void list_processes();
int64_t kill_process ( tPid pid );
int64_t kill_process_pcb ( PCB * pcb );
tPid wait ( tPid pid, int64_t * ret );
ProcessInfoList * ps();
int8_t get_status ( tPid pid );
void free_ps ( ProcessInfoList * ps );
void close_fds ( PCB * pcb );
void ctrl_c_handler();
int64_t make_me_zombie ( int64_t retval );
#endif
