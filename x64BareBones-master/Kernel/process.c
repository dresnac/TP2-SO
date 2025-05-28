#include <process.h>

#define STACK_SIZE 256

#define PCB_AMOUNT 100

PCB pcb_array[PCB_AMOUNT] = {0};

PCB * running;

static int64_t findFreePcb();
//uint64_t blockProcess();
//uint64_t readyProcess();

int64_t newProcess(uint64_t rip, tPriority priority){

    uint64_t rsp = (uint64_t) allocMemory(STACK_SIZE*4) + STACK_SIZE*4; //falta pasarle el mm
    
    if(rsp == STACK_SIZE*4){
        return -1;
    }
    rsp =loadStack(rip, rsp);

    int64_t pid = findFreePcb();

    if (pid == -1){
        return -1;
    }

    pcb_array[pid].pid = pid;
    pcb_array[pid].ppid = running->pid;
    pcb_array[pid].rsp = rsp;
    pcb_array[pid].status = READY;

    ready(&pcb_array[pid]);
    return pid;
}

static int64_t findFreePcb(){
    int64_t to_return = 0;
    while(pcb_array[to_return].status != FREE && to_return < PCB_AMOUNT){
        to_return++;
    }
    return to_return;
}

PCB * getPcb(int64_t pid){
    if(pid >= PCB_AMOUNT || pid < 0){
        return NULL;
    }
    return &pcb_array[pid];
}

int64_t killProcess(int64_t pid){
    if(pid >= PCB_AMOUNT || pid<0 || pcb_array[pid].status == FREE){
        return -1;
    }
    unschedule(&pcb_array[pid]);
    pcb_array[pid].status = ZOMBIE;
}