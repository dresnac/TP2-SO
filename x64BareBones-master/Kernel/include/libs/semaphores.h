#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#define SEM_AMOUNT_USER 100
#define KERNEL_SEM_ID_LIMIT 350

#include <queueADT.h>
#include <shared_defs.h>
#include <scheduler.h>
#include <process.h>
#include <memoryManager.h>

void acquire ( uint8_t * lock );
void release ( uint8_t * lock );
int64_t mySemOpen ( int64_t sem_id, int value, uint8_t is_kernel );
int64_t mySemPost ( int64_t sem_id, uint8_t is_kernel );
int64_t mySemWait ( int64_t sem_id, uint8_t is_kernel );
int64_t mySemClose ( int64_t sem_id, uint8_t is_kernel );
int64_t semPostIfValueIsZero ( int64_t sem_id, uint8_t is_kernel );
int64_t mySemOpenGetId ( int value );
int64_t mySemPostNoYield ( int sem_id );
int64_t semDeleteFromBlockedQueue ( PCB * pcb );
#endif
