#include <irq_dispatcher.h>



static void int20();
static void int21();

static void ( *irq_routines[2] ) () = {
	int20,
	int21
};

void irqDispatcher ( uint64_t irq )
{
	irq_routines[irq]();
	return;
}


void int20()
{
	timerHandler();
}
void int21()
{
	keyboardHandler();
}
