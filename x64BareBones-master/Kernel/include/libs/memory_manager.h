#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <shared_defs.h>


typedef struct MemoryManagerCDT * MemoryManagerADT;

MemoryManagerADT createMM ( void * p );
void * allocMemory ( uint64_t size, struct MemoryManagerCDT * mem );
void freeMemory ( void * p, MemoryManagerADT adt );
int64_t memInfo ( MemoryInfo * info, MemoryManagerADT adt );



#define BLOCK_SIZE 0x1000                       // 4k
#define HEAP_SIZE 0x1000000                     // 16MB

#endif