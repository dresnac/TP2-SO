#include <ordered_list_adt.h>

typedef struct node {
	elemType head;
	struct node * tail;
} tNode;

typedef tNode * tOrderedList;

struct listCDT {
	tOrderedList first;
	size_t size;
	tCompare cmp;
	tOrderedList next;
	tOrderedList current;
	tOrderedList previous;
};


orderedListADT new_ordered_list ( tCompare cmp )
{
	orderedListADT aux = alloc_memory ( sizeof ( *aux ), get_kernel_mem() );
	if ( aux == NULL ) {
		return NULL;
	}
	aux->size = 0;
	aux->first = NULL;
	aux->cmp = cmp;
	return aux;
}

static void free_ordered_list_rec ( tOrderedList list )
{
	if ( list == NULL ) {
		return;
	}
	free_ordered_list_rec ( list->tail );
	free_memory ( list, get_kernel_mem() );
}

void free_ordered_list ( orderedListADT list )
{
	free_ordered_list_rec ( list->first );
	free_memory ( list, get_kernel_mem() );
}

int is_empty_ordered_list ( const orderedListADT list )
{
	return list->first == NULL;
}

size_t size_ordered_list ( const orderedListADT list )
{
	return list->size;
}




static tOrderedList add_ordered_list_rec ( tOrderedList list, elemType elem, tCompare cmp, int * flag )
{
	int c;
	if ( list == NULL || ( c = cmp ( elem, list->head ) ) < 0 ) {
		tOrderedList aux = alloc_memory ( sizeof ( *aux ), get_kernel_mem() );
		if ( aux == NULL ) {
			*flag = 0;
			return list;
		}
		aux->head = elem;
		aux->tail = list;
		*flag = 1;
		return aux;
	}
	if ( c > 0 ) {
		list->tail = add_ordered_list_rec ( list->tail, elem, cmp, flag );
	}
	return list;
}

int add_ordered_list ( orderedListADT list, elemType elem )
{
	if ( list == NULL || elem == NULL ) {
		return -1;
	}
	int flag = 0;
	list->first = add_ordered_list_rec ( list->first, elem, list->cmp, &flag );
	list->size += flag;
	if ( flag == 0 ) {
		return -1;
	}
	return 0;
}

static elemType get_rec ( tOrderedList l, size_t idx )
{
	if ( idx == 0 ) {
		return l->head;
	}
	return get_rec ( l->tail, idx - 1 );
}

elemType get ( const orderedListADT list, size_t idx )
{
	if ( list == NULL || idx >= list->size ) {
		return NULL;
	}
	return get_rec ( list->first, idx );
}


static tOrderedList delete_rec ( tOrderedList list, elemType elem, tCompare cmp, int * flag )
{
	int c;
	if ( list == NULL || ( c = cmp ( list->head, elem ) ) > 0 ) {
		*flag = 0;
		return NULL;
	}
	if ( c < 0 ) {
		list->tail = delete_rec ( list->tail, elem, cmp, flag );
		return list;
	}
	*flag = 1;
	tOrderedList aux = list->tail;
	free_memory ( list, get_kernel_mem() );
	return aux;

}

int delete_ordered_list ( orderedListADT list, elemType elem )
{
	if ( list == NULL || list->first == NULL || elem == NULL ) {
		return -1;
	}
	int aux;
	list->first = delete_rec ( list->first, elem, list->cmp, &aux );
	list->size -= aux;
	return !aux;
}


void ordered_list_to_begin ( orderedListADT list )
{
	list->next = list->first;
	list->current = NULL;
	list->previous = NULL;
}


int ordered_list_has_next ( const orderedListADT list )
{
	return list->next != NULL;
}


elemType ordered_list_next ( orderedListADT list )
{
	if ( !ordered_list_has_next ( list ) ) {
		return NULL;
	}
	list->previous = list->current;
	list->current = list->next;
	list->next = list->next->tail;
	return list->current->head;
}

int ordered_list_delete_current ( orderedListADT list )
{
	if ( list->current == NULL ) {
		return -1;
	}

	if ( list->previous == NULL ) {
		list->first = list->next;
	} else {
		list->previous->tail = list->next;
	}

	free_memory ( list->current, get_kernel_mem() );
	list->size--;

	list->current = NULL;

	return 0;
}

