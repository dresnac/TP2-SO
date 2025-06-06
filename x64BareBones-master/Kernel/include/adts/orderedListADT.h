#ifndef _ORDERED_LIST_ADT_H
#define _ORDERED_LIST_ADT_H
#include <memoryManager.h>
#include <PCB.h>
#include <kernel.h>

typedef struct listCDT * ordered_list_adt;
typedef PCB * elemType;

typedef int (*tCompare)(elemType e1, elemType e2);

ordered_list_adt newOrderedList(tCompare cmp);
void freeOrderedList( ordered_list_adt list);
int addOrderedList(ordered_list_adt list, elemType elem);
int deleteOrderedList(ordered_list_adt list, elemType elem);
size_t sizeOrderedList (const ordered_list_adt list);
int isEmptyOrderedList(const ordered_list_adt list);
elemType get(const ordered_list_adt list, size_t idx);
void orderedListToBegin(ordered_list_adt list);
int orderedListHasNext(const ordered_list_adt list);
elemType orderedListNext(ordered_list_adt list);
int orderedListDeleteCurrent(ordered_list_adt list);

#endif