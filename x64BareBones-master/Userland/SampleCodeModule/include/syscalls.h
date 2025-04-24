#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <localtime.h>

uint64_t sys_read(uint64_t fd, char * buffer, uint64_t count, int * flag);

uint64_t sys_write(uint64_t fd, const char * buffer, uint64_t count);

uint64_t sys_clear_screen(void);

uint64_t sys_time(LocalTime * time);

uint64_t sys_zoom(int,int*);

uint64_t sys_put_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color);

uint64_t sys_get_ticks(unsigned long * ticksBuff);

uint64_t sys_beep(uint64_t frequency, uint64_t duration);

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
} Snapshot;

uint64_t sys_get_regs(Snapshot *, int *);

#endif