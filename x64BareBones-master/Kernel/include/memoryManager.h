#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <shared_defs.h>
//#include <syscallDispatcher.h>

typedef struct MemoryManagerCDT *MemoryManagerADT;

//crea admin de mem
MemoryManagerADT createMemoryManager(void * p);

//asignar bloque de mem
void *allocMemory(MemoryManagerADT mem, uint64_t size);

//libera bloque de mem. Se le pasa admin de mem y bloque a liberar
void freeMemory(MemoryManagerADT mem, void * p);

int64_t memInfo(memoryInfo * info, MemoryManagerADT mem);

#define BLOCK_SIZE 0x1000                       // 4k
#define HEAP_SIZE 0x1000000                     // 16MB

#endif