#ifndef IRQDISPATCHER
#define IRQDISPATCHER

#include <time.h>
#include <keyboard_driver.h>
#include <stdint.h>

void irqDispatcher ( uint64_t irq );

#endif