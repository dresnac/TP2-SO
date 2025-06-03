#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <listADT.h>
#include <PCB.h>
#include <interrupts.h>

void initializeScheduler(tPid shell_process_pid, tPid idle_process_pid);
void ready(PCB *process);
uint64_t scheduler(uint64_t current_rsp);
void unschedule(PCB * process);
void schedulerYield();
uint64_t getPid();
uint64_t blockArbitrary(uint64_t pid);
uint64_t unblockArbitrary(uint64_t pid);
int64_t nice (tPid pid, uint64_t new_prio);
void blockCurrent();
PCB * getRunning();
void unblockWaitingMe();
void unblockWaitingPid ( tPid pid );
void unblockWaitingPcb ( PCB * pcb );
void blockCurrentNoYield();
PCB * getIdlePcb();
PCB * getShellPcb();
void setRunningNull();


#endif