#include <process.h>


PCB pcb_array[PCB_AMOUNT] = {0};
uint64_t cant_proc = 0;

static int64_t findFreePcb();
static int64_t setFreePid ( tPid pid );
static int64_t setFreePcb ( PCB * process );
static int64_t setupPipe (tPid pid, int64_t  fds[]);

int8_t getStatus(tPid pid){
    PCB * process = getPcb(pid);
    if(process==NULL){
        return -1;
    }
    return process->status;
}

tPid wait(tPid pid, int64_t * ret){
    PCB * pcb_to_wait = getPcb(pid);
    if((pcb_to_wait == NULL) || (pcb_to_wait->status == FREE) || ( pcb_to_wait->waiting_me != NULL ) || ( pid == getPid() ) ) {
        return -1;
    }
    if(!(pcb_to_wait->status == ZOMBIE)){
        PCB * running = getRunning();
        pcb_to_wait->waiting_me = running;
        running->waiting_for = pcb_to_wait;
        blockCurrent();
        running->waiting_for = NULL;
    }
    if(pcb_to_wait->status == ZOMBIE){
        return -1;
    }
    if(ret != NULL){
        *ret = pcb_to_wait->ret;
    }
    if(setFreePid(pid)!= -1){
        cant_proc--;
    }
    return pid;
}

//static char ** copyArgv(uint64_t pid, char**argv, uint64_t argc);
/* ya no va aca esta logica
static uint64_t newStrlen( const char *str){
    const char *s = str;
    while(*s){
        ++s;
    }
    return s-str;
}

static char ** copyArgv(uint64_t pid, char ** my_argv, uint64_t my_argc){
    
    if( (my_argc == 0 && my_argv != NULL) || (my_argc>0 && my_argv==NULL)){
        return NULL;
    }

    //pcb_array[pid].cant = my_argc;

    if(my_argc == 0){
        //pcb_array[pid].args = NULL;
        return NULL;
    }

    char ** resp = allocMemory(getKernelMem() ,sizeof(char*) * (my_argc+1));

    if(resp == NULL){
        return NULL;
    }

    for(uint64_t i=0; i < my_argc; i++){
        uint64_t len = newStrlen(my_argv[i])+1;
        char * p = allocMemory(getKernelMem(),len);
        if(p == NULL){
            for(uint64_t j=0; j<i; j++){
                freeMemory(getKernelMem(),resp[j]);
            }
            freeMemory(getKernelMem(),resp);
            return NULL;
        }
        memcpy(p, my_argv[i], len);
        resp[i] = p;
    }
    return resp;
}
*/

int64_t newProcess(main_function rip, tPriority priority,uint8_t killable, char ** my_argv, uint64_t my_argc, int64_t fds[]){

    if(((priority != LOW) && (priority != MEDIUM) && (priority != HIGH))){
        return -1;
    }

    tPid pid = findFreePcb();
    if (pid == -1){
        return -1;
    }

    uint64_t rsp_malloc = (uint64_t) allocMemory(getKernelMem(),STACK_SIZE);
    uint64_t rsp = rsp_malloc + STACK_SIZE;

    if((void*)rsp_malloc == NULL){
        return -1;
    }

    char ** args_cpy = cpyArgv(pid, my_argv, my_argc);
    if(my_argc>0 &&args_cpy == NULL){
        freeMemory(getKernelMem(),(void*)rsp_malloc);
        pcb_array[pid].status = FREE;
        return -1;
    }
    rsp = loadStack((uint64_t)rip, rsp, args_cpy, my_argc, pid);

    

    pcb_array[pid].pid = pid;
    pcb_array[pid].rsp = rsp;
    pcb_array[pid].status = READY;
    pcb_array[pid].args = args_cpy;
    pcb_array[pid].cant = my_argc;
    pcb_array[pid].priority = priority;
    pcb_array[pid].killable = killable;
    pcb_array[pid].waiting_me = NULL;
    pcb_array[pid].lowest_stack_address = rsp_malloc;
    pcb_array[pid].blocked_by_sem = -1;
    
    for(int i=0;i<3; i++){
        pcb_array[pid].fds[i] = fds ? fds[i] : -1;
    }
    
    if (setupPipe(pid, fds) == -1){
        freeMemory(getKernelMem(), (void*) rsp_malloc);
        for(uint64_t i=0; i<pcb_array[pid].cant; i++){
            freeMemory(getKernelMem(), (void*) pcb_array[pid].args[i]);
        }
        freeMemory(getKernelMem(), (void*) pcb_array[pid].args);
        pcb_array[pid].status = FREE;
        return -1;
    }

    ready(&pcb_array[pid]);
    cant_proc++;
    return pid;
}

static int64_t findFreePcb(){
    int64_t to_return = 0;
    while(pcb_array[to_return].status != FREE && to_return < PCB_AMOUNT){
        to_return++;
    }
    return to_return;
}

static int64_t setupPipe(tPid pid, int64_t fds[]){
    if(!fds){
        return 0;
    }
    for (int i=0; i<CANT_FDS; i++){
        if( fds[i]<= MAX_COMMON_FD){
            continue;
        }
        tPipeMode mode = i == STDIN ? READER : WRITER;
        if (pipeOpenPid(fds[i]-3, mode, pid) == -1){
            for(int j=0; j < i; j++){
                pipeClose(fds[i]-3,pid);
            }
            return -1;
        }
    }
    return 0;
}

PCB * getPcb(int64_t pid){
    if(pid >= PCB_AMOUNT || pid < 0){
        return NULL;
    }
    return &pcb_array[pid];
}

static int64_t setFreePcb (PCB * process){
    if(process == NULL || process->status == FREE){
        return -1;
    }
    closeFds(process);
    freeMemory(getKernelMem(), (void*)process->lowest_stack_address);
    if(process->args == NULL){
        process->status = FREE;
        return 0;
    }
    for(uint64_t i=0; i<process->cant; i++){
        freeMemory(getKernelMem(),(void *) process->args[i]);
    }
    freeMemory(getKernelMem(),(void *) process->args);
    process->status = FREE;
    return 0;
}

static int64_t setFreePid ( tPid pid )
{
	PCB * process = getPcb ( pid );
	return setFreePcb ( process );
}

int64_t killProcessPcb(PCB * pcb){
    if((pcb == NULL) || (pcb->status == FREE) || (!pcb->killable)){
        return -1;
    }
    unschedule(pcb);
    unblockWaitingPcb(pcb);
    if(pcb->waiting_for && pcb->waiting_for->waiting_me){ //feo
        pcb->waiting_for->waiting_me = NULL;
    }
    if(getKeyboardBlocked() == pcb){
        setKeyboardBlockedNull();
    }
    if(pcb->time != 0 || pcb->start != 0){
        unsleepKill(pcb);
    }
    
    semDeleteFromBlockedQueue ( pcb );
	if ( set_free_pcb ( pcb ) != -1 ) {
		cant_proc--;
	}
    if(pcb == getRunning()){
        setRunningNull();
        timer_tick();   //chequear
    }
    return 0;
}


int64_t killProcess(tPid pid){
    PCB * pcb = getPcb(pid);
    return killProcessPcb(pcb);
}

static int64_t isForeground(PCB * pcb){
    if(pcb == NULL){
        return 0;
    }
    PCB * shell_pcb = getShellPcb();
    if(shell_pcb == NULL){
        return 0;
    }
    PCB * fore1 = shell_pcb->waiting_for;
    if(fore1 == NULL){ //se pueden juntar??
        if(pcb == shell_pcb){
            return 1;
        }
        return 0;
    }
    if(fore1 == pcb){
        return 1;
    }

    PCB * fore2 = NULL;
    if (fore1->fds[STDIN] > MAX_COMMON_FD){
        fore2 = getPcb(pipeGetPid(fore1->fds[STDIN-3],WRITER));
    }
    if(fore2 != NULL && fore2 == pcb){
        return 1;
    }
    return 0;
}

void getProcessInfo(PCB * pcb, process_info * process){
    process->name = newStrCpy(pcb->args != NULL ? pcb->args[0] : NULL);
    process->pid = pcb->pid;
    process->stack_pointer = pcb->rsp;
    process->lowest_stack_address = pcb->lowest_stack_address;
    process->status = pcb->status;
    process->is_background = !isForeground(pcb);
    for(int i=0;i<3; i++){
        process->fds[i] = pcb->fds[i] ? pcb->fds[i] : -1;
    }
}

process_info_list * ps(){
    process_info_list * process_list = allocMemory(getKernelMem(),sizeof(process_info_list));
    if(process_list == NULL){
        return NULL;
    }
    process_list->amount_of_processes = cant_proc;
    process_info * processes = allocMemory(getKernelMem(),cant_proc*sizeof(process_info));
    if(processes == NULL){
        freeMemory(getKernelMem(),(void*)process_list);
        return NULL;
    }

    for(int i=0, found=0; i<PCB_AMOUNT && found <cant_proc; i++){
        if(pcb_array[i].status != FREE){
            getProcessInfo(&pcb_array[i], &processes[found]);
            found++;
        }
    }
    process_list->processes = processes;
    return process_list;
}

void freePs(process_info_list * ps){
    if(ps == NULL || ps->processes == NULL){
        return;
    }
    for( int i=0; i< ps->amount_of_processes; i++){
        if(ps->processes[i].name){
            freeMemory(getKernelMem(),ps->processes[i].name);
        }
    }
    freeMemory(getKernelMem(), ps->processes);
    freeMemory(getKernelMem(),ps);
}

void closeFds(PCB * pcb){
    if (pcb == NULL){
        return;
    }
    for ( int i = 0; i < CANT_FDS ; i++ ) {
		if ( pcb->fds[i] > MAX_COMMON_FD ) {
			pipeClose ( pcb->fds[i] - CANT_FDS, pcb->pid );
		}
	}
}

int64_t makeMeZombie (int64_t ret){
    PCB * pcb = getRunning();
    if((pcb==NULL) || (pcb->status == FREE)){
        return -1;
    }
    pcb->ret = ret;
    unschedule(pcb);
    pcb->status = ZOMBIE;
    unblockWaitingMe();
    closeFds(pcb);
    return 0;
}

void ctrlcHandler()
{
	PCB * shell_pcb = getShellPcb();
	PCB * foreground_process;
	if ( shell_pcb == NULL || ( ( foreground_process = shell_pcb->waiting_for ) == NULL ) ) {
		return;
	}
	// first, we wait on the right side of the pipe.
	PCB * other_process_in_pipe = NULL;
	if ( foreground_process->fds[STDIN] > MAX_COMMON_FD ) {
		other_process_in_pipe = getPcb ( pipeGetPid ( foreground_process->fds[STDIN] - 3, WRITER ) );
	}
	killProcessPcb ( foreground_process );
	killProcessPcb ( other_process_in_pipe );
	return;
}