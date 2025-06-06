#include <time.h>
//#include <lib.h>
extern void _hlt();
static unsigned long ticks = 0;
static ordered_list_adt sleeping_list;

int compare ( PCB * pcb1, PCB * pcb2 )
{
	return ( ( pcb1->start + pcb1->time ) < ( pcb2->start + pcb2->time ) ) ? -1 : 1;
}

void timerHandler() {
	ticks++;
}

int ticksElapsed() {
	return ticks;
}

int secondsElapsed() {
	return ticks / 18;
}

int64_t nano_sleep ( int time )
{
	PCB * pcb  = getRunning();
	pcb->start = ticks;
	pcb->time = time;
	if ( addOrderedList ( sleeping_list, pcb ) != 0 ) {
		return -1;
	}
	blockCurrent();
	pcb->start = 0;
	pcb->time = 0;
	return 0;
}

void unsleepKill(PCB * pcb){
	if(pcb == NULL){
		return;
	}
	deleteOrderedList ( sleeping_list, pcb );
}



void initTimerHandler()
{
	sleeping_list = newOrderedList ( compare );
}

