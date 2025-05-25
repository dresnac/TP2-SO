#ifndef PROCESS_H
#define PROCESS_H

#include <memoryManager.h>
#include <stack.h>
#include <stdint.h>

#define FREE 0
#define READY 1
#define BLOCKED 2
#define RUNNING 3 //?

typedef struct PCB{
    //int64_t pid, ppid;
    uint64_t rsp;
    uint8_t status;
}PCB;

#endif