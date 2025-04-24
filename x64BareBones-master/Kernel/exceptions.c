#include <stdint.h>
#include <exceptions.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <syscallDispatcher.h>

#define STDERR 1

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6

#define CANT_REGS 18
extern uint64_t exception_regs[CANT_REGS];

//Orden: RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8, R9, R10, R11, R12, R13, R14, R15, RIP, RFLAGS


static uint8_t * numToString(uint64_t num, uint64_t base) {
    static char buffer[64];
    uint8_t * ptr = &buffer[63];
    *ptr = '\0';
    do {
        *--ptr = "0123456789abcdef"[num % base];
        num /= base;
    } while(num != 0);
    return ptr;
}

static void printRegs(uint8_t * message, uint8_t cant_chars_message){
    uint8_t * regs[] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "RFLAGS"};
    vdClearScreen();
    // vdriver_set_mode(TEXT_MODE, col);
    // vdriver_text_set_font_size(2);

    vdPrint(message, cant_chars_message, 0x00FF0000);
    // uint8_t newline = '\n';
    for(int i=0; i<CANT_REGS;i++){
        vdPrint(regs[i], 10, 0x00FF0000);
        vdPrint( ": 0x", 4, 0x00FF0000);
        vdPrint( numToString(exception_regs[i],16), 10, 0x00FF0000);
        vdPrint( "h", 1, 0x00FF0000);
        newline();
    }
}



void exceptionDispatcher(uint64_t exception) {

    uint8_t * message = "";
    uint8_t message_cant = 1;
	if (exception == ZERO_EXCEPTION_ID){
        message = "Zero div exception\n\n";
        message_cant = 20;
    }

	if(exception == OPCODE_EXCEPTION_ID){
        message = "Invalid opcode\n\n";
        message_cant= 16;
    }

    printRegs(message, message_cant);
    vdPrint("\nPress any key to continue", 30, 0x00FF0000);
    uint16_t buffer;



    picMasterMask(0xFD);                    //Solo habilito interrupciones de teclado
    _sti();
    while(read_with_params(1, &buffer,1) == 0);
    _cli();
    picMasterMask(0xFC); 

	vdClearScreen();
}





// #define ZERO_EXCEPTION_ID 0

// static void zero_division();

// void exceptionDispatcher(int exception) {
// 	if (exception == ZERO_EXCEPTION_ID)
// 		zero_division();
// }

// static void zero_division() {
// 	// Handler para manejar excepcíon
// }