#include <stdint.h>

extern char bss;
extern char endOfBinary;

int main();

static void * loaderMemset ( void * destiny, int32_t c, uint64_t length );

int _start()
{
	//Clean BSS
	loaderMemset ( &bss, 0, &endOfBinary - &bss );

	return main();

}


void * loaderMemset ( void * destiation, int32_t c, uint64_t length )
{
	uint8_t chr = ( uint8_t ) c;
	char * dst = ( char * ) destiation;

	while ( length-- )
		dst[length] = chr;

	return destiation;
}
