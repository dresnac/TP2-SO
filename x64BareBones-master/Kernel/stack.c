#include <stack.h>

#define RSI_POS 8
#define RDI_POS 9
#define RDX_POS 11

void processWrapper(main_function rip, char ** my_argv, uint64_t my_argc){
    int ret = rip(my_argv, my_argc);
}


uint64_t loadStack(uint64_t rip, uint64_t rsp, char ** my_argv, uint64_t my_argc){
    stack * my_stack = (stack*)(rsp-sizeof(stack));
    
    my_stack->regs.rdi = rip;
    my_stack->regs.rsi = my_argv;
    my_stack->regs.rdx = my_argc;

    my_stack->rip = &processWrapper;
    my_stack->cs = 0x8;
    my_stack->rflags = 0x202;
    my_stack->rsp = rsp;
    my_stack->ss = 0x0;
    return (uint64_t) my_stack;
}