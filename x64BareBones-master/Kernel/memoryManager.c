#ifdef FROM_SCRATCH
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
		allocMemory ( ( MemoryManagerADT ) aux , 1);
	}

	return ( MemoryManagerADT ) aux;
}

void *allocMemory(MemoryManagerADT mem , uint64_t size) {
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if ( aux == NULL || size > BLOCK_SIZE || aux->current >= BLOCK_COUNT ) {
		return NULL;
	}

	return aux->free_ptrs[aux->current++];
}

void freeMemory(MemoryManagerADT mem, void * p){
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if ( aux == NULL ) {
		return;
	}
	aux->current--;
	aux->free_ptrs[aux->current] = p;
}

int64_t memInfo(MemoryManagerADT mem,memoryInfo * info){
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if ( aux == NULL || info == NULL ) {
		return -1;
	}
	info->total_size = HEAP_SIZE;
	info->free = ( ( BLOCK_COUNT - aux->current ) * BLOCK_SIZE );
	return 0;
}

#endif