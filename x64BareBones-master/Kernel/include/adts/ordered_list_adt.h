
//listas ordenadas sin repetidos de cualquier tipo
#ifndef _ordered_list_adt_H
#define _ordered_list_adt_H
#include <memory_manager.h>
#include <PCB.h>
#include <kernel.h>
typedef struct listCDT * orderedListADT;
typedef PCB * elemType;



typedef int ( *tCompare ) ( elemType e1, elemType e2 );

orderedListADT new_ordered_list ( tCompare cmp );

void free_ordered_list ( orderedListADT list );

int add_ordered_list ( orderedListADT list, elemType elem ); // retorna 1 si lo inserto, 0 si no.

int delete_ordered_list ( orderedListADT list, elemType elem );

size_t size_ordered_list ( const orderedListADT list );


int is_empty_ordered_list ( const orderedListADT list );


elemType get ( const orderedListADT list, size_t idx );

//le avisamos que vamos a recorrer la lista desde el primero
void ordered_list_to_begin ( orderedListADT list );

//retorna true si hay pendientes que recorrer
int ordered_list_has_next ( const orderedListADT list );

//me devuelve el siguiente. precondicion, has_next debe devolver true
elemType ordered_list_next ( orderedListADT list );
int ordered_list_delete_current ( orderedListADT list );

#endif