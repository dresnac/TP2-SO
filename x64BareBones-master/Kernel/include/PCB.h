#ifndef _PCB_H
#define _PCB_H

#include <stdlib.h>

typedef struct PCB {
    uint64_t pid, ppid;
    uint64_t rsp;
    uint8_t status;
    char ** args;
    uint64_t cant;
}PCB;

#endif