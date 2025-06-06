#include <syscalls_kernel.h>
#include <syscalls.h>
//resolver esto porque no anda 

static int64_t sys_free_ps_wrapper (process_info_list * ps);
static int64_t sys_free_wrapper ( void * p);

extern uint64_t regs_shot[17];
extern uint64_t regs_shot_available;

static PCB * blocked; //esto tiene que ir a videoDriver

typedef uint64_t (*sys_function) (uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

static sys_function syscall_table[NUM_SYSCALLS] = {
    (sys_function) sys_read,
    (sys_function) sys_write,
    (sys_function) sys_clear_screen,
    (sys_function) sys_get_register_snapshot,
    (sys_function) sys_beep,
    (sys_function) sys_set_font_size,
    (sys_function) sys_put_rectangle,
    (sys_function) sys_ticks_sleep,
    (sys_function) sys_get_time,
    (sys_function) sys_malloc,
    (sys_function) sys_free_wrapper,
    (sys_function) sys_get_pid,
    (sys_function) sys_create_process,
    (sys_function) sys_block,
    (sys_function) sys_unblock,
    (sys_function) sys_yield,
    (sys_function) sys_nice,
    (sys_function) sys_kill,
    (sys_function) sys_wait,
    (sys_function) sys_ps,                       
	(sys_function) sys_free_ps_wrapper,          
	(sys_function) sys_get_status,
    (sys_function) sys_get_my_fds,
    (sys_function) sys_mem_info
};

int64_t syscallDispatcher (stack_registers * regs){
    if(regs->rax >= NUM_SYSCALLS){
        return NOT_VALID_SYS_ID;
    }
	return syscall_table[regs->rax] ( ( uint64_t ) regs->rdi, ( uint64_t ) regs->rsi, ( uint64_t ) regs->rdx, ( uint64_t ) regs->rcx, ( uint64_t ) regs->r8, ( uint64_t ) regs->r9 );
}

int64_t sys_get_my_fds(int64_t fds[CANT_FDS]){
    PCB * pcb = getRunning();
    if (pcb == NULL || fds == NULL) {
        return -1;
    }
    for(int i=0; i<CANT_FDS; i++){
        fds[i] = pcb->fds[i];
    }
    return 0;
}

int64_t sys_pipe_open ( int64_t id, tPipeMode mode )
{
	if ( id < 3 ) {
		return -1;
	}
	return pipeOpen ( id - 3, mode );
}

int64_t sys_pipe_open_free ( tPipeMode mode )
{
	return pipeOpenFree ( mode ) + 3 ;
}

int64_t sys_pipe_read ( int64_t id, uint8_t * buffer, uint64_t amount )
{
	if ( id < 3 ) {
		return -1;
	}
	return pipeRead ( id - 3, buffer, amount );
}

int64_t sys_pipe_write ( int64_t id, uint8_t * buffer, uint64_t amount )
{
	if ( id < 3 ) {
		return -1;
	}
	return pipeWrite ( id - 3, buffer, amount, getPid() );
}

int64_t sys_pipe_close ( int64_t id )
{
	if ( id < 3 ) {
		return -1;
	}
	return pipeClose ( id - 3, getPid() );
}

int64_t sys_pipe_reserve()
{
	return pipeReserve() + 3;
}

//la logica de la flag ya no seria igual.
int64_t sys_read (uint8_t * buffer, uint64_t amount){
    uint64_t i = 0;
    //int * flag = regs->r10;
    if(blocked != NULL){
        return -1;
    }
    if ( !bufferHasNext() ) {
		blocked = getRunning();
		blockCurrent();
	}
    if ( buffer[buffer_urrent] == EOF ) {
		getCurrent();
	}//inc hay que mandar esto a videoDriver

    while(i < amount && bufferHasNext()){
        buffer[i] = getCurrent();
        i++;
    //    *flag = 1;
    }
}

int64_t sys_write(uint64_t fd, uint8_t * buffer, uint64_t amount){
    uint32_t colorByFD[] = { 0, 0x00FFFFFF, 0x00FF0000, 0x0000FF00 };
    vdPrint(buffer, amount, colorByFD[fd]);
    return 0;
}

int64_t sys_set_font_size ( uint64_t size )
{
	return vdChangeFontSize(size);
}

int64_t sys_beep ( uint32_t freq, uint32_t time )
{
	beep ( freq, time );
	return 0;
}

int64_t sys_put_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color * color )
{
    //chequear orden de los params y hacer que devuelva int
    vdDrawRectangle(x, y, width, height, color);
    return 0; //cambiar
}

int64_t sys_ticks_sleep(uint64_t ns){
    nano_sleep(ns);
    return 0;
}

int64_t sys_get_register_snapshot(Snapshot * snapshot){
    //despues agregar esta logica
    /*
    if(!regs_shot_available) {
        *flag = -1;
        return;
    }*/ 

    snapshot->rax = regs_shot[0];
    snapshot->rbx = regs_shot[1];
    snapshot->rcx = regs_shot[2];
    snapshot->rdx = regs_shot[3];
    snapshot->rsi = regs_shot[4];
    snapshot->rdi = regs_shot[5];
    snapshot->rbp = regs_shot[6];
    snapshot->rsp = regs_shot[7];
    snapshot->r8 = regs_shot[8];
    snapshot->r9 = regs_shot[9];
    snapshot->r10 = regs_shot[10];
    snapshot->r11 = regs_shot[11];
    snapshot->r12 = regs_shot[12];
    snapshot->r13 = regs_shot[13];
    snapshot->r14 = regs_shot[14];
    snapshot->r15 = regs_shot[15];
    snapshot->rip = regs_shot[16];

    return 0;
}

int64_t sys_clear_screen(){
    vdClearScreen();
    return 0; //hacer que vd devuelva int
}

int64_t sys_get_time (LocalTime * time){
    time->segundos = getRTCSeconds();
    time->minutos =  getRTCMinutes();
    time->horas =  getRTCHours();
    time->dias = getRTCDayOfMonth();
    time->mes = getRTCMonth();
    time->anio = getRTCYear();
    return 0;
}

void * sys_malloc (uint64_t size){
    return allocMemory(getUserlandMem(), size);
}

static int64_t sys_free_wrapper(void * p){
    freeMemory(p, getUserlandMem());
    return 0;
}

int64_t sys_create_process (main_function rip, tPriority priority, char ** argv, uint64_t argc, int64_t fds[]){
    return (int64_t) newProcess(rip, priority,1,argv,argc,fds);
}

int64_t sys_get_pid(){
    return getPid();
}

int64_t sys_nice( tPid pid, uint64_t new_prio){
    return nice(pid, new_prio);
}

int64_t sys_kill (tPid pid){
    return killProcess(pid);
}

int64_t sys_block(tPid pid){
    return blockArbitrary(pid);
}

int64_t sys_unblock(tPid pid){
    return unblockArbitrary(pid);
}

int64_t sys_yield(){
    schedulerYield();
    return 0;
}

int64_t sys_wait ( tPid pid, int64_t * ret )
{
	return wait ( pid, ret );
}

process_info_list * sys_ps ()
{
	return 0; //ps();
}

static int64_t sys_free_ps_wrapper ( process_info_list * ps )
{
	//freePs ( ps );
	return 0;
}

int8_t sys_get_status ( tPid pid )
{
	return getStatus ( pid );
}

int64_t sys_mem_info ( memoryInfo info[2] )
{
	return memInfo ( &info[0], getUserlandMem() ) + memInfo ( &info[1], getKernelMem() ) ;
}

static char * numToString(uint64_t num, uint64_t base) {  //creo que se puede borrar
    static char buffer[64];
    char * ptr = &buffer[63];
    *ptr = '\0';
    do {
        *--ptr = "0123456789abcdef"[num % base];
        num /= base;
    } while(num != 0);
    return ptr;
}

uint64_t read_with_params(uint64_t fd, uint16_t * buffer, uint64_t amount){
    uint64_t i = 0;

    while(i < amount && bufferHasNext()){
        buffer[i] = getCurrent();
        i++;
    }
    return i;
}