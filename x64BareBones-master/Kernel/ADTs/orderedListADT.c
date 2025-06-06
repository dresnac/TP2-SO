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