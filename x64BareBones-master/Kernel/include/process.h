#ifndef PROCESS_H
#define PROCESS_H

#include <memoryManager.h>
#include <stack.h>
#include <stdint.h>
#include <scheduler.h>
#include <PCB.h>

#define FREE 0
#define READY 1
#define BLOCKED 2
#define ZOMBIE 3 

/*
typedef struct PCB{
    int64_t pid, ppid;
    uint64_t rsp;
    uint8_t status;
}PCB; */

//typedef enum { LOW = 0, MEDIUM, HIGH } tPriority;
typedef int(*main_function)(char ** my_argv, uint64_t my_argc);

int64_t newProcess(main_function rip, tPriority priority, char ** my_argv, uint64_t my_argc); 
PCB * getPcb(int64_t pid);

//void listProcesses();

#endif