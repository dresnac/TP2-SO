#include <stack.h>



void processWrapper ( mainFunction rip, char ** argv, uint64_t argc, tPid pid )
{
	int ret = rip ( argv, argc );
	_cli();
	PCB * pcb = getPcb ( pid );
	if ( pcb == NULL ) {
		return;
	}
	makeMeZombie ( ret );
	timerTick();
}

uint64_t loadStack ( uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc, tPid pid )
{
	stack * my_stack = ( stack * ) ( rsp - sizeof ( stack ) );

	my_stack->regs.rdi = rip;
	my_stack->regs.rsi = ( uint64_t ) argv;
	my_stack->regs.rdx = argc;
	my_stack->regs.rcx = pid;
	my_stack->rip = ( uint64_t ) &processWrapper;
	my_stack->cs = 0x8;
	my_stack->rflags = 0x202;
	my_stack->rsp = rsp;
	my_stack->ss = 0x0;

	return ( uint64_t ) my_stack;
}