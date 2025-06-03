#include <stack.h>


void processWrapper(main_function rip, char ** my_argv, uint64_t my_argc, uint64_t pid){
    int ret = rip(my_argv, my_argc);
    PCB * pcb = getPcb(pid);
    if(pcb == NULL){
        return;
    }
    pcb->ret = ret;
    killProcess(pid);
    //__asm__("int $0x20");
}


uint64_t loadStack(uint64_t rip, uint64_t rsp, char ** my_argv, uint64_t my_argc, uint64_t pid){
    stack * my_stack = (stack*)(rsp-sizeof(stack));
    
    my_stack->regs.rdi = rip;
    my_stack->regs.rsi = (uint64_t)my_argv;
    my_stack->regs.rdx = my_argc;
    my_stack->regs.rcx = pid;
    my_stack->rip = (uint64_t)&processWrapper;
    my_stack->cs = 0x8;
    my_stack->rflags = 0x202;
    my_stack->rsp = rsp;
    my_stack->ss = 0x0;
    return (uint64_t) my_stack;
}