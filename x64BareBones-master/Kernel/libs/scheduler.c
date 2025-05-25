#include <scheduler.h>

uint8_t initialized = 0;

static listADT ready_list;
static listADT blocked_list;
static PCB * running = NULL;

int compareElements(elemTypePtr e1, elemTypePtr e2){
    return e1 - e2;
}

uint8_t findFirstReady(PCB **next_process) {
    for (int i = 0; i < 2; i++) {
        while (hasNext(ready_list)) {
            PCB *process = next(ready_list);
            if (process->status == READY) {
                *next_process = process;
                return 1;
            }
        }
        startByFirst(ready_list); // reinicia solo si no se encontro
    }
    return 0;
}

void ready(PCB * process){
    process->status = READY;
    addList(ready_list, process);
    deleteList(blocked_list, process);
}

void block(PCB * process){
    process->status = BLOCKED;
    deleteList(ready_list, process);
    addList(blocked_list, process);
}

void initializeScheduler(){
    tCompare cmp = compareElements;
    ready_list = newList(cmp);
    blocked_list = newList(cmp);
    initialized = 1;
    return;
}

uint64_t scheduler(uint64_t current_rsp){

    uint64_t new_rsp;
    if(running != NULL){
        running->rsp = current_rsp;
    }
    new_rsp = next(ready_list)->rsp;
     return new_rsp;
}

//revisar
/*
    if(!initialized){
        initializeScheduler();
    }
    if(isEmptyList(ready_list)){
        return;
    }
    if(running == NULL){
        startByFirst(ready_list);
        running = next(ready_list);
        running->status = RUNNING;
        return;
    } else {
        if (running->status == RUNNING)
        {
            running->status = READY;
            addList(ready_list, running);
        } else if (running->status == BLOCKED){
            deleteList(ready_list, running);
            addList(blocked_list, running);
        }
    }

    PCB * next_process = NULL;

    if(findFirstReady(&next_process)){
        deleteList(ready_list, next_process);
        next_process->status = RUNNING;
        contextSwitch(running, next_process);
        running =  next_process;
    }
}
    */
