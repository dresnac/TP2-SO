#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <listADT.h>
#include <PCB.h>

void initializeScheduler(int64_t idle_process_pid);
void ready(PCB *process);
uint64_t scheduler(uint64_t current_rsp);
void unschedule(PCB * process);

uint64_t yield();
uint64_t getPid();
uint64_t blockArbitrary(uint64_t pid);
uint64_t unblockArbitrary(uint64_t pid);



#endif