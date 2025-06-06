#include <queueADT.h>

typedef struct node {
	elemTypePtr head;
	struct node * tail;
} tNode;

typedef tNode * tQueue;

struct queueCDT{
    tQueue first;
    tQueue last;
    uint64_t size;
};

queue_adt newQueue()
{
	queue_adt to_return = allocMemory (getKernelMem(), sizeof ( *to_return ));
	if ( to_return == NULL ) {
		return NULL;
	}
	to_return->size = 0;
	to_return->first = NULL;
	to_return->last = NULL;
	return to_return;
}

uint64_t queueIsEmpty ( queue_adt queue )
{
	if ( queue == NULL ) {
		return 1;
	}
	return ( queue->size == 0 );
}

elemTypePtr dequeue ( queue_adt queue )
{
	if ( queueIsEmpty ( queue ) ) {
		return NULL;
	}
	queue->size--;
	elemTypePtr to_return = queue->first->head;
	tQueue aux = queue->first->tail;
	freeMemory (getKernelMem(), queue->first);
	queue->first = aux;
	if ( aux == NULL ) {
		queue->last = NULL;
	}
	return to_return;
}

int64_t deleteFromQueue ( queue_adt queue, elemTypePtr head )
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

	freeMemory (getKernelMem(), current);
	queue->size--;

	return 0;
}

int64_t enqueue ( queue_adt queue, elemTypePtr head )
{
	if ( queue == NULL ) {
		return -1;
	}
	tQueue new_node = allocMemory (getKernelMem(), sizeof ( *new_node ));
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

void freeQueue ( queue_adt queue )
{
	if ( queue == NULL ) {
		return;
	}
	while ( !queueIsEmpty ( queue ) ) {
		dequeue ( queue );
	}
	freeMemory (getKernelMem(), queue);
}