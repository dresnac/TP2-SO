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
    //aca falta algo
    
    for(int i=0;i<3; i++){
        pcb_array[pid].fds[i] = fds ? fds[i] : -1;
    }
    
    if (setupPipe(pid, fds) == -1){
        freeMemory(getKernelMem(), (void*) rsp_malloc);
        for(uint64_t i=0; i<pcb_array[pid].cant; i++){
            freeMemory(getKernelMem(), (void*) pcb_array[pid].args);
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

PCB * getPcb(int64_t pid){
    if(pid >= PCB_AMOUNT || pid < 0){
        return NULL;
    }
    return &pcb_array[pid];
}

static int64_t setFreePid ( tPid pid )
{
	PCB * process = getPcb ( pid );
	return 0;//setFreePcb ( process );
}

//int64_t killProcessPcb(PCB * pcb) que killProcess llame a esta

//actualizar
int64_t killProcess(tPid pid){
    if(pid >= PCB_AMOUNT || pid<0 || pcb_array[pid].status == FREE){
        return -1;
    }
    unschedule(&pcb_array[pid]);
    pcb_array[pid].status = ZOMBIE;
}
