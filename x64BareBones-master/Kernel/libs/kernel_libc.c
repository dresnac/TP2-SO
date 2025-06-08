#include <kernel_libc.h>


char * newStrCopy ( char * string )
{
	if ( string == NULL ) {
		return NULL;
	}
	uint64_t len = sharedLibcStrlen ( string ) + 1;
	char * copy = allocMemory ( len, getKernelMem() );
	if ( copy == NULL ) {
		return NULL;
	}
	sharedLibcMemcpy ( copy, string, len );
	return copy;
}

char ** copyArgv ( tPid pid, char ** argv, uint64_t argc )
{
	if ( argc == 0 || argv == NULL ) {
		return NULL;
	}

	char ** ans = allocMemory ( sizeof ( char * ) * ( argc + 1 ), getKernelMem() );

	if ( ans == NULL ) {
		return NULL;
	}

	for ( uint64_t i = 0; i < argc; i++ ) {
		ans[i] = newStrCopy ( argv[i] );
		if ( ans[i] == NULL ) {
			for ( uint64_t j = 0; j < i; j++ ) {
				freeMemory ( ( void * ) ans[j], getKernelMem() );
			}
			freeMemory ( ( void * ) ans, getKernelMem() );
			return NULL;
		}
	}
	return ans;
}

