#ifndef _KERNEL_LIBC_
#define _KERNEL_LIBC_

#include <shared_defs.h>
#include <shared_libc.h>
#include <memory_manager.h>
#include <kernel.h>

char * new_str_copy ( char * string );
char ** copy_argv ( tPid pid, char ** argv, uint64_t argc );

#endif