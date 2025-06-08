#include <time.h>
extern void _hlt();
static unsigned long ticks = 0;

static orderedListADT sleeping_list;

int compare ( PCB * pcb1, PCB * pcb2 )
{
	return ( ( pcb1->start + pcb1->time ) < ( pcb2->start + pcb2->time ) ) ? -1 : 1;
}

void initTimerHandler()
{
	sleeping_list = newOrderedList ( compare );
}


void unsleepKill ( PCB * pcb )
{
	if ( pcb == NULL ) {
		return;
	}
	deleteOrderedList ( sleeping_list, pcb );
}

void timerHandler()
{
	ticks++;
	if ( sleeping_list == NULL ) {
		return;
	}
	orderedListToBegin ( sleeping_list );
	while ( orderedListHasNext ( sleeping_list ) ) {
		PCB * current_pcb = orderedListNext ( sleeping_list );
		if ( current_pcb == NULL || ticks - current_pcb->start < current_pcb->time ) {
			return;
		}
		ready ( current_pcb );
		orderedListDeleteCurrent ( sleeping_list );
	}
}

int ticksElapsed()
{
	return ticks;
}

int secondsElapsed()
{
	return ticks / 18;
}

int64_t ticksSleep ( int time )
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