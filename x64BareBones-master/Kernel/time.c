#include <time.h>
//#include <lib.h>

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

void nano_sleep(int time){
	int start = ticks;
	while(ticks - start < time){
		hlt();
	}
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

