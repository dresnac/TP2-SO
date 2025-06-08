#include <queue_adt.h>

typedef struct node {
	elemTypePtr head;
	struct node * tail;
} tNode;

typedef tNode * tQueue;

struct queueCDT {
	tQueue first;
	tQueue last;
	uint64_t size;
};

queueADT newQueue()
{
	queueADT to_return = allocMemory ( sizeof ( *to_return ), getKernelMem() );
	if ( to_return == NULL ) {
		return NULL;
	}
	to_return->size = 0;
	to_return->first = NULL;
	to_return->last = NULL;
	return to_return;
}

uint64_t queueIsEmpty ( queueADT queue )
{
	if ( queue == NULL ) {
		return 1;
	}
	return ( queue->size == 0 );
}

elemTypePtr dequeue ( queueADT queue )
{
	if ( queueIsEmpty ( queue ) ) {
		return NULL;
	}
	queue->size--;
	elemTypePtr to_return = queue->first->head;
	tQueue aux = queue->first->tail;
	freeMemory( queue->first, getKernelMem() );
	queue->first = aux;
	if ( aux == NULL ) {
		queue->last = NULL;
	}
	return to_return;
}

int64_t deleteFromQueue ( queueADT queue, elemTypePtr head )
{
	if ( queueIsEmpty ( queue ) ) {
		return -1;
	}

	tQueue current = queue->first;
	tQueue previous = NULL;

	while ( current != NULL && current->head != head ) {
		previous = current;
		current = current->tail;
	}

	if ( current == NULL ) {
		return -1;
	}

	if ( previous == NULL ) {
		queue->first = current->tail;
	} else {
		previous->tail = current->tail;
	}

	if ( current->tail == NULL ) {
		queue->last = previous;
	}

	freeMemory( current, getKernelMem() );
	queue->size--;

	return 0;
}




int64_t enqueue ( queueADT queue, elemTypePtr head )
{
	if ( queue == NULL ) {
		return -1;
	}
	tQueue new_node = allocMemory ( sizeof ( *new_node ),  getKernelMem() );
	if ( new_node == NULL ) {
		return -1;
	}
	new_node->head = head;
	new_node->tail = NULL;
	if ( queueIsEmpty ( queue ) ) {
		queue->first = new_node;
		queue->last = new_node;
		queue->size++;
		return 0;
	}
	queue->size++;
	queue->last->tail = new_node;
	queue->last = new_node;
	return 0;
}

void freeQueue ( queueADT queue )
{
	if ( queue == NULL ) {
		return;
	}
	while ( !queueIsEmpty ( queue ) ) {
		dequeue ( queue );
	}
	freeMemory ( queue, getKernelMem() );
}


