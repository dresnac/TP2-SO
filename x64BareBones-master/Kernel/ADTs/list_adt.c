#include <list_adt.h>

typedef struct node {
	elemTypePtr head;
	struct node * tail;
} tNode;

typedef tNode * tList;

struct listCDT {
	tList pre_next;
	size_t size;
	tCompare cmp;
};



listADT newList ( tCompare cmp )
{
	listADT aux = allocMemory ( sizeof ( *aux ), getKernelMem() );
	if ( aux == NULL ) {
		return NULL;
	}
	aux->size = 0;
	aux->pre_next = NULL;
	aux->cmp = cmp;
	return aux;
}


void freeList ( listADT list )
{
	if ( list == NULL ) {
		return;
	}
	for ( int i = 0; i < list->size; i++ ) {
		tList aux = list->pre_next;
		list->pre_next = list->pre_next->tail;
		freeMemory ( aux, getKernelMem() );
	}
	freeMemory ( list, getKernelMem() );
}

int isEmptyList ( const listADT list )
{
	return list == NULL || list->pre_next == NULL;
}

int sizeList ( const listADT list )
{
	return list ? (int)list->size : -1;
}

int addList ( listADT list, elemTypePtr elem )
{
	if ( list == NULL ) {
		return -1;
	}

	tList new_node = allocMemory ( sizeof ( *new_node ), getKernelMem() );
	if ( new_node == NULL ) {
		return -1;
	}


	if ( list->pre_next == NULL ) {
		list->pre_next = new_node;
		list->pre_next->tail = new_node;
		list->pre_next->head = elem;
	} else {
		new_node->head = list->pre_next->head;
		list->pre_next->head = elem;

		new_node->tail = list->pre_next->tail;
		list->pre_next->tail = new_node;
		list->pre_next = new_node;
	}



	list->size++;
	return 0;
}

elemTypePtr next ( listADT list )
{
	if ( isEmptyList ( list ) ) {
		return NULL;
	}

	elemTypePtr aux = list->pre_next->tail->head;
	list->pre_next = list->pre_next->tail;
	return aux;
}


int deleteList ( listADT list, elemTypePtr elem )
{
	if ( isEmptyList ( list ) ) {
		return -1;
	}
	if ( list->size == 1 ) {
		if ( list->cmp ( elem, list->pre_next->head ) != 0 ) {
			return -1;
		}
		freeMemory ( list->pre_next, getKernelMem() );
		list->pre_next = NULL;
		list->size = 0;
		return 0;
	}

	tList ant = list->pre_next;
	tList current = list->pre_next->tail;
	for ( int i = 0; i < list->size ; i++ ) {
		if ( list->cmp ( elem, current->head ) == 0 ) {
			ant->tail = current->tail;
			if ( list->pre_next == current ) {
				list->pre_next = ant;
			}
			freeMemory ( current, getKernelMem() );
			list->size -- ;
			return 0;
		}
		ant = ant->tail;
		current = current->tail;
	}
	return -1;
}
