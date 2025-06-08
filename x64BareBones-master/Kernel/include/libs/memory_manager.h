#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <shared_defs.h>


typedef struct MemoryManagerCDT * MemoryManagerADT;

MemoryManagerADT my_mm_init ( void * p );
void * alloc_memory ( uint64_t size, struct MemoryManagerCDT * mem );
void free_memory ( void * p, MemoryManagerADT adt );
int64_t mem_info ( MemoryInfo * info, MemoryManagerADT adt );



#define BLOCK_SIZE 0x1000                       // 4k
#define HEAP_SIZE 0x1000000                     // 16MB

#endif