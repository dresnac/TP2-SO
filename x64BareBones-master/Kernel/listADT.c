#include <listADT.h>
#include <kernel.h>

typedef struct node {
    elemTypePtr head;
    struct node *tail;
} tNode;

typedef tNode *tList;

struct listCDT{
    //tList first;
    tList pre_next;
    //tList last;
    size_t size;
    tCompare cmp;
    tList next;
};

listADT newList(tCompare cmp){
    listADT aux = allocMemory(getKernelMem(),sizeof(*aux));
    if(aux == NULL){
        return NULL;
    }
    aux->size = 0;
    //aux->first = NULL;
    //aux->last = NULL;
    aux->pre_next = NULL;
    aux->cmp = cmp;
    return aux;
}
/*
static void freeListRec(tList list) {
    if(list == NULL){
        return;
    }
    freeListRec(list->tail);
    freeMemory(list);
}*/

void freeList(listADT list){
    if(list == NULL){
        return;
    }
    //freeListRec(list->first);
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

    newNode->head = elem;

    if(list->pre_next == NULL){
        list->pre_next = newNode;
        list->pre_next->tail = newNode;
    } else {
        newNode->tail = list->pre_next->tail;
        list->pre_next->tail = newNode;
        list->pre_next = newNode;
    }

    list->size++;
    return 0;
}
/*
static int belongsRec(tList l, elemTypePtr elem, tCompare cmp) {
    if(l == NULL){
        return 0;
    }

    if(cmp(elem, l->head)==0){
        return 1;
    }
    return belongsRec(l->tail, elem, cmp);
}

int belongsList(const listADT list, elemTypePtr elem){
    if(list == NULL){
        return -1;
    }
    return belongsRec(list->first, elem, list->cmp);
}*/

/*
static elemTypePtr getRec(tList l, size_t idx){
    if (idx == 0){
        return l->head;
    }
    return getRec(l->tail, idx-1);
}*/
/*
elemTypePtr get(const listADT list, size_t idx){
    if(list == NULL || idx >= list->size){
        return NULL;
    }
    return getRec(list->first, idx);
}*/

elemTypePtr next(listADT list){
    if(isEmptyList(list)){
        return NULL;
    }

    elemTypePtr aux = list->pre_next->tail->head;
    list->pre_next = list->pre_next->tail;
    return aux;
}

/*
int startByFirst(listADT list){
    if(list == NULL){
        return -1;
    }
    list->next = list->first;
    return 0;
}*/
/*
int hasNext(const listADT list){
    return list != NULL && list->next != NULL;
}*/
/*
elemTypePtr next(listADT list){
    if(!hasNext(list)){
        return NULL;
    }
    elemTypePtr aux = list->next->head;
    list->next = list->next->tail;
    return aux;
}*/
/*
static tList deleteRec(tList list, elemTypePtr elem, tCompare cmp, int *flag){
    if(list == NULL){
        *flag = 0;
        return NULL;
    }
    if(cmp(elem, list->head) == 0){
        *flag = 1;
        tList aux = list->tail;
        freeMemory(list);
        return aux;
    }
    list->tail = deleteRec(list->tail, elem, cmp, flag);
    return list;
}*/

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


