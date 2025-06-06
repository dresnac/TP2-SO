#include <scheduler.h>

static int initialized = 0;

static listADT ready_list;
static listADT blocked_list;

static PCB * running = NULL;
static PCB * idle_pcb;
static uint64_t ran_counter = 0;
static PCB * shell_pcb;

int compareElements(elemTypePtr e1, elemTypePtr e2){
    return e1 - e2;
}

void initializeScheduler(tPid shell_process_pid, tPid idle_process_pid){
    tCompare cmp = compareElements;
    ready_list = newList(cmp);
    blocked_list = newList(cmp);
    idle_pcb = getPcb(idle_process_pid);
    shell_pcb = getPcb(shell_process_pid);

    //deleteList(ready_list, idle_pcb);
    initialized = 1;

    ready (shell_pcb);
    return;
}

void ready(PCB * process){
    if(process == NULL){
        return;
    }
    process->status = READY;
    addList(ready_list, process);
    deleteList(blocked_list, process);
}

PCB * getIdlePcb(){
    return idle_pcb;
}

PCB * getShellPcb(){
    return shell_pcb;
}

void setRunningNull(){
    running = NULL;
}

void blockCurrentNoYield(){
    PCB * process = running;
    if(process == NULL){
        return;
    }
    process->status = BLOCKED;
    deleteList( ready_list, process );
    addList ( blocked_list, process );
}

void block(PCB * process){
    if (process == NULL){
        return;
    }
    process->status = BLOCKED;
    deleteList(ready_list, process);
    addList(blocked_list, process);
    if (process == running){
        schedulerYield();
    }
}

void blockCurrent(){
    block(running);
}

void unblockWaitingMe(){
    PCB * pcb = running;
    if(pcb == NULL || pcb->waiting_me == NULL || pcb->waiting_me->status == ZOMBIE || pcb->waiting_me->status == FREE){
        return;
    }
    ready(pcb->waiting_me);
}

void unblockWaitingPid ( tPid pid )
{
	PCB * pcb = getPcb ( pid );
	unblockWaitingPcb ( pcb );
}

void unblockWaitingPcb ( PCB * pcb )
{
	if ( pcb == NULL || pcb->waiting_me == NULL || pcb->waiting_me->status == ZOMBIE || pcb->waiting_me->status == FREE ) {
		return;
	}
	ready ( pcb->waiting_me );
}

PCB * getRunning()
{
	return running;
}

void unschedule(PCB * process){
    if(process->status == READY){
        deleteList(ready_list, process);
    }else if(process->status == BLOCKED){
        deleteList(blocked_list, process);
    }
}

void schedulerYield()
{
	if ( ! ( running == NULL || isEmptyList ( ready_list ) ) ) {
		ran_counter = running->priority;
	}
	timer_tick();
}

uint64_t getPid(){
    return running->pid;
}


uint64_t blockArbitrary(uint64_t pid){
    PCB * process = getPcb(pid);
    if(process == NULL){
        return -1;
    }
    block(process);
    return 0;
}

uint64_t unblockArbitrary(uint64_t pid){
    PCB * process = getPcb(pid);
    if(process == NULL){
        return -1;
    }
    ready(process);
    return 0;
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
    if(running == NULL){
        running = next (ready_list);
        return running->rsp;
    }
    if(ran_counter >= running->priority || running->status == ZOMBIE){
        PCB * next_pcb = next(ready_list);
        ran_counter = 0;
        running = next_pcb;
        return next_pcb->rsp;
    }
    ran_counter++;
    return current_rsp;
}

int64_t nice ( tPid pid, uint64_t new_prio )
{
	PCB * process = getPcb ( pid );
	if ( process == NULL || process->status == FREE ) {
		return -1;
	}
	process->priority = new_prio;
	return 0;
}
