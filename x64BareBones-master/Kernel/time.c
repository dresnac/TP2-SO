#include <time.h>
//#include <lib.h>

static unsigned long ticks = 0;
static ordered_list_adt sleeping_list;

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