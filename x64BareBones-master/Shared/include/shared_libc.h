#ifndef _SHARED_LIBC_H
#define _SHARED_LIBC_H
#include <stdint.h>
#include <shared_defs.h>

void * sharedLibcMemset ( void * destination, int32_t c, uint64_t length );
void * sharedLibcMemcpy ( void * destination, const void * source, uint64_t length );
uint64_t sharedLibcStrlen ( const char * str );

#endif