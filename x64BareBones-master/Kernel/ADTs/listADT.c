#include <listADT.h>
#include <kernel.h>

typedef struct node {
    elemTypePtr head;
    struct node *tail;
} tNode;

typedef tNode *tList;

struct listCDT{
    tList pre_next;
    size_t size;
    tCompare cmp;
    //tList next;
};

listADT newList(tCompare cmp){
    listADT aux = allocMemory(getKernelMem(),sizeof(*aux));
    if(aux == NULL){
        return NULL;
    }
    aux->size = 0;
    aux->pre_next = NULL;
    aux->cmp = cmp;
    return aux;
}

void freeList(listADT list){
    if(list == NULL){
        return;
    }
    for(int i=0; i<list->size; i++){
        tList aux = list->pre_next;
        list->pre_next = list->pre_next->tail;
        freeMemory(getKernelMem(),aux);
    }
    freeMemory(getKernelMem(),list);
}

int isEmptyList(const listADT list){
    return list == NULL || list->pre_next == NULL;
}

int sizeList(const listADT list){
    if(list == NULL){
        return -1;
    }
    return list->size;
}

int addList(listADT list, elemTypePtr elem){
    if (list == NULL){
        return -1;
    }
    
    tList newNode = allocMemory(getKernelMem(),sizeof(*newNode));
    if (newNode == NULL){
        return -1;
    }

    if(list->pre_next == NULL){
        list->pre_next = newNode;
        list->pre_next->tail = newNode;
        list->pre_next->head = elem;
    } else {
        newNode->head = list->pre_next->head;
        list->pre_next->head = elem;

        newNode->tail = list->pre_next->tail;
        list->pre_next->tail = newNode;
        list->pre_next = newNode;
    }

    list->size++;
    return 0;
}


elemTypePtr next(listADT list){
    if(isEmptyList(list)){
        return NULL;
    }

    elemTypePtr aux = list->pre_next->tail->head;
    list->pre_next = list->pre_next->tail;
    return aux;
}

int deleteList(listADT list, elemTypePtr elem){
    
    if(isEmptyList(list)){
        return -1;
    }
    
    if (list->size == 1){
        if (list->cmp(elem, list->pre_next->head) != 0){
            return -1;
        }
        freeMemory(getKernelMem(),list->pre_next);
        list->pre_next = NULL;
        list->size = 0;
        return 0;
    }

    tList pre = list->pre_next;
    tList current = list->pre_next->tail;
    for(int i=0; i<list->size; i++){
        if(list->cmp(elem, current->head) == 0){
            pre->tail = current->tail;
            if(list->pre_next == current){
                list->pre_next = pre;
            }
            freeMemory(getKernelMem(),current);
            list->size--;
            return 0;
        }
        pre = pre->tail;
        current = current->tail;
    }
    return -1;
}


