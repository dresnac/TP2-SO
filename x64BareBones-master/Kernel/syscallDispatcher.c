#include <syscalls_kernel.h>
#include <syscalls.h>
//resolver esto porque no anda 

static int64_t sys_free_ps_wrapper (process_info_list * ps);
static int64_t sys_free_wrapper ( void * p);
static int64_t empty();

extern uint64_t regs_shot[17];
extern uint64_t regs_shot_available;

static PCB * blocked; //esto tiene que ir a videoDriver

typedef uint64_t (*sys_function) (uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

static sys_function syscall_table[NUM_SYSCALLS] = {
    (sys_function) sys_read,    //0
    (sys_function) sys_write,   //1
    (sys_function) sys_get_register_snapshot,  //2
    (sys_function) sys_beep,   //3
    (sys_function) sys_set_font_size,   //4
    (sys_function) sys_clear_screen,  //5
    (sys_function) empty,   //sys_put_pixel,                // 6
    (sys_function) sys_put_rectangle,   //7
    (sys_function) empty,  //sys_draw_letter,              // 8
    (sys_function) empty, //sys_set_mode,                 // 9
	(sys_function) empty, //sys_get_screen_information,   // 10
    (sys_function) sys_ticks_sleep,     //11
    (sys_function) sys_get_time,       //12
    (sys_function) sys_malloc,       //13
    (sys_function) sys_free_wrapper,    //14
    (sys_function) sys_get_pid,    //15
    (sys_function) sys_create_process,    //16
    (sys_function) sys_block,    //17
    (sys_function) sys_unblock,    //18
    (sys_function) sys_yield,     //19
    (sys_function) sys_nice,     //20
    (sys_function) sys_kill,     //21
    (sys_function) sys_wait,     //22
    (sys_function) sys_sem_open,                 // 23
	(sys_function) sys_sem_wait,                 // 24
	(sys_function) sys_sem_post,                 // 25
	(sys_function) sys_sem_close,                // 26
    (sys_function) sys_ps,       //27       
	(sys_function) sys_free_ps_wrapper,   //28      
	(sys_function) sys_get_status,    //29
    (sys_function) sys_pipe_open,                // 30
	(sys_function) sys_pipe_open_free,           // 31
	(sys_function) sys_pipe_read,                // 32
	(sys_function) sys_pipe_write,               // 33
	(sys_function) sys_pipe_close,               // 34
	(sys_function) sys_pipe_reserve,             // 35
	(sys_function) sys_sem_open_get_id,          // 36
    (sys_function) sys_get_my_fds,    //37
    (sys_function) sys_mem_info       //38
};

int64_t syscallDispatcher (stack_registers * regs){
    if(regs->rax >= NUM_SYSCALLS){
        return NOT_VALID_SYS_ID;
    }
	return syscall_table[regs->rax] ( ( uint64_t ) regs->rdi, ( uint64_t ) regs->rsi, ( uint64_t ) regs->rdx, ( uint64_t ) regs->rcx, ( uint64_t ) regs->r8, ( uint64_t ) regs->r9 );
}

static int64_t empty(){
    return 0;
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
    int64_t fd = getRunning()->fds[STDIN];
	if ( fd == STDIN ) {
		return stdinRead ( buffer, amount );
	}
	return sys_pipe_read ( fd, buffer, amount );
}

int64_t sys_write(uint64_t fd, uint8_t * buffer, uint64_t amount){

    if(fd != STDOUT && fd != STDERR){
        return -1;
    }

    int64_t actual_fd = getRunning()->fds[fd];
    
    if(actual_fd == STDOUT || actual_fd == STDERR){
        uint32_t colorByFD[] = { 0, 0x00FFFFFF, 0x00FF0000, 0x0000FF00 };
        return vdPrint(buffer, amount, colorByFD[fd]);
    }
    return sys_pipe_write(actual_fd, buffer, amount);

    
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
    vdDrawRectangle(x, y, width, height, color);
    return 0;
}

int64_t sys_ticks_sleep(uint64_t ns){
    nano_sleep(ns);
    return 0;
}

/*  IMPLEMENTAR
int64_t sys_draw_letter ( uint64_t x, uint64_t y, char * letter, color * color, uint64_t font_size )
{
	return vdriver_video_draw_font ( x, y, *letter, *color, font_size );
}*/

// int64_t sys_put_pixel ( uint64_t x, uint64_t y, color * color )
// {
// 	return vdriver_video_draw_pixel ( x, y, *color );
// }

// int64_t sys_get_screen_information ( screen_information * screen_information )
// {
// 	return vdriver_get_screen_information ( screen_information );
// }

// int64_t sys_set_mode ( uint64_t mode )
// {
// 	return vdriver_set_mode ( mode, ( color ) {
// 		0, 0, 0
// 	} );
// }
// */

int64_t sys_get_register_snapshot(Snapshot * snapshot){
    
    if(!regs_shot_available) {
        return -1;
    }

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

int64_t sys_sem_open ( int64_t sem_id, int value )
{
	return mySemOpen ( sem_id, value, 0 );
}

int64_t sys_sem_wait ( int64_t sem_id )
{
	return mySemWait ( sem_id, 0 );
}

int64_t sys_sem_post ( int64_t sem_id )
{
	return mySemPost ( sem_id, 0 );
}

int64_t sys_sem_open_get_id ( int value )
{
	return mySemOpenGetId ( value );
}

int64_t sys_sem_close ( int64_t sem_id )
{
	return mySemClose ( sem_id, 0 );
}


process_info_list * sys_ps ()
{
	return ps();
}

static int64_t sys_free_ps_wrapper ( process_info_list * ps )
{
	freePs ( ps );
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