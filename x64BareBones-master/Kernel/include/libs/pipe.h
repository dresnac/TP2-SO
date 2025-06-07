#ifndef _PIPE_H
#define _PIPE_H

#include <stdint.h>
#include <semaphores.h>
#include <shared_defs.h>

#define PIPE_BUFFER_SIZE 2048
#define AMOUNT_OF_PIPES 100


void pipe_init();
int64_t pipe_open ( int64_t id, tPipeMode mode );
int64_t pipe_read ( int64_t id, uint8_t * buffer, uint64_t amount );
int64_t pipe_write ( int64_t id, uint8_t * buffer, uint64_t amount, tPid pid );
int64_t pipe_close ( int64_t id, tPid pid );
int64_t pipe_open_free ( tPipeMode mode );
tPid pipe_get_pid ( int64_t id, tPipeMode mode );
tPid pipe_reserve();
int64_t pipe_open_pid ( int64_t id, tPipeMode mode, tPid pid );

#endif