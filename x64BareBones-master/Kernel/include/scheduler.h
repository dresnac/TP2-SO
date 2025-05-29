#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <listADT.h>
#include <PCB.h>

void initializeScheduler(int64_t idle_process_pid);
void ready(PCB *process);
uint64_t scheduler(uint64_t current_rsp);
void unschedule(PCB * process);

void yield(PCB * process);
uint64_t getPid();
void blockArbitrary(uint64_t pid);
void unblockArbitrary(uint64_t pid);



#endif