#ifndef _TIME_H_
#define _TIME_H_
#include <ordered_list_adt.h>
#include <process.h>
void timerHandler();
int ticksElapsed();
int secondsElapsed();
int64_t ticksSleep ( int time );
void initTimerHandler();
void unsleepKill ( PCB * pcb );

#endif
