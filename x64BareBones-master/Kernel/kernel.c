#include <kernel.h>
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
static const uint64_t PageSize = 0x1000;



const uint64_t shell_code_module_addr_int = 0x400000;
const uint64_t shell_data_module_addr_int = 0x500000;
const uint64_t heap_addr_int = 0x600000;

static void * const shell_code_module_address = (void *) * ( &shell_code_module_addr_int );
static void * const shell_data_module_address = (void *) * ( &shell_data_module_addr_int );
static void * const heap = (void *) * ( &heap_addr_int );


static MemoryManagerADT kernel_mem, userland_mem;
typedef int ( *EntryPoint ) ();


void clearBSS ( void * bssAddress, uint64_t bssSize )
{
	sharedLibcMemset ( bssAddress, 0, bssSize );
}

void * getStackBase()
{
	return ( void * ) (
	           ( uint64_t ) &endOfKernel
	           + PageSize * 8					//The size of the stack itself, 32KiB
	           - sizeof ( uint64_t )			//Begin at the top of the stack
	       );
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		shell_code_module_address,
		shell_data_module_address
	};
	loadModules ( &endOfKernelBinary, moduleAddresses );
	clearBSS ( &bss, &endOfKernel - &bss );
	return getStackBase();
}


void idleProcess()
{
	while ( 1 ) {
		_hlt();
	}
}

MemoryManagerADT getUserlandMem()
{
	return userland_mem;
}
MemoryManagerADT getKernelMem()
{
	return kernel_mem;
}


int main()
{
	loadIdt();
	kernel_mem = createMM ( heap );
	userland_mem = createMM ( heap + HEAP_SIZE );
	char * argv_idle[] = {"idle"};
	char * argv_shell[] = {"sh"};
	tFd idle_fds[3] = {-1, -1, -1};
	tFd shell_fds[3] = {STDOUT, STDERR, STDIN};
	initializeScheduler ( newProcess ( ( mainFunction ) shell_code_module_address, HIGH, 0, argv_shell, 1, shell_fds ), newProcess ( ( mainFunction ) idleProcess, LOW, 0, argv_idle, 1, idle_fds ) );
	pipeInit();
	initTimerHandler();
	timerTick();
	return 0;
}