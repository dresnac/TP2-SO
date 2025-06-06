#ifndef _PIPE_H
#define _PIPE_H

#include <stdint.h>
#include <semaphore.h>
#include <shared_defs.h>

#define PIPE_BUFFER_SIZE 2048
#define AMOUNT_OF_PIPES 100

void pipeInit();
int64_t pipeOpen ( int64_t id, tPipeMode mode );
int64_t pipeRead ( int64_t id, uint8_t * buffer, uint64_t amount );
int64_t pipeWrite ( int64_t id, uint8_t * buffer, uint64_t amount, pid_t pid );
int64_t pipeClose ( int64_t id, tPid pid );
int64_t pipeOpenFree ( tPipeMode mode );
tPid pipeGetPid ( int64_t id, tPipeMode mode );
tPid pipeReserve();
int64_t pipeOpenPid ( int64_t id, tPipeMode mode, tPid pid );

#endif
