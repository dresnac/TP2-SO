#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define SEM_AMOUNT_USER 100
#define KERNEL_SEM_ID_LIMIT 350 //Kernel sems goes from ID SEM_AMOUNT_USER to KERNEL_SEM_AMOUT-1

#include <queue_adt.h>
#include <shared_defs.h>
#include <scheduler.h>
#include <process.h>
#include <memory_manager.h>

void acquire ( uint8_t * lock );
void release ( uint8_t * lock );
int64_t semOpen ( int64_t sem_id, int value, uint8_t is_kernel );
int64_t semPost ( int64_t sem_id, uint8_t is_kernel );
int64_t semWait ( int64_t sem_id, uint8_t is_kernel );
int64_t semClose ( int64_t sem_id, uint8_t is_kernel );
int64_t postIfValueIsZero ( int64_t sem_id, uint8_t is_kernel );
int64_t semOpenGetId ( int value );
int64_t semPostNoYield ( int sem_id );
int64_t semDeleteFromBlockedQueue ( PCB * pcb );
#endif
