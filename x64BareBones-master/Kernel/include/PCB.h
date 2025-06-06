#ifndef _PCB_H
#define _PCB_H

#include <stdlib.h>
#include <shared_defs.h>
#include <drivers_info.h>


typedef struct PCB {
    uint64_t pid, ppid;
    uint64_t rsp;
    uint8_t status;
    char ** args;
    uint64_t cant;
    int64_t ret;
    tPriority priority;
    struct PCB * waiting_me;
    struct PCB * waiting_for;
    int64_t fds[CANT_FDS];
    uint8_t killable;
    uint64_t lowest_stack_address;
    uint64_t time;
    uint64_t start;
    int64_t blocked_by_sem;
}PCB;

#endif