#ifndef _LISTADT_H_
#define _LISTADT_H_

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <process.h>
#include <memoryManager.h>

typedef struct listCDT * listADT;
typedef PCB * elemTypePtr;
//typedef int * PCB;

/*
    <0 -> e1 antes que e2 
    >o -> e1 despues que e2
    0 -> iguales
*/

typedef int (*tCompare)(elemTypePtr e1, elemTypePtr e2);

listADT newList(tCompare cmp);

void freeList(listADT list);

int addList(listADT list, elemTypePtr elem);

int deleteList(listADT list, elemTypePtr elem);

int sizeList(const listADT list);

int isEmptyList(const listADT list);

//int belongsList(const listADT list, elemTypePtr elem);

//elemTypePtr get(const listADT list, size_t idx);

//int startByFirst(listADT list);

//int hasNext(const listADT list);

elemTypePtr next(listADT list);

#endif