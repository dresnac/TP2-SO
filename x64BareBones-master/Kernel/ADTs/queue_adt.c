// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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

queueADT new_queue()
{
	queueADT to_return = alloc_memory ( sizeof ( *to_return ), get_kernel_mem() );
	if ( to_return == NULL ) {
		return NULL;
	}
	to_return->size = 0;
	to_return->first = NULL;
	to_return->last = NULL;
	return to_return;
}

uint64_t queue_is_empty ( queueADT queue )
{
	if ( queue == NULL ) {
		return 1;
	}
	return ( queue->size == 0 );
}

elemTypePtr dequeue ( queueADT queue )
{
	if ( queue_is_empty ( queue ) ) {
		return NULL;
	}
	queue->size--;
	elemTypePtr to_return = queue->first->head;
	tQueue aux = queue->first->tail;
	free_memory( queue->first, get_kernel_mem() );
	queue->first = aux;
	if ( aux == NULL ) {
		queue->last = NULL;
	}
	return to_return;
}

int64_t delete_from_queue ( queueADT queue, elemTypePtr head )
{
	if ( queue_is_empty ( queue ) ) {
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

	free_memory( current, get_kernel_mem() );
	queue->size--;

	return 0;
}




int64_t enqueue ( queueADT queue, elemTypePtr head )
{
	if ( queue == NULL ) {
		return -1;
	}
	tQueue new_node = alloc_memory ( sizeof ( *new_node ),  get_kernel_mem() );
	if ( new_node == NULL ) {
		return -1;
	}
	new_node->head = head;
	new_node->tail = NULL;
	if ( queue_is_empty ( queue ) ) {
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

void free_queue ( queueADT queue )
{
	if ( queue == NULL ) {
		return;
	}
	while ( !queue_is_empty ( queue ) ) {
		dequeue ( queue );
	}
	free_memory ( queue, get_kernel_mem() );
}


