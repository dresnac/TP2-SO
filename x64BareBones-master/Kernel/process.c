#include <process.h>

#define STACK_SIZE 256

#define PCB_AMOUNT 100

PCB pcb_array[PCB_AMOUNT] = {0};

PCB * running;

int64_t findFreePcb();
uint64_t blockProcess();
uint64_t readyProcess();

int64_t newProcess(uint64_t rip, int priority){
    uint64_t rsp = (uint64_t) allocMemory(STACK_SIZE) + STACK_SIZE; //falta pasarle el mm
    rsp =loadStack(rip, rsp);

    int64_t pid = findFreePcb();
    if(pid == -1){
        return -1;
    }

    pcb_array[pid].pid = pid;
    pcb_array[pid].ppid = running->pid;
    pcb_array[pid].rsp = rsp;
    pcb_array[pid].status = READY;
    return pid;
}

int64_t findFreePcb(){
    int64_t to_return = 0;
    while(pcb_array[to_return].status != FREE && to_return < PCB_AMOUNT){
        to_return++;
    }
    return to_return;
}