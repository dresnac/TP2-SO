
#include <kernel.h>



extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


static const uint64_t PageSize = 0x1000;

const uint64_t sampleCodeModuleAddress = 0x400000;
const void * const sampleDataModuleAddress = (void*)0x500000;

static void * const shell_code_module_address = (void *) * ( &sampleCodeModuleAddress );


static void * const heap = ( void * ) 0x600000;

static MemoryManagerADT kernel_mem;

typedef int (*EntryPoint)();




void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}
/*
void writeArg(char ** my_argv, uint64_t my_argc){
	if(my_argc != 1) return;
	//inc
}*/

void idleProcess(){
	while(1){
		_hlt();
	}
}

MemoryManagerADT getKernelMem()
{
	return kernel_mem;
}

int main()
{	
	load_idt();
	MemoryManagerADT kernel_mem = createMemoryManager( heap, HEAP_SIZE);
	char * argv_idle[] = {"idle"};
	char * argv_shell[] = {"sh"};
	int64_t idle_fds[3] = {-1,-1,-1};
	int64_t shell_fds[3] = {0, 1, 2};
	initializeScheduler ( newProcess ( ( main_function ) shell_code_module_address, HIGH, 0, argv_shell, 1, shell_fds ), newProcess ( ( main_function ) idleProcess, LOW, 0, argv_idle, 1, idle_fds ) );

	newProcess((main_function) sampleCodeModuleAddress,0, HIGH, NULL, 0, NULL);
	//int64_t pid = newProcess((uint64_t) rec, HIGH); /hacer la func rec
	__asm__("int $0x20");
	return 0;
}

//ayuda dios porfa
