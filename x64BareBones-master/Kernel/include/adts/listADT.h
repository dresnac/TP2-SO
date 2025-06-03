#ifndef _LISTADT_H_
#define _LISTADT_H_

#include <stddef.h>
#include <assert.h>
#include <process.h>
#include <memoryManager.h>
#include <PCB.h>

typedef struct listCDT * listADT;
typedef PCB * elemTypePtr;


typedef int (*tCompare)(elemTypePtr e1, elemTypePtr e2);

listADT newList(tCompare cmp);

void freeList(listADT list);

int addList(listADT list, elemTypePtr elem);

int deleteList(listADT list, elemTypePtr elem);

int sizeList(const listADT list);

int isEmptyList(const listADT list);

elemTypePtr next(listADT list);

#endif