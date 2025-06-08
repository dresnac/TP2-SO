#include <process.h>



PCB pcb_array[PCB_AMOUNT] = {0};
uint64_t amount_of_processes = 0;

static int64_t findFreePcb();
static int64_t setFreePid ( tPid pid );
static int64_t setFreePcb ( PCB * process );
static int64_t setupPipe ( tPid pid, tFd fds[] );

int8_t getStatus ( tPid pid )
{
	PCB * process = getPcb ( pid );
	if ( process == NULL ) {
		return -1;
	}
	return process->status;
}


//Returns PID if it was succesfull and -1 if it wasnt.
tPid wait ( tPid pid, int64_t * ret )
{

	PCB * pcb_to_wait = getPcb ( pid );
	if ( ( pcb_to_wait == NULL ) || ( pcb_to_wait->status == FREE ) || ( pcb_to_wait->waiting_me != NULL ) || ( pid == getPid() ) ) {
		return -1;
	}
	if ( ! ( pcb_to_wait->status == ZOMBIE ) ) {
		PCB * running = getRunning();
		pcb_to_wait->waiting_me = running;
		running->waiting_for = pcb_to_wait;
		blockCurrent();
		running->waiting_for = NULL;
	}
	if ( ! ( ( pcb_to_wait->status == ZOMBIE ) ) ) { // This could happen if it was killed
		return -1;
	}

	if ( ret != NULL ) {
		*ret = pcb_to_wait->ret;
	}

	if ( setFreePid ( pid ) != -1 ) {
		amount_of_processes--;
	}
	return pid;

}



int64_t newProcess ( mainFunction rip, tPriority priority, uint8_t killable, char ** argv, uint64_t argc, tFd fds[] )
{

	if ( ( ( priority != LOW ) && ( priority != MEDIUM ) && ( priority != HIGH ) ) ) {
		return -1;
	}

	tPid pid = findFreePcb();
	if ( pid == -1 ) {
		return -1;
	}

	uint64_t rsp_malloc = ( uint64_t ) allocMemory ( STACK_SIZE,  getKernelMem() ) ;
	uint64_t rsp = rsp_malloc + STACK_SIZE;

	if ( ( void * ) rsp_malloc == NULL ) {
		return -1;
	}

	char ** args_cpy = copyArgv ( pid, argv, argc );
	if ( argc > 0 && args_cpy == NULL ) {
		freeMemory ( ( void * ) rsp_malloc, getKernelMem() );
		pcb_array[pid].status = FREE;
		return -1;
	}


	rsp = loadStack ( ( uint64_t ) rip, rsp, args_cpy, argc, pid );

	pcb_array[pid].pid = pid;
	pcb_array[pid].rsp = rsp;
	pcb_array[pid].status = READY;
	pcb_array[pid].argv = args_cpy;
	pcb_array[pid].argc = argc;
	pcb_array[pid].priority = priority;
	pcb_array[pid].killable = killable;
	pcb_array[pid].waiting_me = NULL;
	pcb_array[pid].lowest_stack_address = rsp_malloc;
	pcb_array[pid].blocked_by_sem = -1;
	for ( int i = 0; i < 3; i++ ) {
		pcb_array[pid].fds[i] = fds ? fds[i] : -1;
	}

	if ( setupPipe ( pid, fds ) == -1 ) {
		freeMemory ( ( void * ) rsp_malloc, getKernelMem() );
		for ( uint64_t i = 0; i < pcb_array[pid].argc ; i++ ) {
			freeMemory ( ( void * ) pcb_array[pid].argv[i], getKernelMem() );
		}
		freeMemory ( ( void * ) pcb_array[pid].argv, getKernelMem() );
		pcb_array[pid].status = FREE;
		return -1;
	}

	ready ( &pcb_array[pid] );
	amount_of_processes++;
	return pid;
}


static int64_t setupPipe ( tPid pid, tFd fds[] )
{
	if ( !fds ) {
		return 0;
	}
	for ( int i = 0; i < CANT_FDS; i++ ) {
		if ( fds[i] <= MAX_COMMON_FD ) {
			continue;
		}
		tPipeMode mode = i == STDIN ? READER : WRITER;
		if ( pipeOpenPid ( fds[i] - 3, mode, pid ) == -1 ) {
			for ( int j = 0; j < i ; j++ ) {
				pipeClose ( fds[i] - 3, pid );
			}
			return -1;
		}
	}
	return 0;
}

static int64_t findFreePcb()
{
	int64_t to_return = 0;

	while ( to_return < PCB_AMOUNT && pcb_array[to_return].status != FREE ) {
		to_return++;
	}

	if ( to_return == PCB_AMOUNT ) {
		return -1;
	}

	return to_return;
}

PCB * getPcb ( tPid pid )
{
	if ( pid >= PCB_AMOUNT || pid < 0 ) {
		return NULL;
	}
	return &pcb_array[pid];
}

static int64_t setFreePcb ( PCB * process )
{
	if ( process == NULL || process->status == FREE ) {
		return -1;
	}
	closeFds ( process );

	freeMemory ( ( void * ) process->lowest_stack_address, getKernelMem() );
	if ( process->argv == NULL ) {
		process->status = FREE;
		return 0;
	}
	for ( uint64_t i = 0; i < process->argc ; i++ ) {
		freeMemory ( ( void * ) process->argv[i], getKernelMem() );
	}
	freeMemory ( ( void * ) process->argv, getKernelMem() );
	process->status = FREE;

	return 0;
}

static int64_t setFreePid ( tPid pid )
{
	PCB * process = getPcb ( pid );
	return setFreePcb ( process );
}

int64_t killProcessPcb ( PCB * pcb )
{
	if ( ( pcb == NULL ) || ( pcb->status == FREE ) || ( !pcb->killable ) ) {
		return -1;
	}

	unschedule ( pcb );
	unblockWaitingPcb ( pcb );
	if ( pcb->waiting_for && pcb->waiting_for->waiting_me ) {
		pcb->waiting_for->waiting_me = NULL;
	}

	if ( getKeyboardBlocked() == pcb ) {
		setKeyboardBlockedNull();
	}
	if ( pcb->time != 0 || pcb->start != 0 ) {
		unsleepKill ( pcb );
	}

	semDeleteFromBlockedQueue ( pcb );
	if ( setFreePcb ( pcb ) != -1 ) {
		amount_of_processes--;
	}

	if ( pcb == getRunning() ) {
		setRunningNull();
		timerTick();
	}

	return 0;
}


int64_t killProcess ( tPid pid )
{
	PCB * pcb = getPcb ( pid );
	return killProcessPcb ( pcb );
}


static int64_t isForeground ( PCB * pcb )
{
	if ( pcb == NULL ) {
		return 0;
	}
	PCB * shell_pcb = getShellPcb();
	if ( shell_pcb == NULL ) {
		return 0;
	}
	PCB * fore1 = shell_pcb->waiting_for;
	if ( fore1 == NULL ) {
		if ( pcb == shell_pcb ) {
			return 1;
		}
		return 0;
	}
	if ( fore1 == pcb ) {
		return 1;
	}

	PCB * fore2 = NULL;
	if ( fore1->fds[STDIN] > MAX_COMMON_FD ) {
		fore2 = getPcb ( pipeGetPid ( fore1->fds[STDIN] - 3, WRITER ) );
	}
	if ( fore2 != NULL && fore2 == pcb ) {
		return 1;
	}
	return 0;
}

void getProcessInfo ( PCB * pcb, ProcessInfo * process )
{
	process->name = newStrCopy ( pcb->argv != NULL ? pcb->argv[0] : NULL ); // If malloc fails, we print it as "No name" but keep the rest of the state.
	process->pid = pcb->pid;
	process->priority = pcb->priority;
	process->stack_pointer = pcb->rsp;
	process->lowest_stack_address = pcb->lowest_stack_address;
	process->status = pcb->status;
	process->is_background = !isForeground ( pcb );
	for ( int i = 0; i < 3; i++ ) {
		process->fds[i] = pcb->fds[i] ? pcb->fds[i] : -1;
	}
}


ProcessInfoList * ps()
{
	ProcessInfoList * process_list = allocMemory ( sizeof ( ProcessInfoList ),  getKernelMem() );
	if ( process_list == NULL ) {
		return NULL;
	}
	process_list->amount_of_processes = amount_of_processes;
	ProcessInfo * processes = allocMemory ( amount_of_processes * sizeof ( ProcessInfo ), getKernelMem() );
	if ( processes == NULL ) {
		freeMemory ( ( void * ) process_list, getKernelMem() );
		return NULL;
	}

	for ( int i = 0, found = 0; i < PCB_AMOUNT && found < amount_of_processes; i++ ) {
		if ( pcb_array[i].status != FREE ) {
			getProcessInfo ( &pcb_array[i], &processes[found] );
			found++;
		}
	}

	process_list->processes = processes;
	return process_list;
}

void freePs ( ProcessInfoList * ps )
{
	if ( ps == NULL || ps->processes == NULL ) {
		return;
	}
	for ( int i = 0; i < ps->amount_of_processes ; i++ ) {
		if ( ps->processes[i].name ) {
			freeMemory ( ps->processes[i].name,  getKernelMem() );
		}
	}
	freeMemory ( ps->processes, getKernelMem() );
	freeMemory ( ps, getKernelMem() );
}

void closeFds ( PCB * pcb )
{
	if ( pcb == NULL ) {
		return;
	}
	for ( int i = 0; i < CANT_FDS ; i++ ) {
		if ( pcb->fds[i] > MAX_COMMON_FD ) {
			pipeClose ( pcb->fds[i] - CANT_FDS, pcb->pid );
		}
	}
}

int64_t makeMeZombie ( int64_t retval )
{
	PCB * pcb = getRunning();
	if ( ( pcb == NULL ) || ( pcb->status == FREE ) ) {
		return -1;
	}
	pcb->ret = retval;
	unschedule ( pcb );
	pcb->status = ZOMBIE;
	unblockWaitingMe();
	closeFds ( pcb );
	return 0;
}



void ctrlcHandler()
{
	PCB * shell_pcb = getShellPcb();
	PCB * foreground_process;
	if ( shell_pcb == NULL || ( ( foreground_process = shell_pcb->waiting_for ) == NULL ) ) {
		return;
	}
	PCB * other_process_in_pipe = NULL;
	if ( foreground_process->fds[STDIN] > MAX_COMMON_FD ) {
		other_process_in_pipe = getPcb ( pipeGetPid ( foreground_process->fds[STDIN] - 3, WRITER ) );
	}
	killProcessPcb ( foreground_process );
	killProcessPcb ( other_process_in_pipe );
	return;
}


