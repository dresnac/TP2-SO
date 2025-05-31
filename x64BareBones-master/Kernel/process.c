#include <process.h>
#include <kernel.h>

#define STACK_SIZE 256

#define PCB_AMOUNT 100

PCB pcb_array[PCB_AMOUNT] = {0};

PCB * running;

static int64_t findFreePcb();

static char ** copyArgv(uint64_t pid, char**argv, uint64_t argc);

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


int64_t newProcess(main_function rip, tPriority priority, char ** my_argv, uint64_t my_argc){

    //feo cambiar
    if(!((priority != LOW) || (priority != MEDIUM) || (priority != HIGH))){
        return;
    }

    int64_t pid = findFreePcb();

    if (pid == -1){
        return -1;
    }

    uint64_t rsp_malloc = (uint64_t) allocMemory(getKernelMem(),STACK_SIZE*4); //falta pasarle el mm
    uint64_t rsp = rsp_malloc + STACK_SIZE*4;

    if(rsp_malloc == NULL){
        return -1;
    }
    /*
    if(rsp == STACK_SIZE*4){
        return -1;
    }*/

    char ** args_cpy = copyArgv(pid, my_argv, my_argc);
    if(my_argc>0 &&args_cpy == NULL){
        freeMemory(getKernelMem(),(void*)rsp_malloc);
        pcb_array[pid].status = FREE;
        return -1;
    }
    rsp = loadStack(rip, rsp, args_cpy, my_argc, pid);

    

    pcb_array[pid].pid = pid;
    pcb_array[pid].ppid = running->pid;
    pcb_array[pid].rsp = rsp;
    pcb_array[pid].status = READY;
    pcb_array[pid].args = args_cpy;
    pcb_array[pid].cant = my_argc;
    pcb_array[pid].priority = priority;

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