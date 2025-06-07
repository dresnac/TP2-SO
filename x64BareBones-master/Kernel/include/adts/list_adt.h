#ifndef _LISTADT_H_
#define _LISTADT_H_


#include <stddef.h>
#include <assert.h>
#include <memory_manager.h>
#include <process.h>
#include <PCB.h>

typedef struct listCDT * listADT;
typedef PCB * elemTypePtr;




typedef int ( *tCompare ) ( elemTypePtr e1, elemTypePtr e2 );

listADT new_list ( tCompare cmp );

void free_list ( listADT list );

int add_list ( listADT list, elemTypePtr elem );

int delete_list ( listADT list, elemTypePtr elem );

int size_list ( const listADT list );

int is_empty_list ( const listADT list );

elemTypePtr next ( listADT list );



#endif