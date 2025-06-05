//#ifdef BUDDY
#include <memoryManager.h>

#define GET_SIBLING(i) (((i) % 2) ? ((i) + 1) : ((i) - 1))
#define GET_PARENT(i) (((i) - 1) / 2)

#define MAX_MEM_SIZE HEAP_SIZE
#define MIN_BLOCK_SIZE BLOCK_SIZE


#define TREE_BIT_MAP_SIZE (((MAX_MEM_SIZE / MIN_BLOCK_SIZE)) * 2)


static int next_power_of_2 ( int n );
static int get_index_level ( int index );
static int get_size_level ( int size );
static int get_block_from_index ( int index );
static void my_free_idx ( int index, int * flag, int n, MemoryManagerADT mem );
static void *my_alloc ( int index, int level, MemoryManagerADT mem );


typedef struct {
	void *start;
	char tree_bitmap[TREE_BIT_MAP_SIZE];
	uint64_t free_mem;
} MemoryManagerCDT;

#define LEVELS 13  // log2(HEAP_SIZE / BLOCK_SIZE) + 1

typedef struct Block {
    struct Block *next;
} Block;


static int getLevel(size_t size) {
    size_t s = BLOCK_SIZE;
    int level = 0;
    while (s < size && level < LEVELS - 1) {
        s <<= 1;
        level++;
    }
    return level;
}

static size_t levelSize(int level) {
    return BLOCK_SIZE << level;
}

MemoryManagerADT createMemoryManager(void * p) {

    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) p;

	aux->start = ( void * ) ( ( char * ) p );

	for ( int i = 0; i < TREE_BIT_MAP_SIZE; i++ ) {
		aux->tree_bitmap[i] = 0;
	}
	aux->free_mem = MAX_MEM_SIZE;
	MemoryManagerADT ans = allocMemory ( sizeof ( *aux ), ( MemoryManagerADT ) aux );
	return ans;

}

void * allocMemory( uint64_t size,  MemoryManagerADT mem ) {
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if( aux == NULL ){
		return NULL;
	}
	int npo2 = next_power_of_2 ( size );
	int real_size = npo2 >= MIN_BLOCK_SIZE ? npo2 : MIN_BLOCK_SIZE;
	int level = get_size_level ( real_size );

	void * ptr = allocMemory ( 0, level, mem );  //
	if ( ptr != NULL ) {
		aux->free_mem -= real_size;
	}

	return ptr;
}

static int getBuddyIndex(uint8_t *base, void *addr, size_t size) {
    return ((uintptr_t)addr - (uintptr_t)base) / size;
}

void freeMemory( void * p, MemoryManagerADT mem ) {
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if( aux == NULL ){
		return;
	}
	if ( ( p - aux->start ) % MIN_BLOCK_SIZE != 0 ) {
		return;
	}

	int index = ( ( p - aux->start ) / MIN_BLOCK_SIZE ) + MAX_MEM_SIZE / MIN_BLOCK_SIZE - 1; // we aux->start with the block of maximum granularity
	int flag = 0;
	int n = 1;
	freeIndex ( index, &flag, n, mem ); //
}


int64_t memInfo(memoryInfo * info, MemoryManagerADT adt){
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if ( info == NULL || aux == NULL ) {
		return -1;
	}
	info->total_size = MAX_MEM_SIZE;
	info->free = aux->free_mem;
	return 0;
}


static int next_power_of_2 ( int n )
{
	int count = 0;
	if ( n && ! ( n & ( n - 1 ) ) )
		return n;
	while ( n != 0 ) {
		n >>= 1;
		count += 1;
	}
	return 1 << count;
}


//#endif