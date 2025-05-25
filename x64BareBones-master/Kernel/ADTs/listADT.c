#include <listADT.h>

typedef struct node {
    elemTypePtr head;
    struct node *tail;
} tNode;

typedef tNode *tList;

struct listCDT{
    tList first;
    tList last;
    size_t size;
    tCompare cmp;
    tList next;
};

listADT newList(tCompare cmp){
    listADT aux = allocMemory(sizeof(*aux));
    if(aux == NULL){
        return NULL;
    }
    aux->size = 0;
    aux->first = NULL;
    aux->last = NULL;
    aux->cmp = cmp;
    return aux;
}

static void freeListRec(tList list) {
    if(list == NULL){
        return;
    }
    freeListRec(list->tail);
    freeMemory(list);
}

void freeList(listADT list){
    if(list == NULL){
        return;
    }
    freeListRec(list->first);
    freeMemory(list);
}

bool isEmptyList(const listADT list){
    return list == NULL || list->first == NULL;
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
    
    tList newNode = allocMemory(sizeof(*newNode));
    if (newNode == NULL){
        return -1;
    }

    newNode->head = elem;
    newNode->tail = NULL;

    if(list->first == NULL){
        list->fist = newNode;
        list->last = newNode;
    } else {
        list->last->tail = newNode;
        list->last = newNode;
    }

    list->size++;
    return 0;
}

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
}

static elemTypePtr getRec(tList l, size_t idx){
    if (idx == 0){
        return l->head;
    }
    return getRec(l->tail, idx-1);
}

elemTypePtr get(const listADT list, size_t idx){
    if(list == NULL || idx >= list->size){
        return NULL;
    }
    retutn getRec(list->first, idx);
}

int startByFirst(listADT list){
    if(list == NULL){
        return -1;
    }
    list->next = list->first;
    return 0;
}

bool hasNext(const listADT list){
    return list != NULL && list->next != NULL;
}

elemTypePtr next(listADT list){
    if(!hasNext(list)){
        return NULL;
    }
    elemTypePtr aux = list->next->head;
    list->next = list->next->tail;
    return aux;
}

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
}

int deleteList(listADT list, elemTypePtr elem){
    if(list == NULL){
        return -1;
    }
    int flag;
    list->first = deleteRec(list->first, elem, list->cmp, &flag);
    list->size -= flag;
    return flag;
}

