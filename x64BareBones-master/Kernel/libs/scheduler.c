#include <listADT.h>
uint8_t initialized = 0;

static listADT ready_list;
static listCDT blocked_list;

int compareElements(elemTypePtr e1, elemTypePtr e2){
    return e1->pid - e2->pid;
}

void scheduler(){
    if(!initialized){
        tCompare cmp = compareElements;
        ready_list = newList(cmp);
        blocked_list = newList(cmp);
        initialized=1;
    }
}
