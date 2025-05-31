#ifndef _PCB_H
#define _PCB_H

#include <stdlib.h>

typedef enum {LOW=1, MEDIUM, HIGH} tPriority;

typedef struct PCB {
    uint64_t pid, ppid;
    uint64_t rsp;
    uint8_t status;
    char ** args;
    uint64_t cant;
    int64_t ret;
    tPriority priority;
    struct PCB * waiting_me;
}PCB;

#endif