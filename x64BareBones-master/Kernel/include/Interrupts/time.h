#ifndef _TIME_H_
#define _TIME_H_

#include <orderedListADT.h>
#include <process.h>

void timerHandler();
int ticksElapsed();
int secondsElapsed();
int64_t nano_sleep(int time);
void unsleepKill(PCB * pcb);
void initTimerHandler();

#endif
