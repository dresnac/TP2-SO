#include <kernel_libc.h>

char * newStrCpy( char * string){
    if (string == NULL){
        return NULL;
    }
    //inc
}

char ** cpyArgv (tPid pid, char ** argv, uint64_t argc){
    if(argc == 0 || argv == NULL) {
        return NULL;
    }
    char * resp = allocMemory(getKernelMem(),sizeof(char*));

    if(resp == NULL){
        return NULL;
    }

    for(uint64_t i=0; i<argc;i++){
        resp[i] = newStrCpy(argv[i]);
        if(resp[i] == NULL){
            for(uint64_t j=0; j<i; j++){
                freeMemory(getKernelMem(),(void*) resp[j]);  
            }
            freeMemory(getKernelMem(),(void*)resp);
            return NULL;
        }
    }
    return resp;
}