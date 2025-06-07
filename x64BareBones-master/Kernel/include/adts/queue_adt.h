#ifndef _QUEUEADT_H_
#define _QUEUEADT_H_

#include <stddef.h>
#include <stdint.h>
#include <PCB.h>
#include <memory_manager.h>
#include <kernel.h>

typedef struct queueCDT * queueADT;
typedef PCB * elemTypePtr;
typedef int ( *tCompare ) ( elemTypePtr e1, elemTypePtr e2 );
queueADT new_queue();
uint64_t queue_is_empty ( queueADT queue );
elemTypePtr dequeue ( queueADT queue );
int64_t enqueue ( queueADT queue, elemTypePtr value );
void free_queue ( queueADT queue );
int64_t delete_from_queue ( queueADT queue, elemTypePtr head );

#endif