#include <orderedListADT.h>

typedef struct node{
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

ordered_list_adt newOrderedList(tCompare cmp){
    ordered_list_adt aux = allocMemory (getKernelMem(), sizeof ( *aux ));
	if ( aux == NULL ) {
		return NULL;
	}
    aux->size = 0;
	aux->first = NULL;
	aux->cmp = cmp;
	return aux;
}

static void freeOrderedListRec ( tOrderedList list )
{
	if ( list == NULL ) {
		return;
	}
	freeOrderedListRec ( list->tail );
	freeMemory (getKernelMem(), list);
}

void freeOrderedList ( ordered_list_adt list )
{
	freeOrderedListRec ( list->first );
	freeMemory (getKernelMem(), list);
}

int isEmptyOrderedList ( const ordered_list_adt list )
{
	return list->first == NULL;
}

size_t sizeOrderedList ( const ordered_list_adt list )
{
	return list->size;
}

static tOrderedList addOrderedListRec ( tOrderedList list, elemType elem, tCompare cmp, int * flag )
{
	int c;
	if ( list == NULL || ( c = cmp ( elem, list->head ) ) < 0 ) {
		tOrderedList aux = allocMemory (getKernelMem(), sizeof ( *aux ));
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
		list->tail = addOrderedListRec ( list->tail, elem, cmp, flag );
	}
    return list;
}
	
int addOrderedList ( ordered_list_adt list, elemType elem )
{
	if ( list == NULL || elem == NULL ) {
		return -1;
	}
	int flag = 0;
	list->first = addOrderedListRec ( list->first, elem, list->cmp, &flag );
	list->size += flag;
	if ( flag == 0 ) {
		return -1;
	}
	return 0;
}

static elemType getRec ( tOrderedList l, size_t idx )
{
	if ( idx == 0 ) {
		return l->head;
	}
	return getRec ( l->tail, idx - 1 );
}

elemType get ( const ordered_list_adt list, size_t idx )
{
	if ( list == NULL || idx >= list->size ) {
		return NULL;
	}
	return getRec ( list->first, idx );
}

static tOrderedList deleteRec ( tOrderedList list, elemType elem, tCompare cmp, int * flag )
{
	int c;
	if ( list == NULL || ( c = cmp ( list->head, elem ) ) > 0 ) {
		*flag = 0;
		return NULL;
	}
	if ( c < 0 ) {
		list->tail = deleteRec ( list->tail, elem, cmp, flag );
		return list;
	}
	*flag = 1;
	tOrderedList aux = list->tail;
	freeMemory(getKernelMem(),list);
	return aux;

}

int deleteOrderedList ( ordered_list_adt list, elemType elem )
{
	if ( list == NULL || list->first == NULL || elem == NULL ) {
		return -1;
	}
	int aux;
	list->first = deleteRec ( list->first, elem, list->cmp, &aux );
	list->size -= aux;
	return !aux;
}

void orderedListToBegin ( ordered_list_adt list )
{
	list->next = list->first;
	list->current = NULL;
	list->previous = NULL;
}

int orderedListHasNext ( const ordered_list_adt list )
{
	return list->next != NULL;
}

elemType orderedListNext ( ordered_list_adt list )
{
	if ( !orderedListHasNext ( list ) ) {
		return NULL;
	}
	list->previous = list->current;
	list->current = list->next;
	list->next = list->next->tail;
	return list->current->head;
}

int orderedListDeleteCurrent ( ordered_list_adt list )
{
	if ( list->current == NULL ) {
		return -1;
	}

	if ( list->previous == NULL ) {
		list->first = list->next;
	} else {
		list->previous->tail = list->next;
	}

	freeMemory (getKernelMem(), list->current);
	list->size--;

	list->current = NULL;

	return 0;
}