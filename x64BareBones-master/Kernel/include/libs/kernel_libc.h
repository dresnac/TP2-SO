#ifndef _KERNEL_LIBC_
#define _KERNEL_LIBC_

#include <shared_defs.h>
#include <shared_libc.h>
#include <memoryManager.h>
#include <kernel.h>

char * newStrCpy ( char * string );
char ** cpyArgv ( tPid pid, char ** argv, uint64_t argc );

#endif