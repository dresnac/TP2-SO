#include <scheduler.h>

static int initialized = 0;

static listADT ready_list;
static listADT blocked_list;
static PCB * running = NULL;
static PCB * idle_pcb;
static uint64_t ran_counter = 0;


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

void initializeScheduler(int64_t idle_process_pid){
    tCompare cmp = compareElements;
    ready_list = newList(cmp);
    blocked_list = newList(cmp);
    idle_pcb = getPcb(idle_process_pid);
    deleteList(ready_list, idle_pcb);
    initialized = 1;
    return;
}

uint64_t scheduler(uint64_t current_rsp){

    if(!initialized){
        return current_rsp;
    }
    if(running != NULL){
        running->rsp = current_rsp;
    }
    if(isEmptyList(ready_list)){
        running = idle_pcb;
        return idle_pcb->rsp;
    }
    if(ran_counter >= running->priority){
        PCB * next_pcb = next(ready_list);
        running = next_pcb;
        return next_pcb->rsp;
    }
    ran_counter++;
    return current_rsp
}

void unschedule(PCB * process){
    if(process->status == READY){
        deleteList(ready_list, process);
    }else if(process->status == BLOCKED){
        deleteList(blocked_list, process);
    }
}

void yield(PCB * process){
    PCB * next_pcb = next(ready_list);
    ran_counter = 0;
    running = next_pcb;
}

uint64_t getPid(){
    return running->pid;
}

void blockArbitrary(uint64_t pid){
    PCB * process = getPcb(pid);
    block(process);
}

void unblockArbitrary(uint64_t pid){
    PCB * process = getPcb(pid);
    ready(process);
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
