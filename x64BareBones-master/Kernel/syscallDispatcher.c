#include <stdint.h> //para uint64_t
#include <stdarg.h> //para cantidad de argumentos variables
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <stddef.h>  //para size_t
#include <syscallDispatcher.h>
#include <time.h>
#include <rtc.h>
#include <irqDispatcher.h>

static int shiftFlag = 0;

static char * numToString(uint64_t num, uint64_t base);
static uint8_t bcd_decimal(uint8_t BCD);

extern int Hours();
extern int Mins();

extern uint64_t regs_shot[17];
extern uint64_t regs_shot_available;

static void (*syscall_manager[])() = {
    empty,
    empty,
    empty,
    read,
    write,
    clear_screen,
    delete,     //este llama directo a la func de video driver
    change_font_size,
    time,
    regs,
    put_rectangle,
    get_ticks,
    empty,
    empty,
    do_beep,
    //completar
    
};

void syscallDispatcher(pushed_registers * regs){ //en realidad serían args variables

    syscall_manager[regs->rax](regs);

}

void write(pushed_registers * regs){
    uint32_t colorByFD[] = { 0, 0x00FFFFFF, 0x00FF0000, 0x0000FF00 };
    vdPrint((char*) regs->rcx, regs->rdx, colorByFD[regs->rbx]);
}

void clear_screen(pushed_registers * regs){
    vdClearScreen();
}

void change_font_size(pushed_registers * regs){
    *(int*)(regs->rcx) = vdChangeFontSize(regs->rbx);
}

void empty(pushed_registers * regs){
    return;
}

void read(pushed_registers * regs){  //el FileDescriptor no sirve de mucho supongo
    uint64_t i = 0;
    char * buffer = (char*) regs->rcx;
    int amount = (int) regs->rdx;
    int * flag = regs->r10;
    while(i < amount && bufferHasNext()){
        buffer[i] = getCurrent();
        i++;
        *flag = 1;
    }
}

uint64_t read_with_params(uint64_t fd, uint16_t * buffer, uint64_t amount){
    uint64_t i = 0;

    while(i < amount && bufferHasNext()){
        buffer[i] = getCurrent();
        i++;
    }
    return i;
}


void time(pushed_registers * regs){
    
    LocalTime * time = regs->rbx;
    
    time->segundos = getRTCSeconds();
    time->minutos =  getRTCMinutes();
    time->horas =  getRTCHours();
    time->dias = getRTCDayOfMonth();
    time->mes = getRTCMonth();
    time->anio = getRTCYear();
}

void regs(pushed_registers * regs){
    Snapshot * snapshot = regs->rbx;
    int * flag = (int*)regs->rcx;

    if(!regs_shot_available) {
        *flag = -1;
        return;
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

    *flag = 0;
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

static uint8_t bcd_decimal(uint8_t BCD) {
    return (BCD >> 4) * 10 + (BCD & 0x0F);
}

void put_rectangle(pushed_registers * regs){
    vdDrawRectangle(regs->rbx, regs->rcx, regs->rdx, regs->r10, regs->r8);
}

void get_ticks(pushed_registers * regs){
    unsigned long * ticks = regs->rbx;
    *ticks = ticksElapsed();
}

void do_beep(pushed_registers * regs){
    beep(regs->rbx, regs->rcx);
}