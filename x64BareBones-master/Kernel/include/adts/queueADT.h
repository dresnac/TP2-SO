#ifndef _QUEUEADT_H_
#define _QUEUEADT_H_

#include <stddef.h>
#include <stdint.h>
#include <PCB.h>
#include <memoryManager.h>
#include <kernel.h>

typedef struct queueCDT * queue_adt;
typedef PCB * elemTypePtr;
typedef int ( *tCompare ) ( elemTypePtr e1, elemTypePtr e2 );
queue_adt newQueue();
uint64_t queueIsEmpty ( queue_adt queue );
elemTypePtr dequeue ( queue_adt queue );
int64_t enqueue ( queue_adt queue, elemTypePtr value );
void freeQueue ( queue_adt queue );
int64_t deleteFromQueue ( queue_adt queue, elemTypePtr head );

#endif