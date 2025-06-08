#include <exceptions.h>


#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6
#define GENERAL_PROTECTION 13
#define CANT_REGS 18

extern uint64_t exception_regs[CANT_REGS];
//Order: RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8, R9, R10, R11, R12, R13, R14, R15, RIP, RFLAGS


static uint8_t * numToString ( uint64_t num, uint64_t base )
{
	static uint8_t buffer[64];
	uint8_t * ptr = &buffer[63];
	*ptr = '\0';
	do {
		*--ptr = "0123456789abcdef"[num % base];
		num /= base;
	} while ( num != 0 );
	return ptr;
}

static void printRegs ( uint8_t * message, uint8_t cant_chars_message )
{
	char * regs[] = {"rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rip", "rflags"};
	color col = {255, 255, 255};
	vdriverClearScreen ( col );
	vdriverSetMode ( TEXT_MODE, col );
	vdriverSetFontSize ( 2 );

	vdriverWrite ( STDERR, ( char * ) message, cant_chars_message );
	uint8_t newline = '\n';
	for ( int i = 0; i < CANT_REGS; i++ ) {
		vdriverWrite ( STDERR, regs[i], 10 );
		vdriverWrite ( STDERR, ": 0x", 4 );
		vdriverWrite ( STDERR, ( char * ) numToString ( exception_regs[i], 16 ), 10 );
		vdriverWrite ( STDERR, "h", 1 );
		vdriverWrite ( STDERR, ( char * ) &newline, 1 );
	}
}



void exceptionDispatcher ( uint64_t exception )
{

	uint8_t * message = ( uint8_t * ) "";
	uint8_t message_cant_chars = 1;
	if ( exception == ZERO_EXCEPTION_ID ) {
		message = ( uint8_t * ) "Zero div exception\n\n";
		message_cant_chars = 20;
	}

	if ( exception == OPCODE_EXCEPTION_ID ) {
		message = ( uint8_t * ) "Invalid opcode\n\n";
		message_cant_chars = 16;
	}
	if ( exception == GENERAL_PROTECTION ) {
		message = ( uint8_t * ) "General Protection\n\n";
		message_cant_chars = 20;
	}



	printRegs ( message, message_cant_chars );
	vdriverWrite ( STDERR, "\nPress any key to continue", 30 );

	picMasterMask ( 0xfd );                 //only keyboard interrupts
	_sti();
	while ( !bufferHasNext() ) {
		_hlt();
	}
	getCurrent();
	_cli();
	picMasterMask ( DEFAULT_MASTER_MASK );  // restores to default


	color c = {0, 0, 0};
	vdriverClearScreen ( c );
}