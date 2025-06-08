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



listADT new_list ( tCompare cmp )
{
	listADT aux = alloc_memory ( sizeof ( *aux ), get_kernel_mem() );
	if ( aux == NULL ) {
		return NULL;
	}
	aux->size = 0;
	aux->pre_next = NULL;
	aux->cmp = cmp;
	return aux;
}


void free_list ( listADT list )
{
	if ( list == NULL ) {
		return;
	}
	for ( int i = 0; i < list->size; i++ ) {
		tList aux = list->pre_next;
		list->pre_next = list->pre_next->tail;
		free_memory ( aux, get_kernel_mem() );
	}
	free_memory ( list, get_kernel_mem() );
}

int is_empty_list ( const listADT list )
{
	return list == NULL || list->pre_next == NULL;
}

int size_list ( const listADT list )
{
	if ( list == NULL ) {
		return -1;
	}
	return list->size;
}

int add_list ( listADT list, elemTypePtr elem )
{
	if ( list == NULL ) {
		return -1;
	}

	tList new_node = alloc_memory ( sizeof ( *new_node ), get_kernel_mem() );
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
	if ( is_empty_list ( list ) ) {
		return NULL;
	}

	elemTypePtr aux = list->pre_next->tail->head;
	list->pre_next = list->pre_next->tail;
	return aux;
}


int delete_list ( listADT list, elemTypePtr elem )
{
	if ( is_empty_list ( list ) ) {
		return -1;
	}
	if ( list->size == 1 ) {
		if ( list->cmp ( elem, list->pre_next->head ) != 0 ) {
			return -1;
		}
		free_memory ( list->pre_next, get_kernel_mem() );
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
			free_memory ( current, get_kernel_mem() );
			list->size -- ;
			return 0;
		}
		ant = ant->tail;
		current = current->tail;
	}
	return -1;
}
