#ifndef _KERNEL_LIBC_
#define _KERNEL_LIBC_

#include <shared_defs.h>
#include <shared_libc.h>
#include <memory_manager.h>
#include <kernel.h>

char * newStrCopy ( char * string );
char ** copyArgv ( tPid pid, char ** argv, uint64_t argc );

#endif