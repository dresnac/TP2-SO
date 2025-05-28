#ifndef _PCB_H
#define _PCB_H

#include <stdlib.h>

typedef struct PCB {
    int64_t pid, ppid;
    uint64_t rsp;
    uint8_t status;
}PCB;

#endif