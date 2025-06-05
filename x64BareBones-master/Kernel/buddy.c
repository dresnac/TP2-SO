#ifdef BUDDY
#include <memoryManager.h>

#define GET_SIBLING(i) (((i) % 2) ? ((i) + 1) : ((i) - 1))
#define GET_PARENT(i) (((i) - 1) / 2)

#define MAX_MEM_SIZE HEAP_SIZE
#define MIN_BLOCK_SIZE BLOCK_SIZE


#define TREE_BIT_MAP_SIZE (((MAX_MEM_SIZE / MIN_BLOCK_SIZE)) * 2)


static int nextPowerOf2 ( int n );
static int getIndexLevel ( int index );
static int getSizeLevel ( int size );
static int getBlock ( int index );
static void freeIndex ( int index, int * flag, int n, MemoryManagerADT mem );
static void * myAlloc ( int index, int level, MemoryManagerADT mem );


typedef struct {
	void *start;
	char tree_bitmap[TREE_BIT_MAP_SIZE];
	uint64_t free_mem;
} MemoryManagerCDT;


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
	int npo2 = nextPowerOf2 ( size );
	int real_size = npo2 >= MIN_BLOCK_SIZE ? npo2 : MIN_BLOCK_SIZE;
	int level = getSizeLevel ( real_size );

	void * ptr = myAlloc ( 0, level, mem ); 
	if ( ptr != NULL ) {
		aux->free_mem -= real_size;
	}

	return ptr;
}

void freeMemory( void * p, MemoryManagerADT mem ) {
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if( aux == NULL ){
		return;
	}
	if ( ( p - aux->start ) % MIN_BLOCK_SIZE != 0 ) {
		return;
	}

	int index = ( ( p - aux->start ) / MIN_BLOCK_SIZE ) + MAX_MEM_SIZE / MIN_BLOCK_SIZE - 1;
	int flag = 0;
	int n = 1;
	freeIndex ( index, &flag, n, mem );
}

int64_t memInfo(memoryInfo * info, MemoryManagerADT mem){
    MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if ( info == NULL || aux == NULL ) {
		return -1;
	}
	info->total_size = MAX_MEM_SIZE;
	info->free = aux->free_mem;
	return 0;
}

static void freeIndex ( int index, int * flag, int n, MemoryManagerADT mem ){
	MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if( aux == NULL ){
		return;
	}
	if ( index == 0 ) {
		aux->tree_bitmap[index] = 0;
		return;
	}
	if ( aux->tree_bitmap[index] == 1 && ! ( *flag ) ) {
		aux->free_mem += MIN_BLOCK_SIZE * n;
		*flag = 1;
	}
	aux->tree_bitmap[index] = 0;

	if ( aux->tree_bitmap[GET_SIBLING ( index )] ) { // si el "buddy" está ocupado
		return;
	}
	freeIndex ( GET_PARENT ( index ), flag, n * 2, mem );
}




static void * myAlloc ( int index, int level, MemoryManagerADT mem ){
	MemoryManagerCDT * aux = ( MemoryManagerCDT * ) mem;
	if ( aux == NULL ) {
		return NULL;
	}
	if ( level == 0 ) {
		if ( aux->tree_bitmap[index] ) {
			return NULL;
		}
		aux->tree_bitmap[index] = 1;
		return aux->start + getBlock ( index ) * MIN_BLOCK_SIZE;
	}
	if ( aux->tree_bitmap[index] && !aux->tree_bitmap[2 * index + 1] && !aux->tree_bitmap[index * 2 + 2] ) {
		return NULL;
	}
    //Si el bloque está ocupado, pero sus hijos libros, entonces el bloque no se divide sino que se ocupa completamente

	void *ptr = myAlloc ( index * 2 + 1, level - 1, mem );
	if ( ptr == NULL ) {
		ptr = myAlloc ( index * 2 + 2, level - 1, mem );
		if ( ptr == NULL ) {
			return NULL;
		}
	}
	aux->tree_bitmap[index] = 1;
	return ptr;
}


static int nextPowerOf2 ( int n ){
	int count = 0;
	if ( n && ! ( n & ( n - 1 ) ) )
		return n;
	while ( n != 0 ) {
		n >>= 1;
		count += 1;
	}
	return 1 << count;
}

static int getIndexLevel ( int index ){
	int level = 0;
	for ( int acum = 0; index > acum; level++ ) {
		acum += 2 << level;
	}
	return level;
}

static int getSizeLevel ( int size ){
	int level = 0;
	for ( ; 2 * size - 1 < MAX_MEM_SIZE; level++ ) {
		size *= 2;
	}
	return level;
}

static int getBlock ( int index ){
	int level = getIndexLevel ( index );
	return ( index + 1 - ( 1 << level ) ) * ( ( MAX_MEM_SIZE / MIN_BLOCK_SIZE ) / ( 1 << level ) );
}


#endif