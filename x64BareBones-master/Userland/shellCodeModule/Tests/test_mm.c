#include <test_mm.h>


typedef struct MM_rq {
	void * address;
	uint64_t size;
} mm_rq;

int64_t test_mm ( char * argv[],  uint64_t argc )
{

	mm_rq mm_rqs[MAX_BLOCKS];
	uint64_t rq;
	uint64_t total;
	uint64_t max_memory;

	if ( argc != 2 ) {
		libcfPrintf ( STDERR, "Usage: testmm <max_memory>\n" );
		return -1;
	}

	int64_t satoi_flag;
	if ( ( max_memory = libcSatoi ( argv[1], &satoi_flag ) ) <= 0 ) {
		libcfPrintf ( STDERR, "Error: <max_memory> must be a positive integer.\n" );
		return -1;
	}


	while ( 1 ) {
		rq = 0;
		total = 0;

		// Request as many blocks as we can
		while ( rq < MAX_BLOCKS && total < max_memory ) {
			mm_rqs[rq].size = get_uniform ( max_memory - total - 1 ) + 1;
			mm_rqs[rq].address = libcMalloc ( mm_rqs[rq].size );
			if ( mm_rqs[rq].address ) {
				total += mm_rqs[rq].size;
				rq++;
			}
		}

		// Set
		uint32_t i;
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				sharedLibcMemset ( mm_rqs[i].address, i, mm_rqs[i].size );

		// Check
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				if ( !memcheck ( mm_rqs[i].address, i, mm_rqs[i].size ) ) {
					libcfPrintf ( STDERR, "ERROR: memcheck is not true\n" );
					return -1;
				}

		// Free
		for ( i = 0; i < rq; i++ )
			if ( mm_rqs[i].address )
				libcFree ( mm_rqs[i].address );
	}
	return 0;
}