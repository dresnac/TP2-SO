

#ifndef TP_SO_EXCEPTIONS_H
#define TP_SO_EXCEPTIONS_H

#include <video_driver.h>
#include <idt_loader.h>
#include <syscalls_kernel.h>
#include <interrupts.h>
#include <keyboard_driver.h>

void exceptionDispatcher ( uint64_t exception );

#endif
