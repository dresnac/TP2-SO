//#ifdef MEM_MANAGER
#include <memoryManager.h>

#define BLOCK_COUNT ((HEAP_SIZE) / BLOCK_SIZE)

typedef struct {
	void *start;
	int64_t current;
	void *free_ptrs[BLOCK_COUNT];
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void * p) {
	MemoryManagerCDT * aux = ( MemoryManagerCDT * ) p;

	for ( int i = 0; i < BLOCK_COUNT; i++ ) {
		aux->free_ptrs[i] = ( void * ) ( ( char * ) p + i * BLOCK_SIZE );
	}

	aux->start = ( void * ) ( ( char * ) p );
	aux->current = 0;
	for ( int i = 0; i <= ( sizeof ( MemoryManagerCDT ) / BLOCK_SIZE ); i++ ) {
		allocMemory ( 1, ( MemoryManagerADT ) aux );														//reserves the space for the CDT
	}

	return ( MemoryManagerADT ) aux;
}

void *allocMemory(uint64_t size,  MemoryManagerADT mem ) {
    MemoryManagerCDT * aux = mem;
	char *allocation = aux->current;

	aux->current += size;

	return (void *) allocation;
}

//#endif