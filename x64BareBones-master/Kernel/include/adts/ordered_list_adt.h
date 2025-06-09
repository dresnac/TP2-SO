#ifndef _ordered_list_adt_H
#define _ordered_list_adt_H
#include <memory_manager.h>
#include <PCB.h>
#include <kernel.h>
typedef struct listCDT * orderedListADT;
typedef PCB * elemType;



typedef int ( *tCompare ) ( elemType e1, elemType e2 );

orderedListADT newOrderedList ( tCompare cmp );

void freeOrderedList ( orderedListADT list );

int addOrderedList ( orderedListADT list, elemType elem ); // retorna 1 si lo inserto, 0 si no.

int deleteOrderedList ( orderedListADT list, elemType elem );

size_t sizeOrderedList ( const orderedListADT list );


int isEmptyOrderedList ( const orderedListADT list );


elemType get ( const orderedListADT list, size_t idx );

void orderedListToBegin ( orderedListADT list );

int orderedListHasNext ( const orderedListADT list );

elemType orderedListNext ( orderedListADT list );
int orderedListDeleteCurrent ( orderedListADT list );

#endif